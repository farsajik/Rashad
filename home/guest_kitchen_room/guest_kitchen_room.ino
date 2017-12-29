#include <IRremote.h>
byte RECV_PIN = 14; 

unsigned long last1;  // IR 1
unsigned long last2;  // IR 2
unsigned long last3;  // IR 3
unsigned long last4;  // IR 4
unsigned long last5;  // Button Left
unsigned long last6;  // Button Right

byte OUTPIN1 = 5;  //  left
byte OUTPIN2 = 4;  //  right 
byte OUTPIN3 = 6;  //  single
byte OUTPIN4 = 12; //  yard light

IRrecv irrecv(RECV_PIN);
decode_results results;


const byte  buttonPin1 = 16;   
long buttonTimer = 0;
long longPressTime = 1000;
boolean buttonActive = false;
boolean longPressActive = false;

const byte  buttonPin2 = 19;   
long buttonTimer2 = 0;
long longPressTime2 = 1000;
boolean buttonActive2 = false;
boolean longPressActive2 = false;

void setup()
{
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  pinMode(OUTPIN1, OUTPUT);
  pinMode(OUTPIN2, OUTPUT);
  pinMode(OUTPIN3, OUTPUT);
  pinMode(OUTPIN4, OUTPUT);
  digitalWrite(OUTPIN1,HIGH);
  digitalWrite(OUTPIN2,HIGH);
  digitalWrite(OUTPIN3,HIGH);
  digitalWrite(OUTPIN4,LOW);  //Pin13

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  
}


void loop() {
  if (irrecv.decode(&results)) {
    
/************************* RED *******************************/
  if (results.value == 0x80BFE31C) {    //0xFFA25D Small                                  
      if (millis() - last1 > 250) {
        //Serial.println("RED");
          digitalWrite(OUTPIN1, !digitalRead(OUTPIN1));               //output
       last1 = millis();
      }
     
/************************ GREEN *******************************/         
  }else if (results.value == 0x80BF639C) {    //0xFF629D                           
       if (millis() - last2 > 250) {
        //Serial.println("GREEN");
         digitalWrite(OUTPIN2, !digitalRead(OUTPIN2));                //output
        last2 = millis(); 
       }
          
/************************ YELLOW *******************************/         
  }else if (results.value == 0x80BFA35C) {   //0xFFE21D                            
       if (millis() - last3 > 250) {
        //Serial.println("YELLOW"); 
          digitalWrite(OUTPIN3, !digitalRead(OUTPIN3));                //output
        last3 = millis();   
       }
         
/************************ BLUE *******************************/         
  }else if (results.value == 0x80BF23DC) {    //0xFF22DD                            
       if (millis() - last4 > 250) {
        //Serial.println("BLUE");
          digitalWrite(OUTPIN4, !digitalRead(OUTPIN4));                //output
        last4 = millis();  
       }
          
  } 

/*************************************************************/          
    irrecv.resume(); // Receive the next value
  } //irrecv


/*************************** Left and Long Single Lights  *****************************************/
 if (millis() - last5 > 50) { 
  
      if (digitalRead(buttonPin1) == LOW) {

        if (buttonActive == false) {
         buttonActive = true;
         buttonTimer = millis();
        }

         if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
          longPressActive = true;              
      
            //   Long Press
            digitalWrite(OUTPIN3, !digitalRead(OUTPIN3));  // Hided  light ( single relay )

         } 

        } else {

          if (buttonActive == true) {                
          if (longPressActive == true) {
          longPressActive = false;
        } else {

            //   Short Press
           digitalWrite(OUTPIN1, !digitalRead(OUTPIN1));  // Left relay

        }
        buttonActive = false;
      }
    }

   last5 = millis(); 
 } //timer
/*************************** Right and Long Yard Lights  *****************************************/
 if (millis() - last6 > 50) { 
  
   if (digitalRead(buttonPin2) == LOW) {

      if (buttonActive2 == false) {
          buttonActive2 = true;
          buttonTimer2 = millis();
       }

      if ((millis() - buttonTimer2 > longPressTime2) && (longPressActive2 == false)) {
         longPressActive2 = true;              
      
          //   Long Press
          digitalWrite(OUTPIN4, !digitalRead(OUTPIN4));  // Right relay 

      }

    } else {

      if (buttonActive2 == true) {                
          if (longPressActive2 == true) {
              longPressActive2 = false;
          } else {

             //   Short Press
             digitalWrite(OUTPIN2, !digitalRead(OUTPIN2));  // Yard light

          }
          buttonActive2 = false;
       }
    }

  last6 = millis(); 
 }
  
} // loop



