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
 
/* Hardware configuration: */
/* Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
 
byte addresses[2][6] = {"XXXXX","XXXXX"};
 
int failcount = 0;
int successcount = 0;
bool mustlistenforanswer;
 
void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24 Transmitter started"));
 
  // Set the PA Level low (RF24_PA_LOW) to prevent 
  // power supply related issues since this is a
  // getting_started sketch, and the likelihood of 
  // close proximity of the devices. RF24_PA_MAX is default.
  // Open a writing and reading pipe on each radio, 
  // with opposite addresses
}
 
void loop() {
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
 
  // First, stop listening so we can talk.
  radio.stopListening();
 
  mustlistenforanswer = true;
 
  // Remember this time, and send it. 
  // This will block until complete
  unsigned long timeSent = micros();
 
  if (!radio.write( &timeSent, sizeof(unsigned long) ))
  {
    mustlistenforanswer = false;
 
    failcount += 1;
 
    Serial.println(F("failed on sending"));
  }
 
  // invariant: request is sent, or not
 
  if (mustlistenforanswer)
  {
    // invariant: request is sent
 
    radio.startListening();                                    
    unsigned long started_waiting_at = micros();
    boolean timeoutoccurred = false;
 
    while ( !radio.available() ) {                            
      unsigned long timeNow = micros();
 
      if (timeNow - started_waiting_at > 400000 ) {           
        timeoutoccurred = true;
        break;
      }
    }
 
    if ( timeoutoccurred )
    {
      // Describe the results
 
      failcount += 1;
 
      Serial.println(F("Failed, response timed out."));
    }
    else
    {
      unsigned long timeReceived;
      radio.read( &timeReceived, sizeof(unsigned long) );
 
      unsigned long timeNow = micros();
 
      // Spew it
      Serial.print(F("Response "));
      Serial.print(timeReceived);
      Serial.print(F(", Round-trip delay "));
      Serial.print((timeNow - timeReceived) / 1000);
      Serial.print(F(" milliseconds; Error ratio:"));
 
      successcount += 1;
      char buffer[8];
      sprintf(buffer, "%03d/%03d;", failcount,  successcount);
 
      Serial.println(buffer);
    }
  }
 
  delay(1500);
}
