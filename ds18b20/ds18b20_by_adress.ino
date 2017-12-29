#include <OneWire.h>
#include <DallasTemperature.h>
 
#define ONE_WIRE_BUS D2
float old_tempC = 5;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1 = {0x28, 0xDE, 0x33, 0x23, 0x05, 0x00, 0x00, 0xCE}; 
 
void setup() { 
  sensors.begin();
  sensors.setResolution(Thermometer1, 10);
  Serial.begin(9600);
}

 
void loop() { 
  
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(Thermometer1);
  if ( old_tempC != tempC) {
  Serial.print("Temperature =  ");
  Serial.print(tempC);
  Serial.println(" C");
  old_tempC = tempC;
  }
}
