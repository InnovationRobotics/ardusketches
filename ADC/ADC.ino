/* 
 * rosserial ADC Example
 * 
 * This is a poor man's Oscilloscope.  It does not have the sampling 
 * rate or accuracy of a commerical scope, but it is great to get
 * an analog value into ROS in a pinch.
 */

// rosserial
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
#include <rosserial_arduino/Adc.h>
ros::NodeHandle nh;
rosserial_arduino::Adc adc_msg;
ros::Publisher p("adc", &adc_msg);

// position
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>



// position
double Pitch = 0, Roll = 0, Yaw = 0;
// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board
//uint16_t PRINT_DELAY_MS = 100; // how often to print the data
//uint16_t printCount = 0; //counter to avoid printing every 10MS sample

void setup()
{ 


// rosserial
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.advertise(p);
//  Serial.begin(115200);
// position
  
 if (!bno.begin())
  {
//  Serial.print("No BNO055 detected");
   while (1);
  }
  delay(1000);
}

//We average the analog reading to elminate some of the noise
int averageAnalog(int pin){
  int v=0;
  for(int i=0; i<4; i++) v+= analogRead(pin);
  return v/4;
}

long adc_timer;

void loop()
{
// position
  unsigned long tStart = micros();
  sensors_event_t orientationData;// , linearAccelData ,angVelData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  Pitch = orientationData.orientation.z;
  Roll = orientationData.orientation.y;
  Yaw = orientationData.orientation.x;
  while ((micros() - tStart) < (BNO055_SAMPLERATE_DELAY_MS * 1000))
  {
    ;//poll until the next sample is ready
  }

// rosserial
  adc_msg.adc0 = averageAnalog(0);
  adc_msg.adc1 = averageAnalog(1);
  adc_msg.adc2 = (int)(Pitch*100.0);//averageAnalog(2);
  adc_msg.adc3 = (int)(Roll*100.0);//averageAnalog(3);
  adc_msg.adc4 = (int)(Yaw*100.0);//averageAnalog(4);
  adc_msg.adc5 = 0;//averageAnalog(5);
  p.publish(&adc_msg);
  
  nh.spinOnce();
  delay(10);
}
