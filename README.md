# ICT-v7-ATGM336

This is based on YO3ICT balloon fw which you can read about here: https://yo3ict.uus.ro/tracker.html

what he did was reverse enginer the QRP-Labs telemetry protocol described here: https://qrp-labs.com/flights/s4#protocol

Another good site for beginers is to read up here: https://traquito.github.io/ and join the io board at: https://groups.io/g/picoballoon 

the Traquito page will host channel reservationand, upload to varous tracking sites like aprs.fi and https://amateur.sondehub.org/

This setup is aimed at three main things: light weight, low power demands and as few components as possible - easy to build

The board can be built to weigh below 1 gram fully assambled (pic below) + solar cells & antenna

<img src="https://github.com/user-attachments/assets/6a8e9051-83a9-4787-aebc-15e3fc69320a" width="220" />

To make the board consume as little power as possible its setup to switch between tx and gps so only one of the two is ON at any time, so it will startup and get valid gps lock with tx side of the board switched off, the tx ic - si5351 consumes 17-27mA (depending on model) in stb curent even when not in use so only the gps is on and draws about 25mA using the ATGM336 (N31), when the board have got a valid lock the gps will turn off and the wspr tx starts, using the low power 76, see further down, it will consume about 20mA and at max power, 79, it will be about 30mA, on top of this the 328 running at 2Mhz and the Tcxo combined will take another 3mA 

The board will work on any band between 80m and 2m but most used band is 20m-10m

# GPS:

The ATGM336-N31 support both GPS and Beidou system, by switching off Beidou and run the ATGM in GPS only mode we save another 4mA, see .ino file for the command.

# Fuses to be set in the 328

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

Xo 4Mhz to drive 328

2x 10K resistor size 0603, value non critical, 1K-10K works

3x 100nF Capacitor size 0603, value non critical 10-470Nf works, it will work fine without any soldered on but I recomend at least one

Thereis also a pad for a bleeed resitor to even out static buildup between gnd and the tx leg, 
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

# Setup parameters in the code

Timing4.h is a testfile that will start tx every 2min






