#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

unsigned long last1;
unsigned long last2;
int Sts;

int Wall_Lght_State = 0;
int Door_Lght_State = 0;
int Hall_Lght_State = 0;

int Wall_Lght_last_State = 0;
int Door_Lght_last_State = 0;
int Hall_Lght_last_State = 0;

WidgetLED Sts_Led  (V0);

char auth[] = "40c4c0e7af5440d78706723b5bf2bfe9";  //ip
char ssid[] = "RH";
char pass[] = "0703507541";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
 
  Sts_Led.off();
}

/************************** Coming datas from Blynk App *************************************/
BLYNK_WRITE(V1)
{
  Wall_Lght_State = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V2)
{
  Door_Lght_State = param.asInt(); // assigning incoming value from pin V2 to a variable
}

BLYNK_WRITE(V3)
{
  Hall_Lght_State = param.asInt(); // assigning incoming value from pin V3 to a variable
}

/********************************************************************************************/

void update_nano (){
    String A = "A";
    
     if(Wall_Lght_State!=Wall_Lght_last_State){
    Serial.print(A+1+"V"+Wall_Lght_State+'n');
    delay(6);
    }
    Wall_Lght_last_State = Wall_Lght_State;

    if(Door_Lght_State!=Door_Lght_last_State){
    Serial.print(A+2+"V"+Door_Lght_State+'n');
    delay(6);
    }
    Door_Lght_last_State = Door_Lght_State;

    if(Hall_Lght_State!=Hall_Lght_last_State){
    Serial.print(A+3+"V"+Hall_Lght_State+'n');
    delay(6);
    }
    Hall_Lght_last_State = Hall_Lght_State;
  
 }
  
void loop()
{
  Blynk.run();
  
/***********************************************/
   if (millis() - last1 > 2000) { 
    update_nano();

    Sts = !Sts;
    if(Sts == 1) { Sts_Led.on(); }else{ Sts_Led.off(); }
    
    last1 = millis();
    
    }

/***********************************************/
 if (Serial.available() > 0) {
    String chrome = Serial.readStringUntil('n');
    int chromeLen = chrome.length();
    String pinRaw = chrome.substring(1,chrome.indexOf("V"));
    String valRaw = chrome.substring(chrome.indexOf("V")+1,chromeLen);
    int pin = int(pinRaw.toInt());
    int val = int(valRaw.toInt());
    
    /***************************/
    if( pin == 1){
         Wall_Lght_State = val;
         Wall_Lght_last_State = val; 

         if( Wall_Lght_State == 1 ){
           Blynk.virtualWrite(V1, val); 
         }else{
           Blynk.virtualWrite(V1, val); 
         }
      }
      
    /***************************/
     if( pin == 2){
         Door_Lght_State = val;
         Door_Lght_last_State = val;

         if( Door_Lght_State == 1 ){
           Blynk.virtualWrite(V2, val); 
         }else{
          Blynk.virtualWrite(V2, val); 
         }
      }
      
    /***************************/
     if( pin == 3){
       Hall_Lght_State = val;
       Hall_Lght_last_State = val; 

       if( Hall_Lght_State == 1 ){
         Blynk.virtualWrite(V3, val); 
       }else{
         Blynk.virtualWrite(V3, val); 
       }
    
     }
      
    /***************************/   
 }// serial
 
} //loop

