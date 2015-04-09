#include <SPI.h>
#include <ar1010lib.h>
AR1010 radio = AR1010();

// TEA5767 und Nokia 5110 LCD Display

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// D7 - Serial clock out (CLK oder SCLK)
// D6 - Serial data out (DIN)
// D5 - Data/Command select (DC oder D/C)
// D4 - LCD chip select (CE oder CS)
// D3 - LCD reset (RST)
Adafruit_PCD8544 lcd = Adafruit_PCD8544(7, 6, 5, 4, 3);

#include <Wire.h>

#define button_frequency_up     13 
#define button_frequency_down   12 
#define button_mute             8
#define button_seek             9
#define button_seek_up          10
#define button_seek_down        11

#define TEA5767_mute_left_right  0x06
#define TEA5767_MUTE_FULL        0x80
#define TEA5767_ADC_LEVEL_MASK   0xF0
#define TEA5767_STEREO_MASK      0x80


int old_frequency=-1;
int frequency=1067;

byte old_stereo=0;
byte stereo=1;

byte old_mute=1;
byte mute=0;

byte old_signal_level=1;
byte signal_level=0;

byte old_seek_mode=1;
byte seek_mode=0;

int flash1 = 100;


unsigned long last_pressed;

void setup(void) {
  
  pinMode(button_frequency_up, INPUT);
  pinMode(button_frequency_down, INPUT);
  pinMode(button_mute, INPUT);
  pinMode(button_seek_up, INPUT);
  pinMode(button_seek_down, INPUT);
  pinMode(button_seek, INPUT);
  
  
  
  Serial.begin(9600);
  Wire.begin();
  
  radio.initialise();
  radio.setVolume(18);
  radio.setFrequency(frequency);
  

  lcd.begin();
  lcd.setContrast(65);
  lcd.clearDisplay();
   
  set_text(1,2,"FM Radio",BLACK,1);  
  set_text(1,39,"Niklas & Khoa",BLACK,0.1);
  
}


void loop() {

    if(frequency!=old_frequency){
      set_text(old_frequency>=10000?6:14,17,value_to_string(old_frequency),WHITE,2);
      set_text(frequency>=10000?6:14,17,value_to_string(frequency),BLACK,2);
      old_frequency=frequency;
    }
    
    TEA5767_read_data();
      
  if(old_seek_mode!=seek_mode){
      
             
        
       set_text(77,8,old_seek_mode?"":"S",WHITE,1);
       set_text(77,8,seek_mode?"":"S",BLACK,1);
       
       set_text(77,16,old_seek_mode?"":"E",WHITE,1);
       set_text(77,16,seek_mode?"":"E",BLACK,1);
       
       set_text(77,24,old_seek_mode?"":"E",WHITE,1);
       set_text(77,24,seek_mode?"":"E",BLACK,1);
              
       set_text(77,32,old_seek_mode?"":"K",WHITE,1);
       set_text(77,32,seek_mode?"":"K",BLACK,1);
       
       old_seek_mode=seek_mode;
       
     /*  set_text(77,8,old_seek_mode?"S":"",WHITE,1);
       set_text(77,8,seek_mode?"S":"",BLACK,1);
      
       set_text(77,16,old_seek_mode?"E":"",WHITE,1);
       set_text(77,16,seek_mode?"E":"",BLACK,1);
       
       set_text(77,24,old_seek_mode?"E":"",WHITE,1);
       set_text(77,24,seek_mode?"E":"",BLACK,1);
              
       set_text(77,32,old_seek_mode?"K":"",WHITE,1);
       set_text(77,32,seek_mode?"K":"",BLACK,1);
       old_seek_mode=seek_mode;
       
           
       }*/
      
      
      
    }

    
    
    if(old_mute!=mute){
        set_text(1,17,old_mute?"M":"S",WHITE,1);
        set_text(1,17,mute?"M":"S",BLACK,1);
        old_mute=mute;
    }
      
    delay(50);
    

  if(digitalRead(button_mute)==HIGH){
    Serial.println("Mute: an");
    TEA5767_mute();
  }
  
   if(digitalRead(button_seek)==HIGH){
     seek_mode = 1;
    if(digitalRead(button_seek_up)==HIGH){
      radio.seek('u');
      delay(100);
      radio.setFrequency(radio.frequency());
      frequency = radio.frequency();
      set_text(1,39,"Niklas & Khoa",BLACK,0.1);
      Serial.println(radio.frequency());
    }
    else if(digitalRead(button_seek_down)==HIGH){
     radio.seek('d');
     delay(100);
     radio.setFrequency(radio.frequency());
     frequency = radio.frequency();
     set_text(1,39,"Niklas & Khoa",BLACK,0.1);
     Serial.println(radio.frequency());
    } 
    
  }
  else{ 
  TEA5767_manual();
  seek_mode = 0;
}
  
  delay(50);
  
}
/*
unsigned char frequencyH = 0;
unsigned char frequencyL = 0;

unsigned int frequencyB;

unsigned char TEA5767_buffer[5]={0x00,0x00,0xB0,0x10,0x00};
*/


void TEA5767_mute(){ 
 /* 
  if(!mute){   
    mute = 1;   
    TEA5767_buffer[0] |= TEA5767_MUTE_FULL;
    TEA5767_write_data(2);
//    TEA5767_buffer[0] &= ~TEA5767_mute;
//    TEA5767_buffer[2] |= TEA5767_mute_left_right;
  }   
  else{
    mute = 0;   
    TEA5767_buffer[0] &= ~TEA5767_MUTE_FULL;
    TEA5767_write_data(2);
//    TEA5767_buffer[0] |= TEA5767_mute;
//    TEA5767_buffer[2] &= ~TEA5767_mute_left_right;
  }
    
//  TEA5767_write_data(3);*/

if(!mute){
  mute = 1;
  radio.setHardmute(mute);
  }
  else{
    mute= 0;
    radio.setHardmute(mute);
 }


}
/*
void TEA5767_seekmode(){ 

if(seek_mode=1){
  seek_mode = 1;
  TEA5767_seek();
  }
  else{
   seek_mode = 0;
    TEA5767_manual();
 }


}

void TEA5767_seek(){
  if(digitalRead(button_seek_up)==HIGH){
    radio.seek('u');
    seek_mode = 0;
  }
  if(digitalRead(button_seek_down)==HIGH){
    radio.seek('d');
    seek_mode = 0;
  }


}
*/
void TEA5767_manual(){  
  if(digitalRead(button_frequency_down)==HIGH){
    
    frequency=frequency-1;
    if(frequency<875)frequency=1080;
    TEA5767_set_frequency();
  }
  if(digitalRead(button_frequency_up)==HIGH){
    
    frequency=frequency+1;
    if(frequency>1080)frequency=875;
    TEA5767_set_frequency();
  }
}


void TEA5767_set_frequency()
{
 radio.setFrequency(frequency);
}

int TEA5767_read_data() {
  /*
  unsigned char buf[5];
  memset (buf, 0, 5);
  
  Wire.requestFrom (0x60, 5); 

  if (Wire.available ()) {
    for (int i = 0; i < 5; i++) {
      buf[i] = Wire.read ();
    }
        
    stereo = (buf[2] & TEA5767_STEREO_MASK)?1:0;
    signal_level = ((buf[3] & TEA5767_ADC_LEVEL_MASK) >> 4);
    
    return 1;
  } 
  else return 0;*/
}

void show_signal_level(int level){
  /*
  byte xs=68;
  byte ys=8;
  for(int i=0;i<15;i++){
    if(i%2!=0)lcd.drawLine(xs+i,ys,xs+i,ys-i/2,level>=i?BLACK:WHITE);
  }
  */
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

