/* 
 * File:   Infrakom.c
 * Author: student
 *
 * Created on 27. duben 2015, 10:19
 *
 * IR_TX PortB5
 * IR_RX PortC0
 * Bargraf napájení C1 aktivuj log 0
 * segmenty bargrafu portD aktivní v log 0
 * tlacitka portd stisknutí je log 0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


#pragma config WDTE = OFF, PWRTE = OFF, CP = OFF, BOREN = OFF, DEBUG = OFF, LVP = ON, CPD = OFF, WRT = OFF, FOSC = XT


#define IR_TX RB5
#define IR_RX RC0

/*
 * 
 */

unsigned char tlacitka;

void odesli(){
    unsigned char i;
    //vynulujeme timer2
    TMR0IF=0;
     TMR0=TMR0+233;

    CCPR1L=7;
   // IR_TX =1;
    while(TMR0IF==0);
    TMR0IF=0;
     TMR0=TMR0+233;
    for(i=0;i<=7;i++){

        //IR_TX=tlacitka&i;//bitová maska
        if(tlacitka&(1<<i))CCPR1L=7;
        else CCPR1L=0;
    }
       CCPR1L=0;
    while(TMR0IF==0);
    TMR0IF=0;
     TMR0=TMR0+233;
    }

void prijmi(){
        unsigned char i;
        while (IR_RX==1);
    TMR0IF=0;
     TMR0=TMR0+233;
    while(TMR0IF==0);
    TMR0IF=0;
     TMR0=TMR0+233;

tlacitka=0;
for(i=0;i<=7;i++){

        //IR_TX=tlacitka&i;//bitová maska
        if(IR_RX==1) tlacitka=tlacitka | (1<<i);

        while(TMR0IF==0);
    TMR0IF=0;
     TMR0=TMR0+233;

    }

    while(TMR0IF==0);
    TMR0IF=0;
     TMR0=TMR0+233;

}



void precti(){
    //vypneme napájení bargrafu-portc.1 zapí?eme 1
    RC1=1;
  //  RC0=1;
    //prepnout portd do vstupního re?imu
    TRISD=0xff;
    //pockame 2xNOP
    NOP();
    NOP();
    tlacitka=PORTD;
    //prepneme

}
void zobraz(){
    PORTD=tlacitka;
    //prepneme portd do výstupního re?imu
    TRISD=0;
    //zapneme napájení bargrafu-portc.1 zapí?eme 0
    RC1=0;

}




void mcu_init(){
   //tmr2 t=27,7 us Fosc=3276800Hz Nic=4
        //T2CON = TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
        //TMR2ON=1;   //zapneme
        //PR2 = 256 - 23; //nastaveni HW zkraceni rozsahu //povolovat preruseni od TMR2 nebudeme //PUVODNI VERZE BEZ PWM

        PR2=22;             //nova verze s PWM  //PWM Period = [(PR2) + 1] ? 4 ? TOSC ? (TMR2 Prescale Value)
        CCPR1L = 0;         //vypneme generovani 7 //strida 30%
        TRISC2 = 1;       //prepenem TRISC[2] do vystupniho rezimu
        TMR2ON = 1;  //zapneme TMR2 = T2CON[TMR2ON]=1
        CCP1M3=CCP1M2 = 1;// PWM rezim pro CCP1

        //pocitani prislych pulzu pomoci timeru0
        PS2 = 0;    //nastavime TMR0 na preddelic 1:16, vstup pulzu interni, perioda 256-23=233
        T0CS = 0;
        PSA = 0;    //prideleni preddelice
        TMR0 = TMR0+233;


       RC1 = 1;//nastavit na 1 portC<1>
      //  RB5 = 1;//nastavit na 1 portB<5>
        PORTD = 0xFF;//nastavit na OxFF portD

       //TRISB5 = 1;//prepneme do vystupniho rezimu portB<5>, portC<1>
        TRISC1 = 0;//prepneme do vystupniho rezimu  portC<1>
        TRISC2 = 0;//prepneme do vystupniho rezimu  portC<1>

}








void main(void) {
//vysíla?
    //init   konfigurace port? a ?íta?e
    mcu_init();

    while(1){
      //  precti();
        //odesli();

        prijmi();
        zobraz();
                    }


    //hlavní smy¨?ka
    //vysíla?-p?e?ti,ode?li
    //p?ijíma? - p?ijmi, zobraz







    //return (EXIT_SUCCESS);
}

