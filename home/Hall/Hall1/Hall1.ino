unsigned long last1;  // Button Left
unsigned long last2;  // Button Right
unsigned long last3;  // Button Right
unsigned long last4;  // Button Right
unsigned long last5;  // Button Right

byte Hall_Lght_State;
byte Door_Lght_State;
byte Wall_Lght_State;

byte HallLGHT = 7;  //  left
byte DoorLGHT = 6;  //  right 
byte WallLGHT = 5;  //  single

const byte  LeftSW = 13;   
long buttonTimer = 0;
long longPressTime = 10000;
boolean buttonActive = false;
boolean longPressActive = false;

const byte  RightSW = 14;   
long buttonTimer2 = 0;
long longPressTime2 = 1000;
boolean buttonActive2 = false;
boolean longPressActive2 = false;

const byte  WC_SW = 17; 
byte        buttonState1 = 0;         // current state of the button
byte        lastButtonState1 = 0;

const byte  Guest_Room_SW = 18; 
byte        buttonState2 = 0;         // current state of the button
byte        lastButtonState2 = 0;

const byte  Kitchen_Room_SW = 19; 
byte        buttonState3 = 0;         // current state of the button
byte        lastButtonState3 = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(HallLGHT, OUTPUT);
  pinMode(DoorLGHT, OUTPUT);
  pinMode(WallLGHT, OUTPUT);

  digitalWrite(HallLGHT,HIGH);
  digitalWrite(DoorLGHT,HIGH);
  digitalWrite(WallLGHT,HIGH);

  pinMode(LeftSW, INPUT);
  pinMode(RightSW, INPUT);
  pinMode(WC_SW, INPUT);
  pinMode(Guest_Room_SW, INPUT);
  pinMode(Kitchen_Room_SW, INPUT);

}


void loop() {
/*************************** Hall Light  *****************************************/
 if (millis() - last1 > 50) { 
  
      if (digitalRead(LeftSW) == LOW) {

        if (buttonActive == false) {
         buttonActive = true;
         buttonTimer = millis();
        }

         if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
          longPressActive = true;              
      
            //   Long Press
            
         } 

        } else {

          if (buttonActive == true) {                
          if (longPressActive == true) {
          longPressActive = false;
        } else {

            //   Short Press
           
           Hall_Lght_State = !Hall_Lght_State;
           Update_Hall_Lght( Hall_Lght_State );
           send();
           

        }
        buttonActive = false;
      }
    }

   last1 = millis(); 
 } //timer


 /**********************  - WC button Hall ***************************************/ 
if (millis() - last2 > 50) { 
  buttonState1 = digitalRead(WC_SW);   
  if (buttonState1 != lastButtonState1) {
    
      Hall_Lght_State = !Hall_Lght_State;
      Update_Hall_Lght( Hall_Lght_State );
      send();
  }
  lastButtonState1 = buttonState1;
  last2 = millis(); 
}//millis

/*************************** Wall and Door Lights  *****************************************/
 if (millis() - last3 > 50) { 
  
   if (digitalRead(RightSW) == LOW) {

      if (buttonActive2 == false) {
          buttonActive2 = true;
          buttonTimer2 = millis();
       }

      if ((millis() - buttonTimer2 > longPressTime2) && (longPressActive2 == false)) {
         longPressActive2 = true;              
      
          //   Long Press         
          
          Door_Lght_State = !Door_Lght_State;
          Update_Door_Lght( Door_Lght_State);
          send();
     
      }

    } else {

      if (buttonActive2 == true) {                
          if (longPressActive2 == true) {
              longPressActive2 = false;
          } else {

             //   Short Press
            
             Door_Lght_State = !Door_Lght_State;
             Wall_Lght_State = !Wall_Lght_State;

             Update_Door_Lght( Door_Lght_State);
             Update_Wall_Lght( Wall_Lght_State);
             send();
             
          }
          buttonActive2 = false;
       }
    }

  last3 = millis(); 
 }

 /**********************  Guest_Room_SW ***************************************/ 
if (millis() - last4 > 50) { 
  buttonState2 = digitalRead(Guest_Room_SW);   
  if (buttonState2 != lastButtonState2) {
    
       Door_Lght_State = !Door_Lght_State;
       Wall_Lght_State = !Wall_Lght_State;

       Update_Door_Lght( Door_Lght_State);
       Update_Wall_Lght( Wall_Lght_State);
       send();
  }
  lastButtonState2 = buttonState2;
  last4 = millis(); 
}//millis

 /**********************  Kitchen_Room_SW ***************************************/ 
if (millis() - last5 > 50) { 
  buttonState3 = digitalRead(Kitchen_Room_SW);   
  if (buttonState3 != lastButtonState3) {
    
       Door_Lght_State = !Door_Lght_State;
       Wall_Lght_State = !Wall_Lght_State;

       Update_Door_Lght( Door_Lght_State);
       Update_Wall_Lght( Wall_Lght_State);
       send();
  }
  lastButtonState3 = buttonState3;
  last5 = millis(); 
}//millis

  
} // loop

/*******************************************/
void Update_Hall_Lght( byte state){
  
    if(state == 0){
        digitalWrite( HallLGHT , 1 );
      }else{
        digitalWrite( HallLGHT , 0 );
      }
  }

/*******************************************/
void Update_Door_Lght( byte state){
  
    if(state == 0){
        digitalWrite( DoorLGHT , 1 );
      }else{
        digitalWrite( DoorLGHT , 0 );
      }
  }

/*******************************************/
void Update_Wall_Lght( byte state){
  
    if(state == 0){
        digitalWrite( WallLGHT , 1 );
      }else{
        digitalWrite( WallLGHT , 0 );
      }
  }
/*******************************************/

void send(){
  Serial.print("Hall = ");
  Serial.println(Hall_Lght_State);
  Serial.print("Door = ");
  Serial.println(Door_Lght_State);
  Serial.print("Wall = ");
  Serial.println(Wall_Lght_State);
  }
