# ICT-v7-ATGM336

This setup is aimed at three main things: light weight, low power demands and as few components as possible - easy to build

The board can be built to weigh below 1 gram fully assambled (pic below) + solar cells & antenna

<img src="https://github.com/user-attachments/assets/6a8e9051-83a9-4787-aebc-15e3fc69320a" width="200" />

To make the board consume as little power as possible its setup to switch between tx and gps so only one of the two is ON at any time, so it will startup and get valid gps lock with tx side of the board switched off, the si5351 consumes 17-27mA (depending on model) in stb curent even when not in use so only the gps is on and draws about 25mA using the ATGM336 (N31), when the board have got a valid lock the gps will turn off and the tx starts, using the low power 76, see further down, it will consume about 20mA and at max power, 79, it will be about 30mA, on top of this the 328 running at 2Mhz and the Tcxo combined will take another 3mA 

GPS:

The ATGM336-N31 support both GPS and Beidou system, by switching off Beidou and run the ATGM in GPS only mode we save another 4mA, see .ino file for the command.

Timing4.h is a testfile that will start tx every 2min

#Fuses to be set in the 328

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

3x 100nF Capacitor size 0603, value non critical 10-470Nf works, it will work fine without any soldered on but I recomend at least one

# Antennas

Gps 46mm
Wire length/band , thicknes

# Solar cells
6psc , sizes, 4 cells and voltage doubler /ldo

# Arduino
version, libraris needed

# Setup parameters in the code






