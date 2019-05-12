#include <SPI.h>
#include "RF24.h"
  
// on Nano or Mini Pro
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
  
/* Hardware configuration: Set up nRF24L01 radio /*
/* on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
  
byte addresses[2][6] = {"XXXXX","XXXXX"};
  
void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24 Receiver started"));
 
  // Set the PA Level low (RF24_PA_LOW) to prevent
  // power supply related issues since this is a
  // getting_started sketch, and the likelihood of
  // close proximity of the devices. RF24_PA_MAX is default.
  // Open a writing and reading pipe on each radio,
  // with opposite addresses
 
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
}
  
void loop() {
  radio.startListening();
 
  if( radio.available()){
 
    unsigned long timeReceived;
  
    while (radio.available()) {
      radio.read( &timeReceived, sizeof(unsigned long) );
    }
 
    radio.stopListening();
    radio.write( &timeReceived, sizeof(unsigned long) );
  
    Serial.print(F("Sent response "));
    Serial.println(timeReceived);
  }
}
