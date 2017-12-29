#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include "AsyncPing.h"
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

BlynkTimer timer;
AsyncPing Pings;
int pingCheck = false;
int blynkConnected = false;

char ssid[]   = "";
char pass[]   = "";
char auth[]   = "";
char server[] = "45.55.96.146";

WidgetLED Sts_Led  (V0);
WidgetLED gas_led  (V3);
int Sts;
int Led;
 
#define brd_ind D5  //connection status

#define kombi_relay D6 
int kombi_relay_state = LOW ; //default kombi off

#define gas_sensor D2
#define gas_relay  D7 
#define button  D1
int addr=0;  // eeprom adress

byte timer_state = 0;  //timer on
long buttonTimer = 0;
boolean buttonActive = false;
boolean longPressActive = false;

/**********************************************************************************/
#define ONE_WIRE_BUS D3      // ds18b20
float old_tempC = 5;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//DeviceAddress Thermometer1 = {0x28, 0xDE, 0x33, 0x23, 0x05, 0x00, 0x00, 0xCE}; 
DeviceAddress Thermometer1 = {0x28, 0x11, 0x59, 0x22, 0x05, 0x00, 0x00, 0x56};
/**********************************************************************************/
BLYNK_CONNECTED() {
  Blynk.syncAll();
}


void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  WiFi.begin(ssid, pass);
  Blynk.config(auth, server);
  
  timer.setInterval(5000, check_Connection); 
  timer.setInterval(1000, sts); 
  timer.setInterval(2000, get_temp);
  timer.setInterval(2000, det_gas);   
  timer.setInterval(300,  btn_timer);
/***********************************************************/
  pinMode (brd_ind , OUTPUT);      //  board indication led
 
  pinMode (kombi_relay , OUTPUT);     
  digitalWrite (kombi_relay , LOW);    // for relay HIGH 
  
  sensors.begin();
  sensors.setResolution(Thermometer1, 10);
  
  pinMode(gas_relay, OUTPUT);
  pinMode(button, INPUT);
  pinMode(gas_sensor, INPUT);
  timer_state = EEPROM.read(addr);
  digitalWrite (gas_relay ,timer_state);
 
} //setup

/***********************************************************/  

void sts(){
   Sts = !Sts;
   if(Sts == 1) { Sts_Led.on(); }else{ Sts_Led.off(); }
   if(timer_state == 1) { gas_led.on(); }else{ gas_led.off(); }
   
/***********************************************/
     if (WiFi.status() == WL_CONNECTED )  
    { 
       if (blynkConnected == false )  
        { 
          digitalWrite( brd_ind , Sts);
        }
  
       if (blynkConnected == true )  
        { 
        digitalWrite( brd_ind , HIGH);
        }
    }else{
      digitalWrite( brd_ind , LOW);
  }//else
/***********************************************/    
} //Sts
/***********************************************************/ 

void get_temp(){
    sensors.requestTemperatures();
    float tempC = sensors.getTempC(Thermometer1);
    if ( old_tempC != tempC) {
      Blynk.virtualWrite(V2,tempC);
      //Serial.print("Temperature =  ");
      //Serial.print(tempC);
     // Serial.println(" C");
      old_tempC = tempC;
    }
  
 }//get temp
 /************ Coming datas from Blynk App ******************/
BLYNK_WRITE(V1)
{
  kombi_relay_state = param.asInt(); 
  digitalWrite ( kombi_relay ,kombi_relay_state); // ! for relay
  //Serial.println(Relay_State);
}
/***********************************************************/
 void det_gas(){
     if (timer_state == 0 )
     {    
        if (digitalRead(gas_sensor) == !HIGH ) {
            timer_state = 1; // timer off
            EEPROM.write(addr, 1);
            EEPROM.commit();
            digitalWrite (gas_relay ,1);
            Serial.println("gas detected");
            Blynk.notify("<<< QAZ SIZINTISI >>>");
            
        } 
   }//timer_state
}//det_gas
/***********************************************************/

void btn_timer(){
  if (digitalRead(button) == HIGH) {
    if (buttonActive == false) {
         buttonActive = true;
         buttonTimer = millis();
    }
    if ((millis() - buttonTimer > 4000) && (longPressActive == false)) {
      longPressActive = true;  // long press
      
           timer_state = 0; // timer on
           EEPROM.write(addr, 0);
           EEPROM.commit();
           digitalWrite (gas_relay ,0);
           Serial.println("Reset");

    }} else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {  // short press
        
        kombi_relay_state =!kombi_relay_state;
        digitalWrite(kombi_relay, kombi_relay_state);    // ! for relay
        Blynk.virtualWrite(V1, kombi_relay_state);

      } buttonActive = false;}

  }
 }//gas_reset
/***********************************************************/

void check_Connection() {
  if (WiFi.status() == WL_CONNECTED)  
  { 
/*******************************/
if (blynkConnected == false){ 
     Pings.begin("45.55.96.146");
     Pings.on(false,[](const AsyncPingResponse& response){
     if (response.total_recv > 0){
        pingCheck = true;
      }else{
         Serial.println("No Internet Connection");
         pingCheck = false;}
         return true;
      }); //ping   
}//blynk connected false
/*******************************/ 
 if (blynkConnected == false && pingCheck == true)
      {
         Blynk.connect();
      }   
/*******************************/ 
   }else{ Serial.println("NO WIFI");} //Wifi
}//check 
/***********************************************************/
 
void loop()
{
   if (Blynk.connected()) 
   { 
      Blynk.run();
      blynkConnected = true;
   } 
   else  
   {  
       blynkConnected = false; 
   }
  
  timer.run();

}
