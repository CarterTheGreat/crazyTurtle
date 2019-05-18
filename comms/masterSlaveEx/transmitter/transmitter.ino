#include <SPI.h>
#include "RF24.h"
 
// For Nano, Mini Pro, or Uno
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
//Set up nRF24L01 radio on SPI bus plus pins 7 & 8 

//Radio
  RF24 radio(7, 8);
  byte addresses[2][6] = {"AAAAA","AAAAA"};
  
//Data
  int failcount = 0;
  int successcount = 0;
  bool mustListen;
  
void setup() {
  
  Serial.begin(115200);
  Serial.println( F("RF24 Transmitter started"));
}
 
void loop() {
  
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  delay(200);

  //Send
    radio.stopListening();
    mustListen = true;
    String data = "test";
    //Attempts to send w/ error checking
    if (!radio.write( &data, sizeof(data) )){
      
      mustListen = false;
      failcount += 1;
      Serial.println(F("failed on sending"));
    }else{
      Serial.print(F("Sent: "));
      Serial.println(data);
    }
 /*
  //Recieve
    if (mustListen){
       
      radio.startListening();                                    
      unsigned long started_waiting_at = micros();
      boolean timeoutoccurred = false;

      //Timeout check
      while ( !radio.available() ){                            
        unsigned long timeNow = micros();
   
        if (timeNow - started_waiting_at > 400000 ) {           
          timeoutoccurred = true;
          break;
        }
      }

      //Read with error check for timeout
      if ( timeoutoccurred ){
        failcount += 1;
        Serial.println(F("Failed, response timed out."));
      }
      else{
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
  */
 
  delay(50);

}
