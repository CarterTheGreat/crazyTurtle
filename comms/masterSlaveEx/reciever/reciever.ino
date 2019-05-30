#include <SPI.h>
#include "RF24.h"
  
// on Nano, Mini Pro, or Uno
// |--------------------|
// |    UPSIDE      8 7 |
// |     PINS       6 5 |\
// |      ON        4 3 |\\
// |  OTHER SIDE    2 1 |\\
// |--------------------|\\
//                     \  \
// # DESC COLOR  Arduino port
// 1 GND  BLACK           GND
// 2 VCC  RED           3.3V!
// 3 CE   ORANGE            7
// 4 CSN  YELLOW            8
// 5 SCK  GREEN            13
// 6 MOSI BLUE             11
// 7 MISO VIOLET           12
// on SPI bus plus pins 7 & 8 

  int led = 4;
  
//Radio
  RF24 radio(7,8);
  byte addresses[2][6] = {"AAAAA","AAAAA"};
  
void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24 Receiver started"));

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
 
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);

  delay(300);
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(300);
  digitalWrite(led, HIGH);
  delay(300);  
  digitalWrite(led, LOW);
  delay(300);
}
  
void loop() {
  radio.startListening();

  delay(5);
  digitalWrite(led, LOW);
 
  if( radio.available()){
 
    char data[40] = "";
  
    while (radio.available()) {
      radio.read( &data, sizeof(data) );
    }
 
    //radio.stopListening();
    //radio.write( &data, sizeof(data) );
  
    Serial.print(F("Recieved: "));
    Serial.println(data);
    digitalWrite(led, HIGH);
  }
}
