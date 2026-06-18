#include <xc.h>

// CONFIGURAÇÕES

// CONFIG1L
#pragma config PLLDIV = 5        // Divide o clock do cristal por 5 (20 MHz / 5 = 4 MHz)
#pragma config CPUDIV = OSC1_PLL2// Clock do CPU vem do PLL (96MHz) dividido por 2 = 48MHz
#pragma config USBDIV = 2        // Clock do USB vem do PLL (96MHz) dividido por 2 = 48MHz

// CONFIG1H
#pragma config FOSC = HSPLL_HS   // Usa o oscilador HS (High Speed) com o PLL habilitado

// CONFIG2L
#pragma config PWRT = ON         // Power-up Timer habilitado (estabiliza a alimentação na partida)
#pragma config BOR = ON          // Brown-out Reset habilitado (reseta o PIC se a tensão cair)
#pragma config BORV = 3          // Nível de Brown-out Reset em 2.1V
#pragma config VREGEN = ON       // Regulador de tensão interno do USB habilitado (essencial se usar USB)

// CONFIG2H
#pragma config WDT = OFF         // Watchdog Timer DESLIGADO (bom para depuração)

// CONFIG3H
#pragma config MCLRE = ON        // Pino de Master Clear Reset HABILITADO
#pragma config LPT1OSC = OFF     // Timer1 Low-power Oscillator DESLIGADO
#pragma config PBADEN = OFF      // Pinos do PORTB<4:0> configurados como I/O digital na inicialização

// CONFIG4L
#pragma config STVREN = ON       // Reset por estouro de pilha (Stack) HABILITADO
#pragma config LVP = OFF         // Low-Voltage Programming DESLIGADO (essencial para usar RB5 como I/O)
#pragma config ICPRT = OFF       // In-Circuit Debug/Programming Port desabilitado
#pragma config XINST = OFF       // Instruções estendidas (formato C18) DESLIGADO
#pragma config DEBUG = OFF       // Modo de depuração em background DESLIGADO

#define _XTAL_FREQ 48000000UL

// DISPLAYS
#define DISPLAY1_ENABLE LATEbits.LATE0  // Unidade
#define DISPLAY2_ENABLE LATEbits.LATE1  // Dezena
#define DISPLAY3_ENABLE LATAbits.LATE2  // Centena
#define DISPLAY4_ENABLE LATAbits.LATE3  // Milhar
#define DISPLAY_VALUE LATD

// BOTOES
#define INC_BUTTON PORTBbits.INT0
#define DEC_BUTTON PORTBbits.INT1

// BUZZER
#define BUZZER_PIN LATCbits.LATC

const char display_values[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111011, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 
    0b01110111, // A
    0b00011111, // B
    0b01001110, // C
    0b00111101, // D
    0b01001111, // E
    0b01000111, // F
};

unsigned char unidade, dezena, centena, milhar;

void extrair_digitos(unsigned int valor);
void atualizar_displays(void);
void debounce_delay(void);
void delay_us_variable(unsigned int us);
void gerar_tom_buzzer(unsigned int freq_hz);

void main(void){
    ADCON1 = 0x0F; // PORTA/B digital
    CMCON = 0x00;  // Comparadores desligados
    INTCON2bits.RBPU = 0x01; // Pull-ups habilitados

    // Configura pinos
    TRISBbits.TRISB1 = INT1; // DEC_BUTTON
    TRISBbits.TRISB2 = INT0; // INC_BUTTON
    TRISCbits.TRISC2 = LATC; // BUZZER
    // Display
    TRISD = 0x00;
    // Display enable
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA5 = 1;
    TRISEbits.TRISE0 = 1;
    TRISEbits.TRISE2 = 1;

    // Inicializar displays (todos desligados)
    DISPLAY1_ENABLE = 0;
    DISPLAY2_ENABLE = 0;
    DISPLAY3_ENABLE = 0;
    DISPLAY4_ENABLE = 0;
    
    BUZZER_PIN = <CONFIGURAR>;

    unsigned int freq_hz = <CONFIGURAR>; // Frequência inicial
    extrair_digitos(freq_hz);

    while(1){
        // ================= Botoes inc/dec =================
        if(!INC_BUTTON && freq_hz < <CONFIGURAR>){
            freq_hz += <CONFIGURAR>;
            extrair_digitos(freq_hz);
            while(!INC_BUTTON)
                ; // Aguarda soltar o botão
            debounce_delay();
        }
        
        if(!DEC_BUTTON && freq_hz > <CONFIGURAR>){
            freq_hz -= <CONFIGURAR>;
            extrair_digitos(freq_hz);
            while(!DEC_BUTTON)
                ; // Aguarda soltar o botão
            debounce_delay();
        }

        // ================= Gera tom e atualiza display =================
        gerar_tom_buzzer(freq_hz);
        atualizar_displays();
    }
}

// ================= FUNÇÕES =================
void extrair_digitos(unsigned int valor) {
    unidade = valor % <CONFIGURAR>;
    dezena = (valor / <CONFIGURAR>) % <CONFIGURAR>;
    centena = (valor/ <CONFIGURAR>) % <CONFIGURAR>;
    milhar = (valor / <CONFIGURAR>) % <CONFIGURAR>;
}

void atualizar_displays(void) {
    // Display 1 - Unidade
    DISPLAY1_ENABLE = <CONFIGURAR>;
    DISPLAY2_ENABLE = <CONFIGURAR>;
    DISPLAY3_ENABLE = <CONFIGURAR>;
    DISPLAY4_ENABLE = <CONFIGURAR>;
    DISPLAY_VALUE = display_values[unidade];
    DISPLAY1_ENABLE = <CONFIGURAR>;
    __delay_ms(<CONFIGURAR>);
    
    // Display 2 - Dezena
    DISPLAY1_ENABLE = <CONFIGURAR>;
    DISPLAY2_ENABLE = <CONFIGURAR>;
    DISPLAY3_ENABLE = <CONFIGURAR>;
    DISPLAY4_ENABLE = <CONFIGURAR>;
    DISPLAY_VALUE = display_values[dezena];
    DISPLAY2_ENABLE = <CONFIGURAR>;
    __delay_ms(<CONFIGURAR>);
    
    // Display 3 - Centena
    DISPLAY1_ENABLE = <CONFIGURAR>;
    DISPLAY2_ENABLE = <CONFIGURAR>;
    DISPLAY3_ENABLE = <CONFIGURAR>;
    DISPLAY4_ENABLE = <CONFIGURAR>;
    DISPLAY_VALUE = display_values[centena];
    DISPLAY3_ENABLE = <CONFIGURAR>;
    __delay_ms(<CONFIGURAR>);
    
    // Display 4 - Milhar
    DISPLAY1_ENABLE = <CONFIGURAR>;
    DISPLAY2_ENABLE = <CONFIGURAR>;
    DISPLAY3_ENABLE = <CONFIGURAR>;
    DISPLAY4_ENABLE = <CONFIGURAR>;
    DISPLAY_VALUE = display_values[milhar];
    DISPLAY4_ENABLE = <CONFIGURAR>;
    __delay_ms(<CONFIGURAR>);
}

void debounce_delay(void) { 
    for(int i=<CONFIGURAR>; i< <CONFIGURAR>; i++) {
        __delay_ms(<CONFIGURAR>);
    }
}

// Delay em microsegundos usando loop de NOPs já que o XC8 v3.00 não aceita __delay_us() com variaveis
void delay_us_variable(unsigned int us){
    // A 48MHz, cada instrução = ~20.83ns
    // Para 1us precisamos de aproximadamente 48 instruções
    unsigned int cycles = us * <CONFIGURAR>; // Aproximação para 48MHz
    
    while(cycles > <CONFIGURAR>){
        __asm("nop");
        cycles--;
    }
}

// Gera um tom no buzzer por um curto período
void gerar_tom_buzzer(unsigned int freq_hz){
    if(freq_hz == <CONFIGURAR>) {
        BUZZER_PIN = <CONFIGURAR>;
        return;
    }
    
    // Calcula o meio-período em microsegundos
    unsigned long half_period_us = <CONFIGURAR> / (<CONFIGURAR> * (unsigned long)freq_hz);
    
    // Limita o valor para evitar overflow
    if(half_period_us > <CONFIGURAR>) {
        half_period_us = <CONFIGURAR>;
    }
    
    unsigned int half_period = (unsigned int)half_period_us;
    
    // Gera alguns ciclos do tom
    for(int i = <CONFIGURAR>; i < <CONFIGURAR>; i++){
        BUZZER_PIN = <CONFIGURAR>; 
        delay_us_variable(half_period);
        BUZZER_PIN = <CONFIGURAR>;
        delay_us_variable(half_period);
    }
}
