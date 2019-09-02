## Using c.l.i


particle upgrade

```
particle 
LaurusNobilis:ParticleElectron grantsteinfeld$ particle login
? Please enter your email address grant.steinfeld.tech@gmail.com

#email
#password

```

    > Successfully completed login!


    ### confirm your particle is listed
    LaurusNobilis:ParticleElectron grantsteinfeld$ particle list
    ichmilatis-pragma [3b0027000647373336373936] (Electron) is online
    Functions:
        int SetInterval(String args)
        int GetCurrTemp(String args)
        int SetXYZThresh(String args)
        int GetRecentXYZ(String args)
    LaurusNobilis:ParticleElectron grantsteinfeld$

### lsusb ? may work ?
may not be necessary if got this far?

### so what libraries do you have?




### DHT11

Wire up ...
https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor

<< my photos ... explain video if works!!>>

//copiend John's properties off WatsonIoTAssetTracker.ino
updated project.properties

#### put particle into DFU (USB cable connected ) mode


-----------------------
[x]  << rgb led>> [y]

||
usb cable
------------------------


push both buttons
hold 5sec
release left button [x]

``` 
should be flashing yellow!!
```

[777]


#### is usb connected?

use 
```sh
lsusb

#tricky bit - restated mac!
#here it shows up!!! :)
2019-09-02 18:29:48.078 system_profiler[1676:10223] SPUSBDevice: IOCreatePlugInInterfaceForService failed 0xe00002be
...
Bus 020 Device 007: ID 2b04:c00a 2b04 Electron  Serial: 3b0027000647373336373936
...

```
### Another way to check you are connected properly
## DF UTIL
[install and useage here](https://docs.particle.io/support/particle-tools-faq/installing-dfu-util/)

 
```sh
dfu-util -l
LaurusNobilis:IoT-AssetTracking-Perishable-Network-Blockchain grantsteinfeld$ dfu-util -l
dfu-util 0.9

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2016 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

Found DFU: [2b04:d00a] ver=0250, devnum=9, cfg=1, intf=0, path="20-2", alt=1, name="@DCT Flash   /0x00000000/01*016Kg", serial="3b0027000647373336373936"
Found DFU: [2b04:d00a] ver=0250, devnum=9, cfg=1, intf=0, path="20-2", alt=0, name="@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg", serial="3b0027000647373336373936"
```

found DFU!!! looks good!!





#### see what libraries are avail for DHT?
```sh
particle library list *DHT*

#Adafruit_DHT 0.0.4 232462 Spark Core DHT Library based on Adafruit Arduino DHT Library
#Adafruit_DHT_Particle 0.0.2 55817 Spark Core DHT Library based on Adafruit Arduino DHT Library

```
we need the second one for particle


#### add dependancy!
``` sh

particle library view Adafruit_DHT_Particle

#success if you see!!
Checking library Adafruit_DHT_Particle...
Library Adafruit_DHT_Particle 0.0.2 installed.
To view the library documentation and sources directly, please change to the directory /Users/grantsteinfeld/Particle/community/libraries/Adafruit_DHT_Particle@0.0.2
```

## Pro Tip you may need to do this for all libraries in your project.properties file!!

### so now we should be ready to try compiling!
```sh
p w d  
IoT-AssetTracking-Perishable-Network-Blockchain/ParticleElectron
cd ..
p w d 
IoT-particle/git/IoT-AssetTracking-Perishable-Network-Blockchain
#great so now you are one level above the 
particle compile electron ParticleElectron --saveTo WatsonIoTAssetTracker.bin

particle compile electron ParticleElectron --saveTo WatsonIoTAssetTracker.bin


Including:
    ParticleElectron/WatsonIoTAssetTracker.ino
    ParticleElectron/project.properties
attempting to compile firmware
downloading binary from: /v1/binaries/5d6d92000be7a300013dfe90
saving to: WatsonIoTAssetTracker.bin
Memory use:
   text	   data	    bss	    dec	    hex	filename
  29468	    180	   2264	  31912	   7ca8	/workspace/target/workspace.elf

Compile succeeded.
Saved firmware to: /Users/grantsteinfeld/Documents/dev/IoT-particle/git/IoT-AssetTracking-Perishable-Network-Blockchain/WatsonIoTAssetTracker.bin
```

### Great we compiled the ino!  
### Lets flash the particle with the new binary!
```sh
particle flash --usb WatsonIoTAssetTracker.bin

#!!! I was unable to detect any devices in DFU mode... [777]
EEK!
RESET PUSH LEFT [y] button
restarted mac ran lsusb and dfu-util -l ..> looked better!

#Flash success!

```

### running the serial monitor to check on things
```sh
particle serial monitor

```










vvvvvvv




## Troubleshooting

[777]
Restart computer - usb seems to remember!
 

!!! I was unable to detect any devices in DFU mode...

> Your device will blink yellow when in DFU mode.
> If your device is not blinking yellow, please:

1) Press and hold both the RESET/RST and MODE/SETUP buttons simultaneously.

2) Release only the RESET/RST button while continuing to hold the MODE/SETUP button.

3) Release the MODE/SETUP button once the device begins to blink yellow.
    