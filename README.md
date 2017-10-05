# Arduino IR Volume Control
Arduino volume control-  will control the TV/Receiver volume using IR LEDS
![N|Handle image](https://github.com/sdebby/Arduino_IR_Volume_Control/blob/master/images/fr_2103.jpg)

Small size disk with rotary top (Amazon DOT stile)

![N|Handle image](https://github.com/sdebby/Arduino_IR_Volume_Control/blob/master/images/EcoRoll.JPG)

# Hardware
* [Arduino nano v3.0](http://www.ebay.com/itm/162002876661?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT)
* [Rotary Encoder](https://www.sparkfun.com/products/10596)
* breadboard.
* 3 X 5mm 940nm IR LEDs.
* 330 oham Resistors.
* Wires and connectors.
* 3D SLA printer to print the encloser.

# Software
Using IRremote librery: 
https://github.com/z3t0/Arduino-IRremote

# Operating
* Pressing the rotary switch will wake up the device, to the previous mode.
* Rotating clockwise will increase the volume.
* Rotating counter clockwise will decrease the volume.
* Pressing the rotary switch while device awake, will change the control device mode and rotary leds, and mute one device. - Green led means controlling the TV volume and Red led means controlling the sound receiver volume.
* The control will enter sleep mode 10 seconds after last operation.

# Schematics
![N|Schemaic image](https://github.com/sdebby/Arduino_IR_Volume_Control/blob/master/images/VolControll.v0_bb.jpg)

# Video
[![YT_Video](https://img.youtube.com/vi/DBQewHMMKIc/0.jpg)](https://www.youtube.com/watch?v=kdwFtUYQnUo)
