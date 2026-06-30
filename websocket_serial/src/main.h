/*
 * File:   main.h<br/>
 * Author: user
 *
 * Created on 23 de Fevereiro de 2026, 12:29
 */

#ifndef _MAIN_H_
#define	_MAIN_H_

#include <xc.h> // Inclui o cabeçalho específico do PIC

// --- Configuração de Fuses ---
// Usando cristal de 20MHz para obter Fosc = 48MHz e Fcy = 12MHz
// Fosc = 48MHz (para USB e alta performance)
// Fcy = Fosc / 4 = 12MHz (frequência de instrução)

#pragma config PLLDIV = 5       // Divide 20MHz por 5 para 4MHz (entrada do PLL)
#pragma config CPUDIV = OSC1_PLL2 // Divide a saída do PLL (96MHz) por 2 para 48MHz (Fosc)
#pragma config USBDIV = 2       // USB Clock = PLL/2 = 48MHz
#pragma config FOSC = HSPLL_HS     // Oscilador HS com PLL habilitado
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor desabilitado
#pragma config IESO = OFF       // Oscilador Switchover desabilitado
#pragma config PWRT = OFF       // Power-up Timer desabilitado
#pragma config BOR = ON         // Brown-out Reset habilitado
#pragma config BORV = 3         // Brown-out Reset em 2.05V
#pragma config VREGEN = OFF     // USB Voltage Regulator desabilitado (se não usar USB)
#pragma config WDT = OFF        // Watchdog Timer desabilitado
#pragma config WDTPS = 32768    // WDT Postscaler (irrelevante se WDT=OFF)
#pragma config CCP2MX = OFF     // CCP2 em RC1 (se não usar CCP2)
#pragma config PBADEN = OFF     // PORTB<4:0> como digital (desabilita analógico)
#pragma config LPT1OSC = OFF    // Timer1 oscilador desabilitado
#pragma config MCLRE = ON       // MCLR habilitado
#pragma config STVREN = ON      // Stack Overflow Reset habilitado
#pragma config LVP = OFF        // Low-Voltage Programming desabilitado
#pragma config XINST = OFF      // Extended Instruction Set desabilitado
#pragma config CP0 = OFF        // Code Protection desabilitado
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config WRT0 = OFF       // Write Protection desabilitado
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF
#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config EBTR0 = OFF      // Table Read Protection desabilitado
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF

// --- Definições Globais ---
#define _XTAL_FREQ 48000000UL   // Frequência do cristal externo (20 MHz)
#define FOSC_HZ    48000000UL   // Frequência do oscilador do sistema (48 MHz)
#define FCY_HZ     12000000UL   // Frequência de instrução (Fosc / 4 = 12 MHz)
#define RX_BUFFER_SIZE 64       // Tamanho do buffer circular de recepção (potência de 2)

#define FOSC_HZ     48000000UL
#define BAUD_RATE   9600UL

// --- Protótipos de Funções ---
void init_system(void);
void init_interrupts(void);
void send_char(unsigned char c);
void send_string(const char *str);
void buffer_put(unsigned char data);
unsigned char buffer_get(unsigned char *data);

#endif