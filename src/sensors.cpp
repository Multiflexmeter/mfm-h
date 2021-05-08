#include "sensors.h"

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <MedianFilter.h>
#include "debug.h"
#include "config.h"

static OneWire wire(PIN_ONE_WIRE);
static DallasTemperature wire_sensors(&wire);
DeviceAddress temp_sensor_addr;

SoftwareSerial JSN(PIN_JSN_TX, PIN_JSN_RX);

bool initialize_sensors(void)
{
  pinMode(PIN_JSN_SDN, OUTPUT);
  wire_sensors.begin();
  bool wireSuccess = wire_sensors.getAddress(temp_sensor_addr, 0);
  disable_sensors();

  // Find address for temp sensor
  if (!wireSuccess)
  {
    _debug(F("Could not find temperature sensor on index 0\r\n"));
    return false;
  }

  disable_sensors();
  return true;
}

void enable_sensors(void)
{
  digitalWrite(PIN_JSN_SDN, HIGH);
  JSN.begin(9600);
}

void disable_sensors(void)
{
  JSN.end();
  wire.reset();
  digitalWrite(PIN_JSN_SDN, LOW);
  digitalWrite(PIN_JSN_TX, LOW);
  digitalWrite(PIN_JSN_RX, LOW);
  digitalWrite(PIN_ONE_WIRE, LOW);
}

/**
 *  Perform an air temperature measurement and return the results
 */
float get_air_temperature(void)
{
  wire_sensors.requestTemperaturesByAddress(temp_sensor_addr);
  return wire_sensors.getTempC(temp_sensor_addr);
}

/**
 *  Perform a distance measurement and return the results
 */
uint16_t get_distance_to_water(void)
{
  uint8_t buffer[4];

  // Wait until there is data available or the timeout is reached
  uint32_t timeout_ms = millis() + SENSOR_JSN_TIMEOUT;
  bool frame_start = false;
  JSN.write(0x55);
  for (;;)
  {
    // Respect timeout
    if (millis() > timeout_ms)
    {
      _debug(F("JSN time out\r\n"));
      return -1;
    }

    // JSN response frame starts with 0xFF
    // if there is anything available then check if the first byte
    // is 0xFF, if not then discard it. Do this until the byte is 0xFF
    while (!frame_start && JSN.available() > 0)
    {
      if (JSN.peek() == 0xff)
      {
        frame_start = true;
      }
      else
      {
        JSN.read();
      }
    }
    if (frame_start && JSN.available() >= 4)
    {
      break;
    }
  }

  // Read the JSN data frame into the buffer
  JSN.readBytes(buffer, 4);

  // Verify the read data through its checksum
  uint8_t checksum = ((uint16_t)buffer[0] + (uint16_t)buffer[1] + (uint16_t)buffer[2]) & 0xff;
  if (checksum != buffer[3])
  {
    _debug(F("JSN checksum error\r\n"));
    return -2;
  }

  // Calculate distance and convert from mm to cm
  uint16_t distance = (buffer[1] << 8) | buffer[2];
  return distance / 10;
}


float get_distance_to_water_SEN0313 (){

  unsigned char data[4]={};
  float distance;

  //digitalWrite(TX,tempCorrect);
  delay(150);

  //clear buffer
  while(JSN.available() > 0) {
    char clear = JSN.read();
  }

  //wait for new measurement
  while(JSN.available() <= 0) {
    delay(2);
  }
  
  //read until startbyte 0xFF is received
  do{
    data[0]=JSN.read();
    //  Serial.print("data0:");
    //  Serial.println(data[0],HEX);
     delay(2);
  }
  while(data[0] != 0xff);
  
  //read the remaining 3 bytes
  int i;
  for(i=1;i<4;i++)
  {
    data[i]=JSN.read();
    // Serial.print("data" + String(i)+ ":");
    // Serial.println(data[i],HEX);
    delay(2);
  }
  JSN.flush();

  int sum;
  sum=(data[0]+data[1]+data[2])&0x00FF;
  // Serial.println("SUM:" + String(sum));
  if(sum==data[3])
  {
    distance=(data[1]<<8)+data[2];
    Serial.println("DISTANCE:" + String(distance));
    if(distance>280)
    {
      //add 0.5 to round
      return (uint16_t)(distance/10 + 0.5);
    }
    else 
    {
      Serial.println("...");
      Serial.println("Below the lower limit");      
      return 0;  
    }
  }
  Serial.println("ERROR");
  return 0;
}

/**
 * 
 */
uint16_t get_distance_to_water_median(uint8_t n)
{
  MedianFilter m(n, 0);
  for (uint8_t i = 0; i < n; i++)
  {
    m.in(get_distance_to_water_SEN0313());
    delay(10);
  }
  Serial.println("FINAL DISTANCE:" + String(m.out()));
  return m.out();
}
