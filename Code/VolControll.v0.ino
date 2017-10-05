/*
 * Project VolControll
 * Control TV and Reciever volume using rotary encoder with Red/Green leds and push button
 * lg Volume up
      20DF40BF
  lg vol down
      20DFC03F
  lg tv power
      20DF10EF
  lg mute
      20DF906F
[Use irsend.sendLG(0x20DFC03F, 32); ]

  JVC vol up
      C578
  JVC vol down
      C5F8
  JVC power
      C0E8
  JVC TV OUT
      C508
  JVC mute
      C538
[for down use irsend.sendJVC(0xC5F8,16,0); // hex value, 16 bits, repeat
  delayMicroseconds(50);]
** Documents https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
* http://bildr.org/2012/08/rotary-encoder-arduino/
* 
 */
#include <IRremote.h>
#include <avr/sleep.h>

IRsend irsend;
#define encA A0 //Analong pin 0
#define encB A1//Analong pin 1
#define encPort PINC
int AvgCount=0; //to reduce errors- making avarage of 5 clicks
int EncoderAvg=0;
#define ButtonRotaryPIN 2
#define Led1 7
#define Led2 8
boolean LastButtonRotary=true;
uint8_t encoder,prevencoder;

unsigned long timer;
long SleepInterval = 10000; //sleep 10 sec from event  
long previousMillis = 0;

void setup()
{
  pinMode(encA, INPUT);
  digitalWrite(encA, HIGH);
  pinMode(encB, INPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(ButtonRotaryPIN, INPUT);
    digitalWrite(ButtonRotaryPIN,HIGH);
  digitalWrite(encB, HIGH);
  Serial.begin (9600);
  Serial.println("Let the game begin");
}


void loop() {
  timer = millis();
  int8_t tmpdata;
  tmpdata = read_encoder();
  if (!digitalRead(ButtonRotaryPIN)) {
    Serial.println("Push!");
    previousMillis = timer; //reset timer
    LastButtonRotary=!LastButtonRotary; //change button state
    if (LastButtonRotary) {//Mute TV/Reciever
      SendIRSignal(5);
      SendIRSignal(2);
    }
    else{
      SendIRSignal(2);
      SendIRSignal(5);
    }
    delay(100); //reduce button errors
  }
  //change led color
  digitalWrite(Led1,LastButtonRotary);
  digitalWrite(Led2,!LastButtonRotary);
  if( tmpdata ) {
    AvgCount+= 1;
    previousMillis = timer; //reset timer
    EncoderAvg+=tmpdata;
    Serial.print(tmpdata);
    //Serial.print("EncoderAvg "); Serial.println(EncoderAvg);
    if (AvgCount==5){
      if (RotaryEncoderPosition()){ //clockwise knob rotation 
        Serial.println("up");
        if (LastButtonRotary)
          SendIRSignal(0); //Send to TV
       else 
          SendIRSignal(3); //Send to Reciever
      }else{
        if (LastButtonRotary)
          SendIRSignal(1);//Send to TV
        else
          SendIRSignal(4);//Send to Reciever
        Serial.println("down");

        }
    }
  }
  if(timer - previousMillis > SleepInterval) { //Check for Sleep interval
      previousMillis = timer; 
      sleepNow();
  }
}

// Rotary Encoder Position, return true for clockwise rotation 
boolean RotaryEncoderPosition(){
  boolean rtn;
      if (EncoderAvg>0) rtn=true;
        else
        rtn=false;
      AvgCount=0;
      EncoderAvg=0;    
  return rtn;
}

//sending signal to IR
void SendIRSignal(int option){
  for (int i = 0; i < 3; i++) {
    switch (option){
      case 0:// LG Volume up
        SendLGVolUp();
      break;
      case 1:// LG Volume Down
        SendLGVolDown();
      break;
      case 2:// LG mute
        SendLGMute();
      break;
      case 3:// JVC Volume up
        SendJVCVolUp();
      break;
      case 4:// JVC Volume Down
        SendJVCVolDown();
      break;
      case 5:// JVC Mute
        SendJVCMute();
      break;
    }
    delay(40);
  }
}
//Sending IR Signal for JVC Volume up
void SendJVCVolUp(){
  irsend.sendJVC(0xC578,16,0);
  delayMicroseconds(50);
}

//Sending IR Signal for LG Mute
void SendLGMute(){
  irsend.sendLG(0x20DF906F, 32);
}

//Sending IR Signal for JVC Mute
void SendJVCMute(){
  irsend.sendJVC(0xC538,16,0);
  delayMicroseconds(50);
}

//Sending IR Signal for JVC Volume down
void SendJVCVolDown(){
  irsend.sendJVC(0xC5F8,16,0);
  delayMicroseconds(50);
}

//Sending IR Signal for LG Volume up
void SendLGVolUp(){
  irsend.sendLG(0x20DF40BF, 32);
}

//Sending IR Signal for LG Volume down
void SendLGVolDown(){
  irsend.sendLG(0x20DFC03F, 32);
}

//read rotary encoder and return (1) for clockwise or (-1) anti-clockwise
int8_t read_encoder()
{
  static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static uint8_t old_AB = 0;
  old_AB <<= 2;      
  old_AB |= ( encPort & 0x03 );  
  return ( enc_states[( old_AB & 0x0f )]);
} 
//Wake up from sleep
void wakeUpNow()   {     
  detachInterrupt(0);
  Serial.println("good morning!!"); 
}

// Put the arduino to sleep mode for low power consumption 
void sleepNow()  { 
  Serial.println("Go to sleep");
  digitalWrite(ButtonRotaryPIN,HIGH);
  // low all leds
  digitalWrite(Led1,LOW);
  digitalWrite(Led2,LOW);
  attachInterrupt(0, wakeUpNow, LOW);
  delay(100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   
  sleep_enable(); 
  sleep_bod_disable();
  sei();
  sleep_mode();
  /* The program will continue from here. */
    /* First thing to do is disable sleep. */
  sleep_disable();
  delay(200); //reduce button errors
}

