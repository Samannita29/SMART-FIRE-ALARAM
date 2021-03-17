///// rx
#include <SPI.h>
#include "RF24.h"

int msg[1]; //SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8
RF24 radio(7,8);
const uint64_t pipe = 00110;

void setup()
{
radio.begin();
Serial.begin(115200);
pinMode(3, OUTPUT);
radio.openReadingPipe(1, pipe);
radio.startListening();
}

void loop()
{
while (radio.available())
{
radio.read(msg, 1);
Serial.println(msg[0]);
analogWrite(3, msg[0]);
delay(1000);
}
}
