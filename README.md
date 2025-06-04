# ICT-v7-ATGM336

This is based on YO3ICT balloon fw which you can read about here: https://yo3ict.uus.ro/tracker.html

What he did was reverse enginer the QRP-Labs telemetry protocol described here: https://qrp-labs.com/flights/s4#protocol 
most balloons use this encoding today, with a few exceptions, like ELK (have both his own and qrplabs) and Zachtek trackers

Another good site for beginers is to read up here: https://traquito.github.io/ and join the io board at: https://groups.io/g/picoballoon 

The Traquito page will host channel reservationb and upload to varous tracking sites like https://aprs.fi/ and https://amateur.sondehub.org/

## Folders in this repo:  
### ICT_WSPR   
the code for the board  
### Pcb-ICT-main  
contains the cam files for pcb ordering as well as the Eagle schematich and board files  
### Boards.txt-main  
to edit/copy-paste or replace the Boards.txt in arduino to controll the speed of the 328, its located in \arduino-1.8.13\hardware\arduino\avr  
### oscillator-main  
Test file to test the 328 and si5351, no gps is needed for this, only the 328, 5351, its tcxo and the two pull up resistors is needed, if all works it will tx a carrier at 14.097Mhz




This setup is aimed at three main things: light weight, low power demands and as few components as possible - easy to build

The board can be built to weigh below 1 gram fully assambled (pic below) + solar cells & antenna

<img src="https://github.com/user-attachments/assets/6a8e9051-83a9-4787-aebc-15e3fc69320a" width="220" />

To make the board consume as little power as possible its setup to switch between tx and gps so only one of the two is ON at any time, so it will startup and get valid gps lock with tx side of the board switched off, the tx ic - si5351 consumes 17-27mA (depending on model) in stb curent even when not in use so only the gps is on and draws about 25mA using the ATGM336 (N31), when the board have got a valid lock the gps will turn off and the wspr tx starts, using the low power 76, see further down, it will consume about 20mA and at max power, 79, it will be about 30mA, on top of this the 328 running at 2Mhz and the Tcxo combined will take another 3mA 

The board will work on any band between 80m and 2m but most used band is 20m-10m

# GPS:

The ATGM336-N31 support both GPS and Beidou system, by switching off Beidou and run the ATGM in GPS only mode we save another 4mA, see .ino file for the command.

# 328 Fuses

Setts the expected external or internal xo used and also set the brownout to 1,8V

The 328 have an internal xo at 8Mhz, you can set it to be divided down by 8 (fuse) and then the board runs at 1Mhz
the internal xo at 8Mhz is not very stable in cold and it will not work at flying altitude, but I have tried a few flights on the internal xo divided down to 1Mhz 
and it have worked fine, anyway, both speeeds works fine for testing a board on the bench.

62 D9 FE 1.8V / internal /div8 @ 1Mhz

E2 D9 FE 1.8V / internal 8Mhz osc

FD D9 FE 1,8V / External 2/4/8Mhz  xo

Good fusecalc page https://www.engbedded.com/fusecalc/

# BOM

Cpu Atmega328

Gps ATGM336 (N31) others will work

TX Si5351

Tcxo 25/26/27 Mhz for si5351, size 3225

Xo 4Mhz to drive 328, we will then use a DIV function to set 1/2 or 1/4 the speed to save some power

2x 10K resistor size 0603, value non critical, 1K-10K works

3x 100nF Capacitor size 0603, value non critical 10-470Nf works, it will work fine without any soldered on but I recomend at least one

There is also a pad for a bleeed resistor to even out possible static buildup between tx leg and gnd
not needed but if you want to fit it use a 0603 resistor around 30K

# Antennas

Gps 46mm
Wire length/band , thicknes, suport wire - fishingline

# Solar cells

6psc , sizes, 4 cells and voltage doubler /ldo

# Arduino

I use Arduino version 1.8.13 which you find here, others will work but not tested, https://www.arduino.cc/en/software/OldSoftwareReleases/

Libraries needed

Ad custom parameters to the Boards.txt in \arduino-1.8.13\hardware\arduino\avr

Course Altitude in standard tx

# Setup parameters in the code

Timing4.h is a testfile that will start tx every 2min

# PCB 

I have always used Jlcpcb at https://jlcpcb.com/ for my balloon, and other pcb:s

The pcb files have been made with Eagle pcb nowdays under the name Autodesk Eagle https://www.autodesk.com/products/eagle/overview

Use as thin pcb as possible to save weight - 0,6mm

In Pcb-ICT-main folder in this repo you will find needed files, if you are a experieced with solder small parts and have a hot air solderstation go with the Xtra Smal QFN version of the board, for a more simple soldering setup use the TQFP version

![TQFP](https://github.com/user-attachments/assets/396c5c7e-c1b4-4560-8005-94a8b082ddce) 
<img src="https://github.com/user-attachments/assets/0c05cefa-c216-4d57-8de8-9f9ed1676eac" width="100" />







