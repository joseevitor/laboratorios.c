#include <xc.h>
#include <pic18f4550.h>


// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (Divide by 5 (20 MHz oscillator input))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 2       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes from the 96 MHz PLL divided by 2)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = ON      // USB Voltage Regulator Enable bit (USB voltage regulator enabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
#pragma config DEBUG = OFF  // Modo de depuração em background DESLIGADO

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>



#define _XTAL_FREQ OSC1_PLL2   // Definição da frequencia do clock para as funcoes de delay.
                                  // ATENCAO - ESTE VALOR PRECISA SER IGUAL AO CLOCK DA CPU DEFINIDO NOS 
								  // BITS DE CONFIGURAÇÃO
// CRIAR CABECALHO DAS FUNCOES
unsigned int reader(void); // FUNCAO DEVE LER ADC
void delay_50ms(void);         // FUNCAO DEVE SER UM DELAY DE 50ms

void main(void) {
	// FAZER A BUSCA NO DATASHEET
	
    // Buscar na Configuração do Módulo A/D (ADC)
	// OS REGISTRADORES ABAIXO DEVEM CONFIGURAR 
	// O MODO DE OPERACAO DO ADC
	
    // Voltage Reference Configuration Bits (00 = Vref- = VSS, Vref+ = VDD)
    // A/D Port Configuration Control Bits (1110 = Only AN0 as analog, outros digitais)
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    // A/D Result Format Select Bit (1 = Right justified) - ADFM
    // A/D Acquisition Time Select Bits (010 = 4 Tad) - ACQT
    // A/D Conversion Clock Select Bits (110 = Fosc/64) - ADCS
    ADCON2 = 1; // 1 (ADFM) | 010 (ACQT) | 110 (ADCS)
    // Channel Select Bits (0000 = Channel 0 (AN0)) - CHS
    // Habilita o módulo A/D (1 = On) - ADON
    ADON = 1; // AN0 selecionado e A/D habilitado (ADON = 1)


    // Configuração de I/O
    // Habilita Pull-Up para o PORTB
    PORTB = 1;

    // Configura todos os pinos do PORTD como output (saída)
    PORTD = 0x00;
    // Inicializa o PORTD como nível alto
    PORTD = 0xFF; // Todos os LEDs desligados

    unsigned int ad_value;
    unsigned char n_leds_on;
    unsigned char led_mask;

    while(1){
        // Leitura do ADC
        ad_value = reader();

        // 1. Lógica de Divisão (n_leds_on será 0 a 7)
        n_leds_on = ad_value >> 7; 

        // Isso garante que o n_leds_on se torne 8, ligando o último LED.
        if (ad_value == 1023) { 
            n_leds_on = 8;
        }

        // 2-3. Cria a máscara e aciona os LEDs
        led_mask = (0x01 << n_leds_on) - 1;
        
        LATD = ~led_mask;

        delay_50ms();
    }
}

// AQUI EH NECESARIO CRIAR OS CABECALHOS
// --- Funcoes Auxiliares ---
unsigned int reader(void){

    // 1. Inicia a conversão (Sinalizar conversão GO/DONE)
    ADCON0bits.GO_DONE = 1;

    // 2. Espera a conversão terminar
    while(ADCON0bits.GO_DONE);

    // 3. Retorna o resultado
    // Como ADCON2bits.ADFM = 1 (Right Justified), o resultado em ADRESH:ADRESL.
    // Registrador de 16 bits ADRES que faz a união automaticamente.
    return ADRES;
    // Se não usasse ADRES (forma manual):
    // return (ADRESH << 8) + ADRESL; // Desloca ADRESH 8 bits para a esquerda e soma com ADRESL
};

void <FUNCAO DE DELAY 50ms>(void){
    for(int i = 0; i < 5; i++){
        <BUSCAR NA BIBLIOTECA DELAY - DELAY DE 10MS>(10); // 5 x 10ms = 50ms
    }
}
