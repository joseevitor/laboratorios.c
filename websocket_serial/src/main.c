#include "main.h"
#include <string.h> // Para strlen
#include <xc.h>     // Para registradores do PIC

// Macro com compensação de arredondamento para divisor de 4 (BRG16=1, BRGH=1)
#define SPBRG_VAL (((FOSC_HZ + (2 * BAUD_RATE)) / (4 * BAUD_RATE)) - 1)

// --- Variáveis Globais para o Buffer Circular ---
volatile unsigned char rx_buffer[RX_BUFFER_SIZE];
volatile unsigned char rx_head = 0; // Ponteiro de escrita
volatile unsigned char rx_tail = 0; // Ponteiro de leitura

// --- Implementação das Funções do Buffer Circular ---

/**
 * @brief Adiciona um byte ao buffer circular.
 * @param data O byte a ser adicionado.
 */
void buffer_put(unsigned char data) {
    unsigned char next_head = (rx_head + 1) & (RX_BUFFER_SIZE - 1); // Próxima posição de escrita

    // Verifica se o buffer está cheio (next_head == rx_tail)
    if (next_head != rx_tail) {
        rx_buffer[rx_head] = data;
        rx_head = next_head;
    }
    // Se o buffer estiver cheio, o dado é descartado.
    // Em aplicações críticas, pode-se adicionar um flag de erro ou expandir o buffer.
}

/**
 * @brief Lê um byte do buffer circular.
 * @param data Ponteiro para armazenar o byte lido.
 * @return 1 se um byte foi lido, 0 se o buffer está vazio.
 */
unsigned char buffer_get(unsigned char *data) {
    if (rx_head == rx_tail) {
        return 0; // Buffer vazio
    } else {
        *data = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) & (RX_BUFFER_SIZE - 1); // Próxima posição de leitura
        return 1; // Byte lido com sucesso
    }
}

// --- Funções de Comunicação Serial (UART) ---

/**
 * @brief Envia um único caractere pela UART.
 * @param c O caractere a ser enviado.
 */
void send_char(unsigned char c) {
    while (!TXSTA1bits.TRMT); // Aguarda até que o registrador de transmissão esteja vazio
    TXREG1 = c;               // Carrega o caractere para o registrador de transmissão
}

/**
 * @brief Envia uma string pela UART.
 * @param str A string a ser enviada.
 */
void send_string(const char *str) {
    while (*str) {
        send_char(*str++);
    }
}

// --- Funções de Inicialização ---

/**
 * @brief Inicializa o oscilador, portas I/O e o módulo EUSART.
 */
void init_system(void) {
    // --- Configuração do Oscilador ---
    // Os fuses já configuram o oscilador para 48MHz (Fosc) com cristal de 20MHz.
    // Apenas aguarda a estabilização do PLL.
    //while (!OSCSTATbits.PLLR); // Aguarda o PLL travar (lock)

    // --- Configuração das Portas I/O ---
    // Desabilita módulos analógicos em PORTA e PORTB para garantir que sejam digitais
    ADCON1 = 0x0F;  // Todos os pinos AN0-AN12 como digitais
    //ANSELH = 0x00;  // Pinos AN8-AN12 (RB0-RB4) como digitais

    // Configura pinos da UART (RC6/TX e RC7/RX)
    TRISCbits.TRISC6 = 0; // RC6 como saída (TX)
    TRISCbits.TRISC7 = 1; // RC7 como entrada (RX)

// 1. Configuração da Arquitetura do EUSART
    TXSTA1bits.SYNC = 0;   // Modo assíncrono
    TXSTA1bits.BRGH = 1;   // Alta velocidade de baud rate
    BAUDCONbits.BRG16 = 1; // RIGOROSO: Habilita gerador de 16 bits para 48MHz

    // 2. Atribuição Dinâmica (Tempo de Compilação)
    SPBRGH = (unsigned char)((SPBRG_VAL >> 8) & 0xFF); // Extrai o byte High
    SPBRG1  = (unsigned char)(SPBRG_VAL & 0xFF);        // Extrai o byte Low
    
    // 3. Ativação dos Módulos
    TXSTA1bits.TXEN = 1;   // Habilitar transmissor
    RCSTA1bits.SPEN = 1;   // Habilitar porta serial
    RCSTA1bits.CREN = 1;   // Habilitar receptor contínuo
}

/**
 * @brief Inicializa as interrupções globais e da UART.
 */
void init_interrupts(void) {
    // Habilitar prioridade de interrupções (se desejado)
    RCONbits.IPEN = 1;

    // Configurar prioridade da interrupção de recepção UART (Alta prioridade)
    IPR1bits.RCIP = 1; // 1 = Alta prioridade, 0 = Baixa prioridade

    // Habilitar interrupção de recepção UART
    PIE1bits.RCIE = 1;

    // Limpar flag de interrupção de recepção UART
    PIR1bits.RCIF = 0; // Garante que não haja interrupção pendente

    // Habilitar interrupções periféricas
    INTCONbits.PEIE = 1;

    // Habilitar interrupções globais
    INTCONbits.GIE = 1;
}

// --- Rotina de Serviço de Interrupção (ISR) ---

/**
 * @brief Rotina de Serviço de Interrupção (ISR).
 *        Configurada para alta prioridade.
 */
void __interrupt(high_priority) isr(void) {
    // Verifica se a interrupção é da recepção UART
    if (PIR1bits.RCIF) {
        // Tratamento de erros de recepção
        if (RCSTA1bits.OERR) { // Overrun Error: buffer de hardware cheio, dado perdido
            // Para limpar o erro de Overrun, é necessário desabilitar e reabilitar o receptor
            RCSTA1bits.CREN = 0;
            RCSTA1bits.CREN = 1;
        }
        // FERR (Framing Error) é limpo automaticamente ao ler RCREG1.
        // Não é necessário tratamento explícito aqui, a menos que se queira registrar o erro.

        // Lê o caractere recebido do registrador RCREG1
        // A leitura de RCREG1 automaticamente limpa o flag PIR1bits.RCIF
        unsigned char received_char = RCREG1;

        // Coloca o caractere no buffer circular
        buffer_put(received_char);

        // O flag PIR1bits.RCIF é limpo automaticamente ao ler RCREG1.
        // Não é necessário limpar manualmente aqui.
    }

    // Se houver outras interrupções (ex: Timer, ADC), elas seriam tratadas aqui<br/>
    // verificando seus respectivos flags (ex: if(PIR1bits.TMR1IF) { ... })
}

// --- Função Principal ---

void main(void) {
    unsigned char received_data;

    // Inicializa o sistema (oscilador, UART, etc.)
    init_system();

    // Inicializa as interrupções
    init_interrupts();

    while (1) {
        // O loop principal pode realizar outras tarefas aqui
        // sem ser bloqueado pela espera de dados da UART.
        // Por exemplo, piscar um LED, ler um sensor, etc.

        send_char('U');
        // Verifica se há dados no buffer de recepção
        /*if (buffer_get(&received_data)) {
            // Se houver dados, processa-os (neste caso, faz um echo)
            send_string("Echo: ");
            send_char(received_data);
            send_string("\r");
        }*/
        
        for(int i = 0; i < 50; i++)
            __delay_ms(10);

        // Exemplo de tarefa não-bloqueante no loop principal:
        // Delay1KTCYx(100); // Pequeno atraso para simular outras tarefas
    }
}
