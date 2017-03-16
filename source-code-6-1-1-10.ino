//fodder solar controller, by time only
//RTC, corrected starts with serial number 70,updated so that relay 16 goes off during cycle
/*switch mappings
22 ground
23 ground
24 	15    1
25 	16    2
26	13    3
27	14    4
28	11    5
29	12    6
30      9     7
31	10    8
32	7     9
33	8    10
34	5    11
35	6    12
36	3     13
37	4    14
38	1     15
39	2   16*/
//#include "DHT.h"
#include <Wire.h>
#include <DS1307RTC.h>
//#include "TSL2561.h"
#include <Time.h>
#include <DateTime.h>
#include <DateTimeStrings.h>
#include <avr/wdt.h>
#include <LiquidCrystal.h>
#include <LCDKeypad.h>



#include <EEPROMex.h>
// Adco for grove sensor, connect yellow wire(NC) to pin 13, red to 5v, black to grnd
//#define DHTPIN 13
//#define DHTTYPE DHT22 

int wPin[15];

    #define wPin[0] 24
    #define wPin[1] 25
    #define wPin[2] 26
    #define wPin[3] 27
    #define wPin[4] 28
    #define wPin[5] 29
    #define wPin[6] 30
    #define wPin[7] 31
    #define wPin[8] 32
    #define wPin[9] 33
    #define wPin[10] 34
    #define wPin[11] 35
    #define wPin[12] 36
    #define wPin[13] 37
    #define wPin[14] 38
    
    


#define CONFIG_VERSION "ls1"
#define memoryBase 32
//DHT dht(DHTPIN, DHTTYPE);
//TSL2561 tsl(TSL2561_ADDR_FLOAT);
long previousMillis = 0;
long currentMillis;
long logpreviousMillis;
long logcurrentMillis;
long interval = 1000;
int t;
int x;
int y;
double lightcalc;
float accumlight;
float accumlightdaily;
int irrigating;
int irrigatingZ[15];
int dayvar;
int daychangeflag;
int secondchangeflag;
 int hourvar;
  int minutevar;







float tempcalc;
float temp_f;
float temperature;
float temptemp = 0;

float humidity;
int settings;
int irrigatebytime;
bool ok  = true;
int configAdress=0;

LCDKeypad lcd;
int menulevel = 1;
int buttonPressed;
int alternate = 0;

tmElements_t tm;

byte c_up[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
};

byte c_down[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
};

byte c_select[8] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
};

// Example settings structure
struct StoreStruct {
    char version[4];   // This is for mere detection if they are your settings
    int a, b;          // The variables of your settings
    double l; //light, accumulated moles set point
    double h; //humidity set point
    double t[8]; //temperature set points
    double wtime[15];
    int r; //run or stop system
    float serialnum;
    char greenhousename[25];
    int ipmtime[4];
    int irrigatetimes[16];
    int irrigatebytime;
    int autostart;
    int senddata;
    float alerttemp;
    int timezone;
    int sdayvar;

} storage = { 
    CONFIG_VERSION,
    220, 1884,
    2.5,
    82.5,
    {64.50, 65.50, 68.50, 68.50, 70.50, 70.50},
    {5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5,5.5},
    0,
    11010021.00,
    "Test",
    {1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    1,
    0,
    1,
    58,
    5,
    1
};
bool loadConfig() {
  EEPROM.readBlock(configAdress, storage);
  return (storage.version == CONFIG_VERSION);
}
void saveConfig() {
   EEPROM.writeBlock(configAdress, storage);
}
void setup()
{
   wdt_disable();
     wdt_reset();
  wdt_enable (WDTO_8S); 
 
     
   
   if (RTC.read(tm)) 
   {
       setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
 
       
   }
   else
   {
     setTime(0,0,0,1,1,13);
   }
   //}
    
   // Serial.begin(38400); // Open serial connection to report values to host
   // Serial.println(F("Starting up"));
   
   //setTime(8,8,0,1,1,13); // set time to Saturday 8:29:00am Jan 1 2011
  //dht.begin();  
   //if (tsl.begin()) {
    //Serial.println(F("Found sensor"));
  //} else {
   // Serial.println(F("No sensor?"));
  //  lcd.write("Error - no light sensor");
 //   stopSketch();
 // }
 // tsl.setGain(TSL2561_GAIN_0X);
 // tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);
  irrigating = 0;
  EEPROM.setMemPool(memoryBase, EEPROMSizeMega); //Set memorypool base to 32, assume Arduino Uno board
  configAdress  = EEPROM.getAddress(sizeof(StoreStruct)); // Size of config object 
  
  //set up 

   //set up 
 
  storage.h = 82.00;
  storage.t[0]= 64.50;
  storage.t[1]= 65.50;
  storage.t[2]= 1.00;
  storage.t[3]= 68.50;
  storage.t[4]= 70.50;
  storage.t[5]= 70.50;
  storage.t[6]= 70.50;
  storage.t[7]= 1.00;
  storage.wtime[0]= 60;
  storage.wtime[1]= 60;
  storage.wtime[2]= 60;
  storage.wtime[3]= 60;
  storage.wtime[4]= 60;
  storage.wtime[5]= 60;
  storage.wtime[6]= 60;
  storage.wtime[7]= 60;
  storage.wtime[8]= 60;
  storage.wtime[9]= 60;
  storage.wtime[10]= 60;
  storage.wtime[11]= 60;
  storage.wtime[12]= 60;
  storage.wtime[13]= 60;
  storage.wtime[14]= 60;
  storage.wtime[15]= 5;
  storage.serialnum = 11010900.00;
  storage.greenhousename[0] = 'E';
  storage.greenhousename[1] = 'n';
  storage.greenhousename[2] = 'v';
  storage.greenhousename[3] = '1';
  storage.greenhousename[4] = ' ';
  storage.greenhousename[5] = ' ';
  storage.greenhousename[6] = ' ';
  storage.greenhousename[7] = ' ';
  storage.greenhousename[8] = '\0';
  storage.greenhousename[9] = '\0';
  storage.greenhousename[10] = '\0';
  storage.greenhousename[11] = '\0';
  storage.greenhousename[12] = '\0';
  storage.greenhousename[13] = '\0';
  storage.greenhousename[14] = '\0';
  storage.greenhousename[15] = '\0';
  storage.greenhousename[16] = '\0';
  storage.greenhousename[17] = '\0';
  storage.greenhousename[18] = '\0';
  storage.greenhousename[19] = '\0';
  storage.greenhousename[20] = '\0';
  storage.greenhousename[21] = '\0';
  storage.greenhousename[22] = '\0';
  storage.greenhousename[23] = '\0';
  storage.greenhousename[24] = '\0';
  storage.l = 2.5;
  storage.ipmtime[0] = 22;
  storage.ipmtime[1] = 00;
  storage.ipmtime[2] = 22;
  storage.ipmtime[3] = 45;
  storage.irrigatetimes[0] = 2;
  storage.irrigatetimes[1] = 30;
  storage.irrigatetimes[2] = 5;
  storage.irrigatetimes[3] = 30;
  storage.irrigatetimes[4] = 8;
  storage.irrigatetimes[5] = 30;
  storage.irrigatetimes[6] = 11;
  storage.irrigatetimes[7] = 30;
  storage.irrigatetimes[8] = 14;
  storage.irrigatetimes[9] = 30;
  storage.irrigatetimes[10] = 17;
  storage.irrigatetimes[11] = 30;
  storage.irrigatetimes[12] = 20;
  storage.irrigatetimes[13] = 30;
  storage.irrigatetimes[14] = 23;
  storage.irrigatetimes[15] = 0;
  storage.irrigatebytime = 1;
  storage.autostart = 0;
  storage.senddata = 0;
  storage.alerttemp = 58;
  storage.timezone = -5;
  storage.a = 500;
  storage.l = 2.5;
  storage.sdayvar = 1;
  saveConfig();
  

  
  ok = loadConfig();
  dayvar = storage.sdayvar;
  
  
    lcd.createChar(1,c_select);
  //lcd.createChar(2,c_up);
  lcd.createChar(3,c_down);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("CEA-grower");
  lcd.setCursor(0,1);
  lcd.print("by AgrowSoft");
  wdt_reset (); 
  delay(3000);
  lcd.clear();
  lcd.print("Fodder Watering");
  lcd.setCursor(0,1);
  lcd.print("Controller");
  wdt_reset (); 
  delay(3000);
  lcd.clear();
  lcd.print("Version");
  lcd.setCursor(0,1);
  lcd.print("6.1.1.10");
  wdt_reset (); 
  delay(3000);
  lcd.clear();
  lcd.print("Serial Number");
  lcd.setCursor(0,1);
  lcd.print(storage.serialnum,0);
  wdt_reset (); 
  delay(3000);
  
  
  daychangeflag = 0;
  secondchangeflag = 0;
}

void loop()
{
  wdt_reset ();  
  buttonPressed=lcd.button();
   if(buttonPressed==KEYPAD_UP)
   {
     waitReleaseButton();
     if(menulevel > 1)
     {
      
       menulevel=menulevel-1;
     }
        }
   else if(buttonPressed==KEYPAD_DOWN)
   {
     waitReleaseButton();
     if(menulevel < 36)
     {
       menulevel=menulevel+1;
     }
 
   }
   else if(buttonPressed==KEYPAD_SELECT)
   {
     waitReleaseButton();
     if(menulevel == 2)
     {
       menulevel = 1;
       x = irrigate();
      
     }
 
   }
   else if(buttonPressed==KEYPAD_RIGHT)
   {
     waitReleaseButton();
     if(menulevel==3)
     {
       //hour
      
       hourvar = hour()+1;
       minutevar = minute();
       tm.Hour = hourvar;
       RTC.write(tm);
       if (RTC.read(tm)) 
       {
         setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
       }
       else
       {
         setTime(hourvar,minutevar,0,1,1,13);
       }
         
     }
     if(menulevel==4)
     {
       //minute
       hourvar = hour();
       minutevar = minute()+1;
       
       tm.Minute = minutevar;
       tm.Second = 0;
       RTC.write(tm);
       if (RTC.read(tm)) 
       {
         setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
       }
       else
       {
          setTime(hourvar,minutevar,0,1,1,13);
       }
         
     }
      if(menulevel > 4 && menulevel <19)
     {
       int j;
       j = menulevel - 5;
       storage.wtime[j]=storage.wtime[j]+5;
        saveConfig();
     }
      if(menulevel == 19)
     {
       if(dayvar<7)
       {  
         dayvar = dayvar+1;
         storage.sdayvar = dayvar;
          saveConfig();
       }
     }  
       if(menulevel > 19 && menulevel <36)
       {
          int j;
          j = menulevel - 20;
          storage.irrigatetimes[j]=storage.irrigatetimes[j]+1;
          saveConfig();
       }
 
   }
   else if(buttonPressed==KEYPAD_LEFT)
   {
     waitReleaseButton();
     if(menulevel==3)
     {
       //hour
       hourvar = hour()-1;
       minutevar = minute();
       tm.Hour = hourvar;
       RTC.write(tm);
       if (RTC.read(tm)) 
       {
         setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
       }
       else
       {
         setTime(hourvar,minutevar,0,1,1,13);
       }
         
     }
     if(menulevel==4)
     {
       //hour
       hourvar = hour();
       minutevar = minute()-1;
        tm.Minute = minutevar;
        tm.Second = 0;
       RTC.write(tm);
       if (RTC.read(tm)) 
       {
         setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
       }
       else
       {
          setTime(hourvar,minutevar,0,1,1,13);
       }
         
     }
     if(menulevel > 4 && menulevel<19)
     {
       int j;
       j = menulevel - 5;
       if(storage.wtime[j] > 0)
       {
         storage.wtime[j]=storage.wtime[j]-5;
          saveConfig();
       }
     }
      if(menulevel == 19)
     {
       if(dayvar>1)
       {
         dayvar = dayvar-1;
          storage.sdayvar = dayvar;
          saveConfig();
       }  
     }
      if(menulevel > 19 && menulevel <36)
       {
          int j;
          j = menulevel - 20;
          storage.irrigatetimes[j]=storage.irrigatetimes[j]-1;
          saveConfig();
       }

     
  
   }
   
    
    
    currentMillis = millis();
 
    if(currentMillis - previousMillis >= interval) 
    {
      // save the last time  
      previousMillis = currentMillis;
        //update time
     x = checktime();
     
      if(menulevel == 1)
     {
       if(alternate <=3)
        {
           lcd.clear();
           lcd.print("Status: Running");
           lcd.setCursor(0,1);
           if(irrigating == 1)
           {
             lcd.write("Irrigating");
           }
           else
           {
            lcd.print("Time ");
             lcd.print(hour());
            lcd.print(":");
            int digits = minute();
            if(digits < 10)
              lcd.print('0');
            lcd.print(minute());
          lcd.print(":");
          digits = second();
            if(digits < 10)
              lcd.print('0');
          lcd.print(second());
          //lcd.print(" ");
          //lcd.print(month());
          //lcd.print("/");
          //lcd.print(day());
          //lcd.print("/");
          //lcd.print(year());
  
             //lcd.print(accumlight, 4);
           }
           alternate = alternate + 1;
        }
        
        else if(alternate <=8)
        {
            lcd.clear();
           lcd.print("Status: Running");
           lcd.setCursor(0,1);
           if(irrigating == 1)
           {
             lcd.write("Irrigating");
           }
           else
           {
             lcd.print("System Day: ");
             lcd.print(dayvar);
           }
           alternate = alternate + 1;
           
        }
        else if(alternate <=11)
        {
            lcd.clear();
           lcd.print("To Edit Settings");
           lcd.setCursor(0,1);
           lcd.print("Use Down Button ");
           
           alternate = alternate +1 ;
           
        }
        else if (alternate >=12)
        {
          lcd.clear();
           
           lcd.print("To Edit Settings");
           lcd.setCursor(0,1);
           lcd.print("Use Down Button ");
            
           alternate = 1;
        }
      }
      if(menulevel == 2)
      {
        lcd.clear();
        lcd.print("Irrigate NOW?");
        lcd.setCursor(0,1);
        lcd.write(' ');
        lcd.write(1);
        lcd.write(' ');
        lcd.write("Press Select"); 
        
       
          
      }
      
      if(menulevel == 3)
      {
        lcd.clear();
        lcd.print("System Hour:");
        lcd.setCursor(0,1);
        lcd.print(hour());
        lcd.write(' ');
        //lcd.write("moles");
       
          
      }
      if(menulevel == 4)
      {
        lcd.clear();
        lcd.print("System Minute:");
        lcd.setCursor(0,1);
        lcd.print(minute());
        lcd.write(' ');
        //lcd.write("moles");
       
          
      }
      if(menulevel == 5)
      {
        lcd.clear();
        lcd.print("R1 Day1 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[0],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 6)
      {
       lcd.clear();
        lcd.print("R1 Day2 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[1],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 7)
      {
       lcd.clear();
        lcd.print("R1 Day3 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[2],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 8)
      {
       lcd.clear();
        lcd.print("R1 Day4 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[3],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 9)
      {
       lcd.clear();
        lcd.print("R1 Day5 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[4],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 10)
      {
       lcd.clear();
        lcd.print("R1 Day6 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[5],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 11)
      {
       lcd.clear();
        lcd.print("R1 Day7 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[6],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 12)
      {
       lcd.clear();
        lcd.print("R2 Day1 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[7],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 13)
      {
       lcd.clear();
        lcd.print("R2 Day2 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[8],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 14)
      {
       lcd.clear();
        lcd.print("R2 Day3 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[9],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 15)
      {
       lcd.clear();
        lcd.print("R2 Day4 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[10],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 16)
      {
       lcd.clear();
        lcd.print("R2 Day5 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[11],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 17)
      {
       lcd.clear();
        lcd.print("R2 Day6 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[12],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 18)
      {
       lcd.clear();
        lcd.print("R2 Day7 On Time:");
        lcd.setCursor(0,1);
        lcd.print(storage.wtime[13],0);
        lcd.write(' ');
        lcd.write("seconds");
      }
      if(menulevel == 19)
      {
       lcd.clear();
        lcd.print("System Day");
        lcd.setCursor(0,1);
        lcd.write("Number: ");
        lcd.print(dayvar);
        
        
      }
      if(menulevel == 20)
      {
       lcd.clear();
        lcd.print("Cycle 1 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[0]);
        
        
      }
      if(menulevel == 21)
      {
       lcd.clear();
        lcd.print("Cycle 1 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[1]);
        
        
      }
      if(menulevel == 22)
      {
       lcd.clear();
        lcd.print("Cycle 2 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[2]);
        
        
      }
      if(menulevel == 23)
      {
       lcd.clear();
        lcd.print("Cycle 2 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[3]);
        
        
      }
      if(menulevel == 24)
      {
       lcd.clear();
        lcd.print("Cycle 3 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[4]);
        
        
      }
      if(menulevel == 25)
      {
       lcd.clear();
        lcd.print("Cycle 3 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[5]);
        
        
      }
      if(menulevel == 26)
      {
       lcd.clear();
        lcd.print("Cycle 4 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[6]);
        
        
      }
       if(menulevel == 27)
      {
       lcd.clear();
        lcd.print("Cycle 4 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[7]);
        
        
      }
       if(menulevel == 28)
      {
       lcd.clear();
        lcd.print("Cycle 5 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[8]);
        
        
      }
        if(menulevel == 29)
      {
       lcd.clear();
        lcd.print("Cycle 5 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[9]);
        
        
      }
        if(menulevel == 30)
      {
       lcd.clear();
        lcd.print("Cycle 6 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[10]);
        
        
      }
        if(menulevel == 31)
      {
       lcd.clear();
        lcd.print("Cycle 6 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[11]);
        
        
      }
        if(menulevel == 32)
      {
       lcd.clear();
        lcd.print("Cycle 7 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[12]);
        
        
      }
        if(menulevel == 33)
      {
       lcd.clear();
        lcd.print("Cycle 7 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[13]);
        
        
      }
        if(menulevel == 34)
      {
       lcd.clear();
        lcd.print("Cycle 8 On Time");
        lcd.setCursor(0,1);
        lcd.write("Hour: ");
        lcd.print(storage.irrigatetimes[14]);
        
        
      }
        if(menulevel == 35)
      {
       lcd.clear();
        lcd.print("Cycle 8 On Time");
        lcd.setCursor(0,1);
        lcd.write("Minute: ");
        lcd.print(storage.irrigatetimes[15]);
        
        
      }
      //Serial.println(F("read sensors"));
     // x = readsensors();
      
     
       // if(accumlight >= storage.l) 
     //   {
      //  
       
    //      if (irrigating == 0)
    //      {
        
    //       x = irrigate();
    //      }
    //     }
        //check for irrgation times.
    if(menulevel<2)
    {
         for (int counter = 0; counter < 15; counter = counter +2) 
         { 
            if (storage.irrigatetimes[counter] == hour())
            {
              //Serial.println(storage.irrigatetimes[counter]);
                //Serial.println(hour());
                //Serial.println(storage.irrigatetimes[counter+1]);
                //Serial.println(minute());
              if (storage.irrigatetimes[counter+1] == minute())
              {
                
                if(irrigating == 0)
                {
                  //Serial.println(second());
                  if(3 - second() > 0)
                  {
                    x = irrigate();            
                  }  
                 }   
                }
              }
         }  
    } 
    
  
  
  
} 



 
}
int readsensors(){
  // read light values
  // read light values
  //Serial.println(F("light calc"));
  //uint32_t lum = tsl.getFullLuminosity();
  //uint16_t ir, full;
  //ir = lum >> 16;
  //full = lum & 0xFFFF;
  ////Serial.print(F("IR: ")); //Serial.print(ir);   //Serial.print(F("\t\t"));
  ////Serial.print(F("Full: ")); //Serial.print(full);   //Serial.print(F("\t"));
  ////Serial.print(F("Visible: ")); //Serial.print(full - ir);   //Serial.print(F("\t"));
  
  ////Serial.print(F("Lux: "));//Serial.println(tsl.calculateLux(full, ir));
  
  //lightcalc = tsl.calculateLux(full, ir);
  //Serial.println(lightcalc);
  ////Serial.print(F("Lux: "));//Serial.println(lightcalc);
  
  //float lightcalc2;
  //lightcalc2 = lightcalc*.0185/86400*.1;
  //float ltemp;
  //float ltemp2;
  //ltemp= accumlight;
  //ltemp2 = accumlightdaily;
  //accumlight = ltemp + lightcalc2;
  //if (lightcalc > 10)
  //{
  //  if(daychangeflag==0)
  //  {
  //    if(dayvar < 8)
  //    {
  //      dayvar = dayvar +1;
  //      daychangeflag = 1;
  //    }
  //    else
  //    {
  //      dayvar = 1;
  //      daychangeflag = 1;
  //  }
  //} 
        
    //accumlightdaily = ltemp2 + lightcalc2;
  //}
  //else
  //{
  //  accumlightdaily = 0;
  //  daychangeflag = 0;
  //}
  
 // //Serial.print(F("Accum Moles: ")); //Serial.println(accumlight);
  //return 1;

}

int checktime()
{
  if(minute()== 59)
        {
          if(6 - second() > 0)
          {
             if(secondchangeflag == 0)
             {
               if (RTC.read(tm)) 
               {
                 setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
               }
               //int thehour = hour();
               //int thesecond = second()+2;
               //setTime(thehour,59,thesecond,1,1,13); // set time to Saturday 8:29:00am Jan 1 2011
               secondchangeflag = 1;
              }
              
           }
           else
           {
             secondchangeflag = 0;
           }
        }
        
        
        //check for day increment.
           if (hour()==0)
            {
              if (minute()==0)
              {
                if(3 - second() > 0)
                {
                    if(daychangeflag==0)
                    {
                      
                      
                      if(dayvar < 7)
                      {
                        dayvar = dayvar +1;
                        storage.sdayvar = dayvar;
                        saveConfig();
                        daychangeflag = 1;
                      }
                      else
                      {
                        dayvar = 1;
                        storage.sdayvar = dayvar;
                        saveConfig();
                        daychangeflag = 1;
                      }
                    }
                    
                 } 
                 else
                 {
                   daychangeflag = 0;
                 }              
               } 
               
              }   
        return 1;
  
}

int irrigate(){
  int var;
  float vartime;
  int long starttime;
  int long endtime;
  int long sensorstarttime;
  int long sensorendtime;
  
  irrigating = 1;
  lcd.clear();
  lcd.print("Status: Running");
  lcd.setCursor(0,1);
  if(irrigating == 1)
  {
      lcd.write("Irrigating");
   }
   //todo make this a loop
   //relay 16 on
  pinMode(39, OUTPUT);
  digitalWrite(39, LOW);
  starttime = millis();
  endtime = starttime;
  while ((endtime - starttime) <=1000) // do this loop for up to 1000mS
  {
      // wait
     endtime = millis();
    
  }
  //w1 on
  
  
  sensorstarttime = millis();
  sensorendtime = sensorstarttime;
  
  for(int i=0;i<14;i=i+1)
  {
      int ontimevar;
      //find on time
      
      //correct code
      if(i<7)
      {
        if((dayvar-i-1) >= 0)
        {
           ontimevar=(dayvar-i) - 1;
        }
        else
        {
          ontimevar=(dayvar-i) +6;
        }
      }
      else
      {
        //i 7 to 13, zones 8 to 14
        if((dayvar+7-i-1) >= 0)
        {
           ontimevar=(dayvar+7-i) - 1 + 7;
        }
        else
        {
          ontimevar=(dayvar+7-i) +6+7;
        }
      }
       
       
      vartime = storage.wtime[ontimevar]*1000;
      starttime = millis();
      endtime = starttime;
      lcd.clear();
      lcd.print("Irrigating:");
      lcd.setCursor(0,1);
      lcd.write("Zone ");
      int zone = i+1;
      lcd.print(zone);
      while ((endtime - starttime) <=vartime) // do this loop for up to 1000mS
      {
        wdt_reset (); 
        // code here
        pinMode(i+24, INPUT);
        var = digitalRead(i+24);
        if(var == 1)
        {
          //turn relay on: set pin to high
        
      
          ////Serial.print(F("W1: On "));
          pinMode(i+24, OUTPUT);
          digitalWrite(i+24, LOW);
        
        }
        endtime = millis();
        sensorendtime = millis();
        if ((sensorendtime - sensorstarttime) >= 1000)
        {
          //readsensors();
          x= checktime();
          sensorstarttime = millis();
        }
      }
      //w1 off
      ////Serial.print(F("W1: Off "));
  
      pinMode(i+24, OUTPUT);
      digitalWrite(i+24, HIGH);
  
      //wait 10 seconds
      starttime = millis();
      endtime = starttime;
      while ((endtime - starttime) <=1000) // do this loop for up to 1000mS
      {
        // wait
        endtime = millis();
    
      }
  }
  //turn 16 off
  pinMode(39, OUTPUT);
  digitalWrite(39, HIGH);
  accumlight = 0;
  irrigating = 0;
  return 1;
}


void waitReleaseButton()
{
  delay(50);
  while(lcd.button()!=KEYPAD_NONE)
  {
  }
  delay(50);
}

int getTimeAndDate() {
   int flag=0;
  
   return flag;
}



void stopSketch(void)
     {
      noInterrupts();
      while(1) {}
     }

