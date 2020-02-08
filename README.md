# JogBox
JogBox. A MPG Pendant for the Duet 2 Controller

The Duet 2 Controller was primarily designed for use with 3D printers but is becoming more popular as a controller for CNC machines. 
One of the disadvantages of the Web Based interface is that it does not suit the fine jogging requirements for CNC use. 

The JogBox connects to the PanelDue Serial port connector on the Duet 2 board and provides a rotary encoder which can be used to jog the CNC machine with the control needed. It also includes 8 buttons which, when pressed, call macros to do any user defined tasks required. 

The controller used is the Arduino Pro Mini board. This must be the 3V3 version. Other Arduinos can probably be used as long as they have a VCC of 3V3. 

You will need to add the Encoder Library by Paul Stoffregen to your Arduino IDE before the sketch will compile. 
This Library can be found by clicking on  'Sketch' 'Include Library' 'Manage Libraries' and search for 'Encoder. Click on 'Install' to add it to your IDE. 


In operation select the X Y or Z axis using the three push buttons and then rotate the rotary encoder. Movement will be 0.05mm per click. If you need to move faster then hold down the X Y or Z button whilst rotating the encoder. Movement will then be in steps of 0.5mm. 

To prevent accidental movement the encoder can be de-selected by pressing X and Z together. 

The 8 Macro buttons can be programmed to do any common task you wish. You will need to create a macro for each button using the Duet Web Interface. First select 'Macros' and then create a folder called 'JogBox' inside that folder create the 8 macros. These must be called 1.g  through to 8.g.  The macros can contain any G code commands. 





