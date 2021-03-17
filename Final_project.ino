#include <SoftwareSerial.h>
#include <stdlib.h>
#define DEBUG true
#define SSID "Android Ap" // "SSID"
#define PASS "12345678"//rd"
#define IP "184.106.153.149" // thingspeak.com ip
String msg = "GET /update?key=6RCQDQ6GJ55WVF0S"; //change it with your api key like "GET /update?key=Your Api Key"
SoftwareSerial esp8266(2, 3);
String tempC;
int error;
int val;
int c1 = 0, c2 = 0;


//NRF part..
///// rx
#include <SPI.h>
#include "RF24.h"

int ms[1]; //SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8
RF24 radio(9,10);
const uint64_t pipe = 00110;
void setup()
{
  //NRF part..
  radio.begin();
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  radio.openReadingPipe(1, pipe);
  radio.startListening();


  //ESP part..
  esp8266.begin(115200);
  Serial.println("AT");
  esp8266.println("AT");
  delay(5000);
  if (esp8266.find("OK"))
  {
    connectWiFi();
  }
}

void loop()
{



  while (radio.available())
  {
    start: //label
    error = 0;
    radio.read(ms, 1);
    delay(1000);



    val = ms[0];
    Serial.println(val);
    tempC = val;
    updateTemp();
    delay(2000);
  }


}


void updateTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if (esp8266.find("Error"))
  {
    return;
  }
  cmd = msg ;
  cmd += "&field1="; //field 1 for temperature
  cmd += tempC;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  if (esp8266.find(">"))
  {
    Serial.print(cmd);
    esp8266.print(cmd);
  }
  else
  {
    Serial.println("AT+CIPCLOSE");
    esp8266.println("AT+CIPCLOSE");
    //Resend...
    error = 1;
  }
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1"); //
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd = "AT+CWJAP=\"";//for joining the wifi to the hotspot.
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if (esp8266.find("OK"))
  {
    return true;
  }
  else
  {
    return false;
  }
}
