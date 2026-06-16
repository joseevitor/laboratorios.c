/*
 * File:   main.c
 * Author: Jose Vitor Miranda
 *
 * Created on 26 October 2026, 09:48am
 */

#include <pic18f4550.h>
#include <xc.h>
#include <delays.h>

// CONFIG1H
#pragma config FOSC = HS // Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
// CONFIG3H
#pragma config CCP2MX = OFF      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT enabled)
// CONFIG4L
#pragma config STVREN = OFF      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config DEBUG = ON

int main(void) {
    DDRD = 0x00;
    for(;;){
    PORTD = 0x55;
        
        Delay10KTCYx(200); // Manter comentado na simulação e depuração ? na gravação pode retirar o //
    PORTD = 0xFF;
        Delay10KTCYx(200); // Manter comentado na simulação e depuração ? na gravação pode retirar o //
    }
}
