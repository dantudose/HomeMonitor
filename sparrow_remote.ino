#include "SparrowTransfer.h"
#include <Wire.h>
#include <SHT2x.h>

#define DEBUG 1

//create object
SparrowTransfer ST; 
 
struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER SPARROW
  uint8_t presence;
  float temperature;
};
 
//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

int redLedPin = 8;              // red LED
int greenLedPin = 11;           // green LED
int inputPin = 18;              // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int controlPin = 7;             // sensor on/off control
 
uint8_t detect(){
  int val = digitalRead(inputPin);  // read PIR input value
  if (val == HIGH) {            // check if the input is HIGH
      digitalWrite(redLedPin, LOW);  // turn LED ON
      return 1;
    }
  else {
      digitalWrite(redLedPin, HIGH); // turn LED OFF
      return 0;
    }
}

void blinkLED() //blinks the green LED
{
  digitalWrite(greenLedPin,LOW);
  delay(20);
  digitalWrite(greenLedPin,HIGH);  
}
 
void setup(){
 
  pinMode(greenLedPin, OUTPUT);      // declare green LED as output
  pinMode(redLedPin, OUTPUT);        // declare red LED as output
  pinMode(inputPin, INPUT);          // declare sensor as input
  pinMode(controlPin, OUTPUT);       // declare sensor control pin as output

  digitalWrite(controlPin, LOW);     // turn on sensors
  delay(1000);
  Wire.begin();
  
  //start the library, pass in the data details
  ST.begin(details(mydata));
  mydata.presence = 0;
  mydata.temperature = -1.23; //default value to test if something is wrong
 
  digitalWrite(greenLedPin, HIGH); //start with all LEDs off
  digitalWrite(redLedPin, HIGH); 
  
  #ifdef DEBUG //only for local debugging purposes
    Serial.begin(9600);
  #endif

}
 
void loop(){
 
  mydata.presence = detect();
  mydata.temperature = SHT2x.GetTemperature();

  #ifdef DEBUG
    Serial.println(mydata.presence);
    Serial.println(mydata.temperature);
  #endif
  
  ST.sendData();
  blinkLED();
  delay(1000);
}
