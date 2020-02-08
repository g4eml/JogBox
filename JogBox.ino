
//Duet 2 Remote Pendant.
//Connects to PanelDue port.
//Uses Arduino Pro Mini 3V3 module (must be 3V3 version)
//Rotary encoder on pins 2 and 3.
//LEDS via 470 Ohm resistors to Ground on pins 6,8,10 for Indicating  X Y Z axis
//Push Buttons to Ground with 4K7 Pull up resistors on pins 5,7,9 for selecting X Y Z Axis
//Push Buttons to Ground with 4K7 Pull up resistors on Pins 11,12,13,A0,A1,A2,A3,A4 for Custom commands
//Connection to PanelDue port using serial port on pins 0 and 1 plus ground and 5V.  
//
//Colin Durbridge G4EML February 2020

#include <Encoder.h>

float stepsize;
bool absmode;
bool highspeed;
bool butok;
long lastmove;
long lastbut;
long lastresp;
long oldPosition;
int i;
int j;
int sent[8];
int axis=0;
int key[8];
char csm;
char str[20];
int linenumber;
bool respavail;


#define largestep 0.5
#define smallstep 0.05

char * command[]={"M98 P\"/macros/JogBox/1.g\"","M98 P\"/macros/JogBox/2.g\"","M98 P\"/macros/JogBox/3.g\"","M98 P\"/macros/JogBox/4.g\"",
                  "M98 P\"/macros/JogBox/5.g\"","M98 P\"/macros/JogBox/6.g\"","M98 P\"/macros/JogBox/7.g\"","M98 P\"/macros/JogBox/8.g\""};

#define RA 2
#define RB 3
#define LEDX 10
#define LEDY 8
#define LEDZ 6
#define SWX 9
#define SWY 7
#define SWZ 5
#define KEY1 12
#define KEY2 13
#define KEY3 15
#define KEY4 17
#define KEY5 11
#define KEY6 14
#define KEY7 16
#define KEY8 18



Encoder myEnc(RA, RB);


void setup() 
{
  Serial.begin(57600);
  Serial.setTimeout(1000);
  pinMode(LEDX,OUTPUT);
  pinMode(LEDY,OUTPUT);
  pinMode(LEDZ,OUTPUT);
  pinMode(SWX,INPUT_PULLUP);
  pinMode(SWY,INPUT_PULLUP);
  pinMode(SWZ,INPUT_PULLUP);
  key[0]=KEY1;
  key[1]=KEY2;
  key[2]=KEY3;
  key[3]=KEY4;
  key[4]=KEY5;
  key[5]=KEY6;
  key[6]=KEY7;
  key[7]=KEY8;
   
  for(i=0;i<8;i++)
  {
    pinMode(key[i],INPUT_PULLUP);
  }
  absmode=true;
  lastmove=0;
  highspeed=false;
  butok=true;
  lastbut=millis();
  lastresp=millis();
  axis=0;
  digitalWrite(LEDX,LOW);
  digitalWrite(LEDY,LOW);
  digitalWrite(LEDZ,LOW);
  delay(500);
  csm=0;
  linenumber=1;
  respavail=false;
  oldPosition=-999;

  for(i=0;i<5;i++)              // send a few linefeeds to establish serial link. 
  {
    Serial.println("");
  }
}



void chsprintlinenumber()
{
  csm=0;
  sprintf(str,"N%d ",linenumber++);
  chsprint(str);
}


void chsprint(char* st)
{
  for(j=0;st[j]!=0;j++)
  {
    csm=csm ^ st[j];
  }
  Serial.print(st);
}
 
void chsprintln(char* st)
{
  for(j=0;st[j]!=0;j++)
  {
    csm=csm ^ st[j];
  }
  Serial.print(st);
  Serial.print("*");
  Serial.println(csm & 0xff);
  csm=0;
  respavail=true;
}

void flushresp(void)
{
  chsprintlinenumber();
  chsprintln("M408 S0 R1");            //request Json Status message including most recent errors. (We do nothing with it. This is just to stop the reprap output buffers filling up)
  respavail=false;
  lastresp=millis();
}




void loop() 
{
  long newPosition = myEnc.read()/2;
  if (newPosition != oldPosition) 
  {
   if(axis>0)
     {
      myEnc.write(0);
      oldPosition = 0;
      if(absmode)
      {
        chsprintlinenumber();
        chsprintln("M120");
        chsprintlinenumber();
        chsprintln("G91");
        absmode=false;        
      }
      if(highspeed)
      {
        stepsize=largestep; 
      }
      else 
      {
        stepsize=smallstep;
      }
      chsprintlinenumber();
      chsprint("G0 ");
      if(axis==1) chsprint("X");
      if(axis==2) chsprint("Y");
      if(axis==3) chsprint("Z");
      float mv=newPosition*stepsize;
      dtostrf(mv,4,2,str);
      chsprintln(str);
      lastmove=millis();
     }
    else
     {
      myEnc.write(0);
      oldPosition = 0; 
     }
  }

  if(((millis()-lastmove) > 1000) && (absmode==false))
  {
    chsprintlinenumber();
    chsprintln("G90");
    absmode=true;
    chsprintlinenumber();
    chsprintln("M121");
  }


  if(((millis()-lastresp)>1000) && respavail)
  {
    flushresp();
  }
  

if(digitalRead(SWX)==HIGH && digitalRead(SWY)==HIGH && digitalRead(SWZ)==HIGH)
  {
    highspeed=false;
    if(millis()>lastbut+200) butok=true;
  }


if(digitalRead(SWX)==LOW && digitalRead(SWY)==HIGH && digitalRead(SWZ)==LOW)
   {
    axis=0;
    highspeed=false;
    lastbut=millis();
    butok=false;
    digitalWrite(LEDX,LOW);
    digitalWrite(LEDY,LOW);
    digitalWrite(LEDZ,LOW);
   }
 
     if(digitalRead(SWX)==LOW && digitalRead(SWY)==HIGH && digitalRead(SWZ)==HIGH)
     {
      highspeed=true;
      if(butok)
      {
      axis=1;
      lastbut=millis();
      butok=false;
      digitalWrite(LEDX,HIGH);
      digitalWrite(LEDY,LOW);
      digitalWrite(LEDZ,LOW);
      }
     }
     if(digitalRead(SWX)==HIGH && digitalRead(SWY)==LOW && digitalRead(SWZ)==HIGH)
     {
      highspeed=true;
      if(butok)
      {
      axis=2;
      lastbut=millis();
      butok=false;
      digitalWrite(LEDX,LOW);
      digitalWrite(LEDY,HIGH);
      digitalWrite(LEDZ,LOW);
      }
     }
     if(digitalRead(SWX)==HIGH && digitalRead(SWY)==HIGH && digitalRead(SWZ)==LOW)
     {
      highspeed=true;
      if(butok)
      {
      axis=3;
      lastbut=millis();
      butok=false;
      digitalWrite(LEDX,LOW);
      digitalWrite(LEDY,LOW);
      digitalWrite(LEDZ,HIGH);
      }
     }



 

 for(i=0;i<8;i++)
 {
   if(digitalRead(key[i])==LOW)
    {
      if(sent[i]==0)
      {
      chsprintlinenumber();
      chsprintln(command[i]);
      }
      sent[i]=2000;
    }
  else
    {
      if(sent[i]>0) sent[i]--;
    }

 }

 
}
