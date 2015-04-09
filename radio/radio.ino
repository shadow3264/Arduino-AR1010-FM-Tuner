// Adapted from http://blog.simtronyx.de/einfaches-fm-stereo-radio-mit-dem-tea5767-und-einem-arduino/ and changed to work with the AR1010 FM Module

// Pins:

// LCD:
//  D7 - Serial clock out (CLK oder SCLK)
//  D6 - Serial data out (DIN)
//  D5 - Data/Command select (DC oder D/C)
//  D4 - LCD chip select (CE oder CS)
//  D3 - LCD reset (RST)

// AR1010:
//  A4 - Data
//  A5 - Clock

// Buttons (Pullup):
//  D8 - Mute
//  D9 - Seekmode
// D10 - Seek Up
// D11 - Seek Down
// D12 - Frequency Up
// D13 - Frequency Down



#include <SPI.h>
#include <ar1010lib.h>
AR1010 radio = AR1010();

// AR1010 und Nokia 5110 LCD Display

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


Adafruit_PCD8544 lcd = Adafruit_PCD8544(7, 6, 5, 4, 3);

#include <Wire.h>

#define button_frequency_up     13 
#define button_frequency_down   12 
#define button_mute             8
#define button_seek             9
#define button_seek_up          10
#define button_seek_down        11


int old_frequency=-1;
int frequency=1067;

byte old_mute=1;
byte mute=0;

byte old_seek_mode=1;
byte seek_mode=0;

void setup(void) {
  
  pinMode(button_frequency_up, INPUT);
  pinMode(button_frequency_down, INPUT);
  pinMode(button_mute, INPUT);
  pinMode(button_seek_up, INPUT);
  pinMode(button_seek_down, INPUT);
  pinMode(button_seek, INPUT);
  
//  Serial.begin(9600);
  Wire.begin();
  
  radio.initialise();
  radio.setVolume(18);
  radio.setFrequency(frequency);
  

  lcd.begin();
  lcd.setContrast(65);
  lcd.clearDisplay();
   
  set_text(1,2,"FM Radio",BLACK,1);  
  set_text(1,39,"(c)shadow3264",BLACK,0.1);
  //set_text(1,39,"Niklas & Khoa",BLACK,0.1);
  
}


void loop() {

    if(frequency!=old_frequency){
      set_text(old_frequency>=10000?6:14,17,value_to_string(old_frequency),WHITE,2);
      set_text(frequency>=10000?6:14,17,value_to_string(frequency),BLACK,2);
      old_frequency=frequency;
    }
    
          
  if(old_seek_mode!=seek_mode){
      
       set_text(77,4,old_seek_mode?"S":"",WHITE,1);
       set_text(77,4,seek_mode?"S":"",BLACK,1);
       
       set_text(77,12,old_seek_mode?"E":"",WHITE,1);
       set_text(77,12,seek_mode?"E":"",BLACK,1);
       
       set_text(77,20,old_seek_mode?"E":"",WHITE,1);
       set_text(77,20,seek_mode?"E":"",BLACK,1);
              
       set_text(77,28,old_seek_mode?"K":"",WHITE,1);
       set_text(77,28,seek_mode?"K":"",BLACK,1);
       
       old_seek_mode=seek_mode;
         
    }

    
    
    if(old_mute!=mute){
        set_text(1,17,old_mute?"M":"S",WHITE,1);
        set_text(1,17,mute?"M":"S",BLACK,1);
        old_mute=mute;
    }
      
    delay(50);
    

  if(digitalRead(button_mute)==HIGH){
    Serial.println("Mute: an");
    AR1010_mute();
  }
  
   if(digitalRead(button_seek)==HIGH){
     seek_mode = 1;
    if(digitalRead(button_seek_up)==HIGH){
      radio.seek('u');
      delay(100);
      radio.setFrequency(radio.frequency());
      frequency = radio.frequency();
      //set_text(1,39,"Niklas & Khoa",BLACK,0.1);
      Serial.println(radio.frequency());
    }
    else if(digitalRead(button_seek_down)==HIGH){
     radio.seek('d');
     delay(100);
     radio.setFrequency(radio.frequency());
     frequency = radio.frequency();
     //set_text(1,39,"Niklas & Khoa",BLACK,0.1);
     Serial.println(radio.frequency());
    } 
    
  }
  else{ 
  AR1010_manual();
  seek_mode = 0;
}
  
  delay(50);
  
}

void AR1010_mute(){ 

if(!mute){
  mute = 1;
  radio.setHardmute(mute);
  }
  else{
    mute= 0;
    radio.setHardmute(mute);
 }


}

void AR1010_manual(){  
  if(digitalRead(button_frequency_down)==HIGH){
    
    frequency=frequency-1;
    if(frequency<875)frequency=1080;
    AR1010_set_frequency();
  }
  if(digitalRead(button_frequency_up)==HIGH){
    
    frequency=frequency+1;
    if(frequency>1080)frequency=875;
    AR1010_set_frequency();
  }
}


void AR1010_set_frequency()
{
 radio.setFrequency(frequency);
}



void set_text(int x,int y,String text,int color,int textsize){
  
  lcd.setTextSize(textsize);
  lcd.setTextColor(color); 
  lcd.setCursor(x,y);     
  lcd.println(text);      
  lcd.display();        
}


String value_to_string(int value){
  
  String value_string = String(value / 10);
  value_string = value_string + '.' + ((value%100<10)?"0":"") + (value % 10);
  return value_string;
  
}

