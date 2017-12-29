const byte  OUTPIN1 = 3;

const byte  buttonPin1 = 18;           // Left button
byte        buttonState1 = 0;          // current state of the button
byte        lastButtonState1 = 0;

const byte  buttonPin2 = 19;           // Right button 19
byte        buttonState2 = 0;          // current state of the button
byte        lastButtonState2 = 0 ;

const byte  buttonPin3 = 16;           // Released button
byte        buttonState3 = 0;          // current state of the button
byte        lastButtonState3 = 0;

unsigned long last1 = millis();
unsigned long last2 = millis();
unsigned long last3 = millis();

void setup() {
  pinMode(OUTPIN1, OUTPUT);
  digitalWrite(OUTPIN1,HIGH); //Relay OFF
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  lastButtonState1 = digitalRead(buttonPin1); 
  lastButtonState2 = digitalRead(buttonPin2); 
  lastButtonState3 = digitalRead(buttonPin3); 
 
}

void loop() {

/********************** Button1 - Left button *****************************/ 
if (millis() - last1 > 50) { 
  buttonState1 = digitalRead(buttonPin1);   
  
  if (buttonState1 != lastButtonState1) {
      digitalWrite(OUTPIN1, !digitalRead(OUTPIN1));  
  }
  
  lastButtonState1 = buttonState1;
  last1 = millis(); 
}//millis
  
/********************** Button2 - Right button *****************************/
if (millis() - last2 > 50) { 
  buttonState2 = digitalRead(buttonPin2);   
  
  if (buttonState2 != lastButtonState2) {
      digitalWrite(OUTPIN1, !digitalRead(OUTPIN1)); 
  }
  
  lastButtonState2 = buttonState2;
  last2 = millis(); 
}//millis

/********************** Button1 - Released button *****************************/ 
if (millis() - last3 > 50) { 
  buttonState3 = digitalRead(buttonPin3);   
  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {   // Button pressed         
      
    } else {                     // Button released
      
      digitalWrite(OUTPIN1, !digitalRead(OUTPIN1)); 
      
    }
  }
  lastButtonState3 = buttonState3;
  last3 = millis(); 
}//millis
 
/******************************************************************************/
}
