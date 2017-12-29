#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include "AsyncPing.h"
#include <BlynkSimpleEsp8266.h>
#include <Button.h>

BlynkTimer timer;
AsyncPing Pings;

int pingCheck = false;
int blynkConnected = false;

char ssid[]   = "";
char pass[]   = "";
char auth[]   = "";
char server[] = "";

WidgetLED Sts_Led  (V0);
int Sts;

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

byte Hall_Lght_State = 0;
byte Door_Lght_State = 0;
byte Wall_Lght_State = 0;

byte Wall_Lght_last_State = 0;
byte Door_Lght_last_State = 0;
byte Hall_Lght_last_State = 0;

byte HallLGHT = D1;  //  left
byte DoorLGHT = D3;  //  right 
byte WallLGHT = D2;  //  single

Button right_sw = Button(D0, HIGH);
Button left_sw = Button(D5, HIGH);
Button wc_sw = Button(D6, HIGH);
Button kitchen_sw = Button(D7, HIGH);
Button guest_sw = Button(D8, HIGH);

byte equal = 0;

void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.config(auth, server);
  checkBlynk();
  
  timer.setInterval(6000, checkBlynk);  
  timer.setInterval(1000, sts);
  timer.setInterval(300, btns);
  timer.setInterval(500, update_blynk); 
 
  right_sw.setHoldDelay(2000);
  
  pinMode(HallLGHT, OUTPUT);
  pinMode(DoorLGHT, OUTPUT);
  pinMode(WallLGHT, OUTPUT);
  pinMode(D4, OUTPUT);
  
  digitalWrite(HallLGHT,LOW);
  digitalWrite(DoorLGHT,LOW);
  digitalWrite(WallLGHT,LOW);

}

/***********************************************************/  

void sts(){
   Sts = !Sts;
   if(Sts == 1) { Sts_Led.on(); }else{ Sts_Led.off(); }
   digitalWrite(D4,Sts);
} //Sts

/***********************************************************/ 

void checkBlynk() {
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

void btns(){
/*************************************/
 left_sw.listen();
  
 if (left_sw.onRelease()){
     //Serial.println("left");
     Hall_Lght_State = !Hall_Lght_State;
     digitalWrite( HallLGHT , Hall_Lght_State );
 }
/*************************************/
 right_sw.listen();
  
 if (right_sw.onRelease()){
     //Serial.println("right");
      equal = !equal;
      Door_Lght_State = equal;
      Wall_Lght_State = equal;
      digitalWrite( DoorLGHT , Door_Lght_State );
      digitalWrite( WallLGHT , Wall_Lght_State );
 }

 if (right_sw.onHold()){
     //Serial.println("hold");
     Door_Lght_State = !Door_Lght_State;
     digitalWrite( DoorLGHT , Door_Lght_State );
 }
 /*************************************/
 wc_sw.listen();
  
 if (wc_sw.onChange()){
     //Serial.println("change");
     Hall_Lght_State = !Hall_Lght_State;
     digitalWrite( HallLGHT , Hall_Lght_State );
 }
/*************************************/
 kitchen_sw.listen();
  
 if (kitchen_sw.onChange()){
     //Serial.println("kitchen");
      equal = !equal;
      Door_Lght_State = equal;
      Wall_Lght_State = equal;
      digitalWrite( DoorLGHT , Door_Lght_State );
      digitalWrite( WallLGHT , Wall_Lght_State );
 }
/*************************************/
 guest_sw.listen();
  
 if (guest_sw.onChange()){
     //Serial.println("guest");
      equal = !equal;
      Door_Lght_State = equal;
      Wall_Lght_State = equal;
      digitalWrite( DoorLGHT , Door_Lght_State );
      digitalWrite( WallLGHT , Wall_Lght_State );
 }
/*************************************/
}//btns

/**************** Coming datas from Blynk App ******************/
BLYNK_WRITE(V1)
{
  Wall_Lght_State = param.asInt(); 
  digitalWrite( WallLGHT , Wall_Lght_State );
}

BLYNK_WRITE(V2)
{
  Door_Lght_State = param.asInt();
  digitalWrite( DoorLGHT , Door_Lght_State ); 
}

BLYNK_WRITE(V3)
{
  Hall_Lght_State = param.asInt(); 
  digitalWrite( HallLGHT , Hall_Lght_State );
}

/**************************************************************/
void update_blynk(){
    
    if(Wall_Lght_State!=Wall_Lght_last_State){
       Blynk.virtualWrite(V1, Wall_Lght_State);
       Wall_Lght_last_State = Wall_Lght_State;
    }

     if(Door_Lght_State!=Door_Lght_last_State){
       Blynk.virtualWrite(V2, Door_Lght_State);
       Door_Lght_last_State = Door_Lght_State;
    }
    

    if(Hall_Lght_State!=Hall_Lght_last_State){
      Blynk.virtualWrite(V3, Hall_Lght_State);
      Hall_Lght_last_State = Hall_Lght_State;
    }
    
  } //update_blynk()
/**************************************************************/

  
void loop() {

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


