# ICT-v7-ATGM336
<img src="https://github.com/user-attachments/assets/87f7522f-9779-4d59-95e0-92f3e82d1d06" width="200" />

This is based on YO3ICT balloon fw which you can read about here: https://yo3ict.uus.ro/tracker.html

What Eduard did was reverse enginer the QRP-Labs telemetry protocol which used wspr to get data through, and open source it, details of the protocol is described here: https://qrp-labs.com/flights/s4#protocol 
most balloons use this encoding today, with a few exceptions, like Zachtek trackers and a few more

Another good site for beginers is to read up here: https://traquito.github.io/ and join the io board at: https://groups.io/g/picoballoon 

The Traquito page will host channel reservation and upload to varous tracking sites like https://aprs.fi/ and https://amateur.sondehub.org/ and as well host its own map, more maps and info avalible here https://lu7aa.org/dx.asp and here http://qrp-labs.com/tracking.html

## Folders in this repo:  
### ICT_WSPR   
The code to upload to the board  
### Libraries
Copy the content in this folder into libraries folder of Arduino C:\Program Files (x86)\Arduino\libraries  
### Pcb-ICT-main  
Cam files for pcb ordering as well as the Eagle schematic and board files, viewer for both files here: https://www.altium365.com/viewer/
### Boards.txt-main  
Replace the Boards.txt in arduino to controll the speed of the 328, its located in: C:\Program Files (x86)\Arduino\hardware\arduino\avr  
### oscillator-main  
Test file to test the 328 and si5351, no gps is needed for this, only the 328, 5351, its tcxo and the two pull up resistors needed to be soldered, if all works it will tx a carrier at 14.097Mhz, upload the .ino ffile with arduino or the .hex with Avrdudes - see below

## Goals  
This setup is aimed at three main things: light weight, low power demands and as few components as possible - easy to build

The board can be built to weigh below 1 gram fully assembled (pic below) with solar cells & antenna connected total weight 4-5 gram

<img src="https://github.com/user-attachments/assets/6a8e9051-83a9-4787-aebc-15e3fc69320a" width="220" />

To make the board consume as little power as possible its setup to switch between tx and gps so only one of the two is ON at any time, so it will startup and get valid gps lock with tx side of the board switched off, the tx ic - si5351 consumes 17-27mA (depending on model) in stb curent even when not in use so only the gps is on and draws about 25mA using the ATGM336 (N31), when the board have got a valid lock the gps will turn off and the wspr tx starts, using the low power 76, see further down, it will consume about 20mA in low and at max power, 79, it will be about 30mA, on top of this the 328 running at 2Mhz and the Tcxo combined will take another 3mA 

The board will work on any band between 80m and 2m but most used band is 20m-10m

# Features  
CW init  
Randomization  
No tx over selected grids - move tx to ism freq over selected grids  
Powerswitching between gps and tx  
Course altitude in main wspr  
Slowly increased power in 4 steps  
Good WDT function of 328  
Will work at low clocks, tested down to 1Mhz on the 328  
Powersave of gps by set it to single naviagtion system  - see Gps section

# GPS:

The ATGM336-N31 support both GPS and Beidou system, by switching off Beidou and run the ATGM in GPS only mode we save another 4mA, see .ino file for the command, its activated by default.  
This gps does not have any altitude roof like many other modules, its been tested to 150.000m with a hack-rf and GPSSim and still worked fine

# 328 Fuses

Setts the expected external or internal xo used and also set the brownout to 1,8V

The 328 have an internal xo at 8Mhz, you can set it to be divided down by 8 (fuse) and then the board runs at 1Mhz
the internal xo at 8Mhz is not very stable in cold and it will not work at flying altitude, but I have tried a few flights on the internal xo divided down to 1Mhz 
and it have worked fine, anyway, both speeeds works fine for testing a board on the bench.  

FD D9 FE 1,8V / External 2/4/8Mhz xo  - This is the fuse you should use if you build this board  

62 D9 FE 1.8V / internal 8Mhz osc /div8 = 1Mhz  
E2 D9 FE 1.8V / internal 8Mhz osc  

To set fuses use Avrdude  https://github.com/avrdudes/avrdudes

Fusecalc page https://www.engbedded.com/fusecalc/

I would recommend to buy a couple of arduino nano boards to practice on before flashing fuses onto a built board as this is the main culprit of non responsive 328 - wrongly set or corupted fuses

# BOM

Cpu Atmega328

Gps ATGM336 (N31) others will work

TX Si5351

Tcxo 25/26/27 Mhz size 3225 3.2x2.5mm

Xo 4Mhz to drive 328 size 4520 4.5mm×2mm  

2x 10K resistor size 0603, value non critical, 1K-10K works

3x 100nF Capacitor size 0603, value non critical 10-470Nf works, it will work fine without any soldered on but I recomend at least one

There is also a pad for a bleeed resistor to even out possible static buildup between tx leg and gnd
not needed but if you want to fit it use a 0603 resistor around 30K

2.54mm x2 Pin Header 

# Antennas

Gps 46mm
Wire length/band , thicknes, suport wire - fishingline

# Solar cells

6psc , sizes, 4 cells and voltage doubler /ldo

# Arduino

Recomended to use Arduino version 1.8.13 which you find here, others will work but not tested, https://www.arduino.cc/en/software/OldSoftwareReleases/

# PCB 

I have always used Jlcpcb at https://jlcpcb.com/ for my balloon boards and other projects

The pcb files have been made with Eagle pcb nowdays under the name Autodesk Eagle https://www.autodesk.com/products/eagle/overview

Use as thin pcb as possible to save weight - 0,6mm

In Pcb-ICT-main folder in this repo you will find needed files, if you are a experieced with solder small parts and have a hot air solderstation go with the Xtra Smal QFN version of the board, for a more simple soldering setup use the TQFP version

![TQFP](https://github.com/user-attachments/assets/396c5c7e-c1b4-4560-8005-94a8b082ddce) 
<img src="https://github.com/user-attachments/assets/0c05cefa-c216-4d57-8de8-9f9ed1676eac" width="100" />


# Programming

The pcb is fited with a 6 pin ICSP/ICP connector  
The Arduino uploads the fw with an USBasp programmer, get a 3,3v version with a 6 pin header adapter on the end of the ribbon cable  
To upload a .hex file like the one supplied in oscillator-main folder I recommend using Avrdudes https://github.com/avrdudes/avrdude

<img src="https://github.com/user-attachments/assets/268ecf5b-a8f3-45db-a4a2-e6bc72a3cb40" width="200" />  <br>  

In Arduino IDE select Usbasp as programmer:    
<img src="https://github.com/user-attachments/assets/8b709942-572e-46ef-bb08-8fecef93d394" width="400" />  <br>

In Boards select rAVA 2Mhz:    
<img src="https://github.com/user-attachments/assets/9a1b4d58-5292-4c4c-ae64-b206f18510b0" width="400" />  <br>


# Schematic

<img src="https://github.com/user-attachments/assets/d44a3582-5e09-4798-9103-74a7370bb043" width="200" />

# Setup parameters in the code  
The Arduino code is setup from start to use a 26Mhz Tcxo and to use a 4Mhz xo divided by 2 to run at 2Mhz, if other values used the code needs to be changed - see below  

Set you call in the ino file by edit this: const char call[] = "SA9BSS";  


Set initial power of the 5351: int pwr_level=79;  
it will for every sequnce increase the power one step so if you set it at 77 it will take two sequencies, 20 min before it will start at full power  
avalible valus are 76,77,78 and 79 where 79 is max power from the tx, 76 is just a few mW so I recomend not to ue that.  


Timing4.h is a testfile that will start tx every 2min, this it convinient to use during testing on the bench, final prep before flight is to edit the  
.ino file from #include "Timing4.h" to #include "Timing.h"

Timing.h is the main config that should be used during a flight, it have a 10 min sequence as needed to fit the scheduling agreed on in the community

we will use a DIV function to set 1/2 or 1/4 the speed to save some power  






