# ICT-v7-ATGM336
<img src="https://github.com/user-attachments/assets/87f7522f-9779-4d59-95e0-92f3e82d1d06" width="400" />  <br> <br>
## Updates
BOM and Pick and place files adaded if you want to for example JLCPCB to make and assamble the complete board <br>
BOM_ict_v361-xs.csv <br>
CPL_pick place_ict_v361-xs.xlsx <br>
Screenshots to see order seq of JLCPcb complete board assembly, ask JLCpcb if you have ordering problems :)



## Introduction
This FW is to 97% the work of YO3ICT balloon fw which you can read about here: https://yo3ict.uus.ro/tracker.html

What Eduard did was reverse enginer the QRP-Labs telemetry protocol which use wspr to get data through, and open sourced it, details of the protocol is described here: https://qrp-labs.com/flights/s4#protocol 
most balloons use this encoding today, with a few exceptions, like Zachtek trackers and a few more

Another good site for beginers is to read up and join the io board at: https://groups.io/g/picoballoon 

The [Traquito](https://traquito.github.io/)  page will host channel reservation and upload to varous tracking sites like https://aprs.fi/ and https://amateur.sondehub.org/ and as well host its own map, more maps and info avalible here https://lu7aa.org/dx.asp and here http://qrp-labs.com/tracking.html, another good page that poped up lately is [wsprtv](https://wsprtv.com/docs/user_guide.html#u4b-channel-map) & [wsprtv](https://wsprtv.com/tools/channel_map.html?band=17m&num_days=30&rx_threshold=1)

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
Test file to test the 328 and si5351, no gps is needed for this, only the 328, 5351, its tcxo and the two pull up resistors have to be soldered, if all works it will tx a carrier at 14.097Mhz, upload the .ino file with arduino or the .hex with Avrdudess https://github.com/ZakKemble/AVRDUDESS

## Goals  
This setup is aimed at three main things: light weight, low power demands and as few components as possible - easy to build

The board can be built to weigh below 1 gram fully assembled (pic below) with solar cells & antenna connected total weight 4-5 gram

<img src="https://github.com/user-attachments/assets/6a8e9051-83a9-4787-aebc-15e3fc69320a" width="220" />
<img src="https://github.com/user-attachments/assets/16894bf4-589b-4a33-bbca-fa34b94adde3" width="220" />



To make the board consume as little power as possible its setup to switch between tx and gps so only one of the two is ON at any time, so it will startup and get valid gps lock with tx side of the board switched off, the tx ic - si5351 consumes 17-27mA (depending on model) in stb curent even when not in use so only the gps is on and draws about 25mA using the ATGM336 (N31), when the board have got a valid lock the gps will turn off and the wspr tx starts, using the low power 76, see further down, it will consume about 20mA in low and at max power, 79, it will be about 30mA, on top of this the 328 running at 2Mhz and the Tcxo combined will take another 3mA 

The board will work on any band between 80m and 2m but most used band is 20m-10m

# Features  
CW init - At power on, when sun hits the solar panels it start to tx cw, then you know you are good for launch  
Randomization - Randomize the tx by ca 5 hz each tx, have been turned off by deafult, enable in Timing.h  
No tx over selected grids - Move tx to ism (13.555) freq over selected grids  
Powerswitching between gps and tx - Will put less stress on the cells and get earlier in the morning startups  
Course altitude in main wspr - Will insert a rough altitude into the standard wspr db field, see TelemFunctions.h  
Slowly increased power in 4 steps - Default setting is 79 - Max power  
Good WDT function of 328 - Will never fail to wake up even with unstable solar cells powering the board  
Will work at low clocks, tested down to 1Mhz on the 328 - Deafult is 2Mhz  
Powersave of gps by set it to single navigational system  - See Gps section  

# GPS:

The ATGM336-N31 support both GPS and Beidou system, by switching off Beidou and run the ATGM in GPS only mode we save about 4mA, see .ino file for the command, gps only is activated by default.  
This gps does not have any altitude limit like many other modules, its been tested to 150.000m with a hack-rf and GPSSim and still worked fine

# 328 Fuses

Sets the expected external or internal xo used and also set the brownout to 1,8V

The 328 have an internal xo at 8Mhz, you can set it to be divided down by 8 (fuse) and then the board runs at 1Mhz
the internal xo at 8Mhz is not very stable in cold and it will not work at flying altitude, but I have tried a few flights on the internal xo divided down to 1Mhz 
and it have worked fine, anyway, both speeeds works fine for testing a board on the bench.  

FD D9 FE 1,8V / External 2/4/8Mhz xo  - This is the fuse you should use if you build this board  

62 D9 FE 1.8V / internal 8Mhz osc /div8 = 1Mhz  
E2 D9 FE 1.8V / internal 8Mhz osc  

To set fuses use Avrdudess https://github.com/ZakKemble/AVRDUDESS

Fusecalc page https://www.engbedded.com/fusecalc/

I would recommend to buy a couple of arduino nano boards to practice on before flashing fuses onto a built board, as this is the main culprit of non responsive 328 - wrongly set or corrupted fuses

# BOM

All components but the si5351 is bought on Aliexpress, you can source the SI535/MS5351 on Ali but they can have an higher power consumption then the original, from my experience the Ali ones draw 27mA instead of 17mA in stb current  
----------------  
Cpu ATmega328

Gps ATGM336 (N31) others will work  

TX Si5351  

Tcxo 25/26/27 Mhz size 3225 3.2x2.5mm  

Xo 4Mhz size 4520 4.5mm×2mm   

2x 10K resistor size 0603, value non critical, 1K-10K works  

2x 100nF Capacitor size 0603, value non critical 10-470nF works, it will work fine without any soldered on but I recommend at least one  

2.54mm x2 Pin Header - Remove before flight by cutting it of with a scissor  

# Antennas

For gps antenna use a 46mm wire which represents a 1/4 antenna at 1575Mhz  

For antenna I use 0,15mm which is a good combination of strength and weigh but any wire in the 0,1 - 0,2mm will work fine.  
I use a small hook 2cm long made of 0,6mm wire bottom, and 0,8mm at the top to solder on the antenna wire, solder and twist a couple of turns arround the hook  
<img src="https://github.com/user-attachments/assets/4242b577-3dce-488c-8667-69651c81a780" width="220" />  <br>  
Here is good picture where you can see the Red and Yellow gps antennas  
<img src="https://github.com/user-attachments/assets/a84f1ce0-c35d-4ae1-815b-1d40e82dd7b6" width="220" />  <br>  

Tx Antenna length calculated as normal, 300/freq x 0.95 /4

Upper antenna leg should be supported by some sort of fishing line taped to the balloon

# Solar cells

6 psc of solar cells connected in series will work fine, or 4 psc of a bit larger cells and a voltage doubler with 3,3V ldo like REG710 will do the work, ready made pcb are avalible on Aliexpress, search for "Low Noise Regulated Charge Pump" see below pic  
<img src="https://github.com/user-attachments/assets/20d9d26f-9d26-46e9-9770-1600b79adca6" width="220" />  <br> 

Do not try use converters with an inductor, they will not work, only converters using a capacitor are likeley to succeed.  

As can be seeen in the below picture the solar cells is intended to be soldered to the board with a center copper rod of 0,8mm, notice,  
there are two pairs of vias, one is GND and one pair is not connected to anything which is where you solder the center support for the cells - see below pic  
<img src="https://github.com/user-attachments/assets/2246c5c4-9fce-4bfc-a279-e8053c7010ab" width="220" />  <br>  
OBS! The programming pinhead should be cutoff before flight and preferably before you solder the cells onto the pcb  

![solderpoints](https://github.com/user-attachments/assets/892893e5-67a8-44e1-9371-487d653f814b) <br> 
Solderpoints for the solar cells  

With 6 x 38x19 cells ounded flat you can see a startup of the board at about 13° solar angle 

How I solder the cells together, I use a 0,6mm wire I extract from a multi stranded electrical cable, and then a very thin wire from an even smaller multi stranded cable to solder the cells to the pcb, pics:

<img src="https://github.com/user-attachments/assets/032f98bb-db6c-457d-9fd2-6ab59c9f318a" width="300" />  <br>  
<img src="https://github.com/user-attachments/assets/1f709e82-cc4f-4c87-bf25-c44f5ea02478" width="300" />  <br>  

To test the solar + board indoor before a flight you can use a Halogen flood light like below pic, I use a 400W, 8545 lumen lamp.  
-OBS- Led lamps wont work.


<img src="https://github.com/user-attachments/assets/c46cecd5-28c3-4f24-a544-17eb5ec61cbf" width="300" />  <br>  


# Arduino

To compile and upload the sketch I use Arduino version 1.8.13 which you find here, others will work but not tested, https://www.arduino.cc/en/software/OldSoftwareReleases/

# PCB  

<img src="https://github.com/user-attachments/assets/d3663667-622f-49d1-af40-d2abe115b113" width="300" />  <br>  

I have always used Jlcpcb at https://jlcpcb.com/ for my balloon boards and other projects

The pcb files have been made with Eagle pcb nowadays under the name Autodesk Eagle https://www.autodesk.com/products/eagle/overview

Use as thin pcb as possible to save weight - 0,6mm

In Pcb-ICT-main folder in this repo you will find needed files, if you are a experieced with solder small parts and have a hot air solderstation go with the Xtra Smal QFN version of the board, for a more simple soldering setup use the TQFP version

![TQFP](https://github.com/user-attachments/assets/396c5c7e-c1b4-4560-8005-94a8b082ddce) 
<img src="https://github.com/user-attachments/assets/0c05cefa-c216-4d57-8de8-9f9ed1676eac" width="100" />


# Programming

The pcb is fited with a 6 pin ICSP/ICP connector  
The Arduino uploads the fw with an USBasp programmer, get a 3,3v version with a 10 to 6 pin header adapter on the end of the ribbon cable  
To upload a .hex file like the one supplied in oscillator-main folder I recommend using Avrdudess  
https://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/  


Use libsusbK in Zadig to install driver for the Usbasp: https://zadig.akeo.ie/

<img src="https://github.com/user-attachments/assets/268ecf5b-a8f3-45db-a4a2-e6bc72a3cb40" width="200" />  <br>  

In Arduino IDE select Usbasp as programmer:    
<img src="https://github.com/user-attachments/assets/8b709942-572e-46ef-bb08-8fecef93d394" width="400" />  <br>

In Boards select rAVA 2Mhz:    
<img src="https://github.com/user-attachments/assets/9a1b4d58-5292-4c4c-ae64-b206f18510b0" width="400" />  <br>  

# Schematic  

<img src="https://github.com/user-attachments/assets/d44a3582-5e09-4798-9103-74a7370bb043" width="400" />

# Setup parameters in the code  
The Arduino code is setup from start to use a 26Mhz Tcxo and to use a 4Mhz xo divided by 2 to run at 2Mhz, if other values used the code needs to be changed - see below  

Values to edit in the ino file  

Sets the frequency and band  
#define WSPR_FREQ  

Sets your call  
const char call[] = "SA9BSS";  

Set initial power of the 5351  
int pwr_level=79;  
it will for every sequence increase the power one step so if you set it at 77 it will take two sequencies, 20 min before it will start at full power   
avalible valus are 76, 77, 78 and 79 where 79 is max power, 76 is just a few mW so I recommend not to use that, the default setting is max power 79

#include "Timing4.h"  
#include "Timing.h"  
Timing4.h is a testfile that will start tx every 2min, this it convinient to use during testing on the bench, final prep before flight is to edit the  
.ino file from #include "Timing4.h" to #include "Timing.h" this is the main config that should be used during a flight, it have a 10 min sequence as needed to fit the scheduling agreed on in the balloon community
Edit Timing.h sceduling at what minute you want to tx your normal wspr and at what minute to tx the telemetry, this can be done inside Arduino ide or with a normal texteditor  

To adjust the sequence in what order the standard and telemtry is transmitted edit Timing.h, it is set to transmitt 4 times - 8 minutes and then switching off the radio and enable gps, gets new loction data last 2 min of the 10 minute slot, then start over, default setting is: WSPR, WSPR_telemtry, WSPR, WSPR, this is set by adjusting the setModeWSPR();  or  setModeWSPR_telem(); in Timing.h file  

Sets speed of the board  
clock_prescale_set(clock_div_2);  or clock_prescale_set(clock_div_4);  
this in conjuction with:    
#define WSPR_CTC 1334  
and select rAVA 2Mhz from boards in Arduino

Alternative clocksettings:  
WSPR CTC - 2668 @ 4Mhz //1334 @ 2Mhz //667 @ 1Mhz  
And to select the appropriate board setting under Boards in Arduino,  
lets say we want to set the board to 1Mhz using a 4Mhz xo we need to set the prescaler to  
clock_prescale_set(clock_div_4);   
and set  
WSPR CTC 667  
and then select gAVA 1Mhz under boards, see section Programming  


then, for example if you wan to run it a 4Mhz set:
WSPR CTC - 2668  
mark out both of the //clock_prescale_set  
and use hAVA 4Mhz board setting  


CW setup, the board will at first moment at power on send a morse signal with your callsign or what you choose to set

Here is my SA6BSS call in the ino file  


cw_s(); cw_a(); cw_6(); cw_b(); cw_s(); cw_s();  


to set cw tx frequncy adjust it in Beep.h  
  // freq = 14096950UL;  
     freq = 18105900UL;  
  // freq = 28125870UL;  

And lastly, there is a couple of setting in the  
TelemFunctions.h  

Sets the frequncy of tcxo used  
#define F_XTAL 26000000;  

Sets Telemetry channel  
call_telemetry[0] = '0';  // set 0 or Q  
call_telemetry[2] = '8';  // set 0 to 9  

Available channels 00, 01, 02 - 08, 09 and Q0, Q1, Q2 - Q8, Q9  

If you have problem getting decodes going from 20m to 10m try change this in TelemFunctions.h <br>

 //outdivider 77000000000 or 30000000000 // If bad decodes at 24/28Mhz try the other value<br> 
  outdivider = 30000000000 / frequency;  // With 900 MHz beeing the maximum internal PLL-Frequency<br> 



# Balance the board

You want the board to be as leveled as possible so the solarcells always points up and not to the sides,  
to do this use the top hook, puti it on a short loop of fishing line and bend the copper rod untill it levels up, see below:  

<img src="https://github.com/user-attachments/assets/eec8681e-1199-46cd-8c37-9d269d65407c" width="300" />  <br> 


# Misc pictures

<img src="https://github.com/user-attachments/assets/b9990e72-a1dd-4269-a934-05309d96239c" width="500" />  <br> 
<img src="https://github.com/user-attachments/assets/d67339c1-1426-44fc-b76d-b88c0535a88e" width="500" />  <br> 

