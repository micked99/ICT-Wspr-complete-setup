# ICT-v7-ATGM336

This setup is aimed at three main things: light weight, low power and as few components - easy to build

The board can be built to weigh below 1 Gram fully assambled (pic) (plus solar & antenna)

![0,94g](https://github.com/user-attachments/assets/6a8e9051-83a9-4787-aebc-15e3fc69320a){width=300 height=200}



Ads support for WDT and switching on GPS only mode on the gps for powersaving

Ads support for WDT - Watch Dog Timer to prevent the program running in the 328 to freeze or lock up.          
Switching on GPS only mode on the gps for powersaving (Beidou Chinese Gps system off), saves about 4-5mA 

Timing4.h is a testfile that will start tx every 2min

Fuses to set

62 D9 FE 1.8V / internal /div8 @ 1Mhz

E2 D9 FE 1.8V / internal 8Mhz osc

or this one if you use a 2/4/8Mhz external xo
FD D9 FE 

Good fusecalc page https://www.engbedded.com/fusecalc/

# BOM
Cpu Atmega328

Gps ATGM336 (N31) other will work

TX Si5351

Tcxo 25/26/27 Mhz for si5351, size 3225

Xo 4Mhz to drive 328

2x 10K resistor size 0603, value non critical, 1K-10K works

3x 100nF Capacitor size 0603, value non critical


