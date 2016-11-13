#include <Wire.h>
#include "SI1145.h"
#include <SHT2x.h>
#include <BaroSensor.h>
#include "SparrowTransfer.h"
 
//create object
SparrowTransfer ST; 
 
struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint8_t presence;
  float temperature;
 
};
//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

struct SENSOR_DATA{
  //we store here sensor readings
  float temperature;
  float humidity;
  float pressure;
  float light;
  float IR;
  float UV; 
  uint8_t remote_presence; 
  float remote_temp;
};

SENSOR_DATA node;

int controlPin = 7; // control pin for all sensors on the node
int greenLedPin = 11; // green LED

Adafruit_SI1145 uv = Adafruit_SI1145();

int getSensorData(){ //reads all sensors
  node.temperature = SHT2x.GetTemperature(); //temperature in degrees C
  node.humidity = SHT2x.GetHumidity(); //humidity in RH
  node.pressure = BaroSensor.getPressure(); //pressure in mbar
  node.light = uv.readVisible(); //visible light 
  node.IR = uv.readIR(); //infrared light level
  node.UV = uv.readUV() / 100.0; //UV index

  return 1;
}

void blinkLED() //blinks the green LED
{
  digitalWrite(greenLedPin,LOW);
  delay(20);
  digitalWrite(greenLedPin,HIGH);  
}

void sendSensorData(){ //sends sensor data over the serial interface as CSV
  Serial.print(node.temperature); //temperature in degrees C
  Serial.print(",");
  Serial.print(node.humidity); //humidity in RH
  Serial.print(",");
  Serial.print(node.pressure); //pressure in mbar
  Serial.print(",");
  Serial.print(node.light); //visible light 
  Serial.print(",");
  Serial.print(node.IR); //infrared light level
  Serial.print(",");
  Serial.print(node.UV); //UV index
  Serial.print(",");
  Serial.print(node.remote_presence); // presence from the remote sensor
  Serial.print(",");
  Serial.println(node.remote_temp); // temperature from the remote sensor
}

void uvInit(){ // initializes the Si1145 light sensor
  if (! uv.begin()) { //no luck first time, try to initialize again
    Serial.println("Didn't find Si1145");
    digitalWrite(controlPin, HIGH);
    delay(100);
    digitalWrite(controlPin, LOW);
    delay(1000);
    if (! uv.begin()) while(1); //the light sensor could not be initialized
  }
}

void setup() {

  //start the library, pass in the data details  
  ST.begin(details(mydata));
  
  pinMode(controlPin, OUTPUT);  //sensor on/off control
  pinMode(greenLedPin, OUTPUT);      // this is where the red LED on the node is connected
  
  digitalWrite(controlPin, LOW); // enable all sensors
  delay(1000); //wait for things to settle
  
  Serial.begin(9600);
  
  uvInit(); // init light sensor
  
  BaroSensor.begin(); // init barometric sensor
}


void loop() {
  if(ST.receiveData()){ //check remote sensor readings
    node.remote_presence = mydata.presence;
    node.remote_temp = mydata.temperature;
  }
  
  if(getSensorData()){
    sendSensorData();
    blinkLED();
  }
  delay(1000);

}
