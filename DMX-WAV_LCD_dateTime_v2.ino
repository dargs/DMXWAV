// dargstronix 
// adding lcd i2c 
// 1400 31/10/2016
//
// dmx ch 512
// trig pin 8
//
// * CLK to Analog #5
// * DAT to Analog #4

// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecv.ino: Sample DMX application for retrieving 3 DMX values:

// 
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// 
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// 28.12.2011 changed to channels 1..3 (RGB) for compatibility with the DmxSerialSend sample.
// 10.05.2012 added some lines to loop to show how to fall back to a default color when no data was received since some time.
// - - - - -

#include <DMXSerial.h>
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(0);

// macros from DateTime.h 
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

// track times
//#define tracknumberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
//#define tracknumberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 


const int TrigPin = 8;  // digital 8 to trigger mp3.

int startplaying = 0;
int playing = 0;
int tracktimer = 0;
unsigned long  startime = 0;


void setup () {
  DMXSerial.init(DMXReceiver);
  pinMode(TrigPin,   OUTPUT); // sets the digital pin as output
  digitalWrite(TrigPin,HIGH); // wave shield trig High - inverted, off 

  lcd.begin(16, 2);

  for (int x = 0; x<5;x++){ // Blink blacklight
  lcd.setBacklight(LOW);
  delay(250);
  lcd.setBacklight(HIGH);
  delay(250);}
  
  lcd.setCursor(0,0);
  lcd.print(" * DARGSTRONIX **");
  lcd.setCursor(16,1);
  lcd.print("* DMX WAVE TRIGGER v1*");  
  delay(1000);
  lcd.noAutoscroll();
  lcd.clear();
  
  
// End Setup
}

void loop() {
  unsigned long lastPacket = DMXSerial.noDataSince();   // Calculate how long no data backet was received

  
  if (lastPacket < 1000) {
      lcd.setCursor(7, 1); // Bottom Left
      lcd.print("Ch512:");
      lcd.print(DMXSerial.read(512)); // Display DMX Ch512 value
      lcd.print("       ");

           
    if (DMXSerial.read(512) >= 128) {
      lcd.setCursor(12, 0); // 4 from top left
      lcd.print("PLAY");
      digitalWrite(TrigPin,LOW); // trigger if dmx ch 1 is 128 or higher
      if (playing == 0) startplaying =1;
      tracktime((millis()/1000)-startime); // diplay track time

      
    }
    if (DMXSerial.read(512) <= 127) {
      lcd.setCursor(12, 0); // 4 from top left
      lcd.print("STOP");
      digitalWrite(TrigPin,HIGH); // don't trigger if dmx c1 is 127 or lower
      startplaying = 0;
      startime=(millis()/1000); // reset track counter
      lcd.setCursor(0,1); // bottom left
      lcd.print("--:--");
      }
  }
     
    else {
      lcd.setCursor(0, 1); // Bottom Left
      lcd.print("    NO DMX RX    ");
      lcd.setBacklight(LOW);
      delay(100);
      lcd.setBacklight(HIGH);
      delay(100);
  
    }


  if (startplaying ==1 & playing == 0){ // only enters this bit if it's just started playing
    playing = 1;
    startime=(millis()/1000);
    startplaying ==0; 
    }
      
  
  time(millis() / 1000); // display uptime

      
// end Loop  
}

void tracktime(long val){  
int minutes = numberOfMinutes(val);
int seconds = numberOfSeconds(val);

 lcd.setCursor(0,1); // bottom left
 printDigits(minutes);
 lcd.print(":");
 printDigits(seconds); 
 lcd.print(" ");
}


void time(long val){  
int days = elapsedDays(val);
int hours = numberOfHours(val);
int minutes = numberOfMinutes(val);
int seconds = numberOfSeconds(val);

 // digital clock display of current time

 lcd.setCursor(0,0);
 lcd.print(days,DEC);
 lcd.print(":");
 printDigits(hours);  
 lcd.print(":");
 printDigits(minutes);
 lcd.print(":");
 printDigits(seconds); 
}


void printDigits(byte digits){
 // utility function for digital clock display: prints colon and leading 0
 if(digits < 10)
   lcd.print('0');
 lcd.print(digits,DEC);  
}

// End.
