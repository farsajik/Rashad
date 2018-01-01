#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include "AsyncPing.h"
#include <BlynkSimpleEsp8266.h>
#include <Button.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Sunrise.h>

BlynkTimer timer;
AsyncPing Pings;
WidgetRTC rtc;
Sunrise mySunrise(40.24, 49.52, 4);  // lat - lon - time zone

int pingCheck = false;
int blynkConnected = false;

char ssid[]   = "RH";
char pass[]   = "0703507541";
char auth[]   = "efca4a3aa2474be9ad1a9369eeaeb675";
char server[] = "45.55.96.146";

WidgetLED Sts_Led  (V0);
byte Sts;

BLYNK_CONNECTED() {
  Blynk.syncAll();
  rtc.begin();
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
byte conn_ind = D4;  // connection indicator

Button right_sw = Button(D0, HIGH);
Button left_sw = Button(D5, HIGH);
Button wc_sw = Button(D6, HIGH);
Button kitchen_sw = Button(D7, HIGH);
Button guest_sw = Button(D8, HIGH);
byte equal = 0;

String  minutes, sunRise, sunSet;
int yy, hh, mm, srh, srm, ssh, ssm, t;  

void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.config(auth, server);
  
  timer.setInterval(5000, check_Connection);  
  timer.setInterval(1000, sts);
  timer.setInterval(300, btns);
  timer.setInterval(500, update_blynk); 
  timer.setInterval(30000L, clockDisplay);
  timer.setInterval(20000L, clock_lights);
  
  right_sw.setHoldDelay(2000);
  
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  mySunrise.Actual(); //Actual, Civil, Nautical, Astronomical
  
  pinMode(HallLGHT, OUTPUT);
  pinMode(DoorLGHT, OUTPUT);
  pinMode(WallLGHT, OUTPUT);
  pinMode(conn_ind, OUTPUT);
  
  digitalWrite(HallLGHT,LOW);
  digitalWrite(DoorLGHT,LOW);
  digitalWrite(WallLGHT,LOW);

}

/***********************************************************/  

void sts(){
   Sts = !Sts;
   if(Sts == 1) { Sts_Led.on(); }else{ Sts_Led.off(); }
   
/***********************************************/
     if (WiFi.status() == WL_CONNECTED )  
    { 
       if (blynkConnected == false )  
        { 
          digitalWrite( conn_ind , Sts);
        }
  
       if (blynkConnected == true )  
        { 
        digitalWrite( conn_ind , HIGH);
        }
    }else{
      digitalWrite( conn_ind , LOW);
  }//else
/***********************************************/
} //Sts

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
  
/******************************************************************************************************************************************************************************/ 
void clockDisplay()
{
    if ( minute() < 10 ) { minutes = String("0") + minute(); }else{ minutes = String(minute()); }   
/***********************************/ 
    t = mySunrise.Rise(month(), day()); // (month,day) 
    srh = int(mySunrise.Hour());
    srm = int(mySunrise.Minute()-2);
    if( srm < 10 ){ sunRise = String(srh) + ":0" + srm ; }else{ sunRise = String(srh) + ":" + srm ; }

/************************************/
    t = mySunrise.Set(month(), day()); // (month,day) 
    ssh = int( mySunrise.Hour());
    ssm = int( mySunrise.Minute()+2);
    if(ssm < 10){ sunSet = String(ssh) + ":0" + ssm ; }else{ sunSet = String(ssh) + ":" + ssm ; }
   
/***********************************/ 
  String currentTime = String(hour()) + ":" + minutes + "   "+ day() + "-" + month() + "-" + year() + "   SR-" + sunRise + "   SS-" + sunSet ;
  Serial.println("Current time: " + currentTime);
  Blynk.virtualWrite(V4, currentTime);

}//clockDisplay
/******************************************************************************************************************************************************************************/ 

void clock_lights()
{
   hh = hour();
   mm = minute();
   yy = year(); 
      
    if( yy !=1970){

           if( hh==srh && mm==srm ){
                Door_Lght_State = LOW;
                Wall_Lght_State = LOW;
                digitalWrite(DoorLGHT,Door_Lght_State);
                digitalWrite(WallLGHT,Wall_Lght_State);
                //Blynk.notify("sunrise");
            }
            
          if( hh==ssh && mm == ssm ){
                Door_Lght_State = HIGH;
                Wall_Lght_State = HIGH;
                digitalWrite(DoorLGHT,Door_Lght_State);
                digitalWrite(WallLGHT,Wall_Lght_State);
                //Blynk.notify("sunset");
            }
            
          if( hh==23 && mm==59 ){
                Door_Lght_State = HIGH;
                Wall_Lght_State = LOW;
                digitalWrite(DoorLGHT,Door_Lght_State);
                digitalWrite(WallLGHT,Wall_Lght_State);
                //Blynk.notify("twele oclock");
            }

      }//1970            
  }//clock_lights
/**************************************************************************/

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


