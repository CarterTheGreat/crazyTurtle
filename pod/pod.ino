#include <SPI.h>
#include <DualVNH5019MotorShield.h>
#include <nRF24L01.h>
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <EEPROM.h>


/*
 * Mini Pod Code
 * V 0.2
 * 
 * Carter Watts
 * 
 * Notes:
 *  Added breaking TEST
 *  Tune comfortable hand gestures
 *  
 */

//Test & Control
  int led = 19;
  
//Comm
  RF24 radio (7, 8); // CE, CSN
  RF24Network network(radio);
  RF24Mesh mesh(radio, network);

//Motor
  unsigned char INA1 = 2;
  unsigned char INB1 = 4;
  unsigned char PWM1 = 9;
  unsigned char EN1DIAG1 = 6;
  unsigned char CS1 = A0;
  unsigned char INA2 = 3;
  unsigned char INB2 = 5;
  unsigned char PWM2 = 10;
  unsigned char EN2DIAG2 = 6;
  unsigned char CS2 = A1;
  DualVNH5019MotorShield md (INA1,
                           INB1,
                           PWM1,
                           EN1DIAG1,
                           CS1,
                           INA2,
                           INB2,
                           PWM2,
                           EN2DIAG2,
                           CS2);

//Vars

  struct payload_t {
    String data;
  };
  int startInd, ind1, ind2, ind3, ind4, ind5, endInd;
  int runningB = 0;
  int x, y, z, f1, f2;
  String runningS, xS ,yS, zS, f1S, f2S;
  
  //SET SIDE AND MESH ID HERE FOR EVERY POD MADE-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-NOTICE-\-\-\-\-\-\-\-\-\-|
  #define nodeID 1
  
  const String LEFT = "left";
  const String RIGHT = "right";
  
  String side = LEFT;
  //String side = RIGHT;
  
void setup() {
  
  Serial.begin(115200);
  
  pinMode(led, OUTPUT);
  
  //Radio
  
    Serial.println("Connecting to mesh");
    mesh.setNodeID(nodeID);
    mesh.begin();

    delay(200);
    
  //Motor Driver  
    Serial.println("Starting Motor Driver");
    md.init();

  //Startup display
    Serial.print(F("Starting "));
    Serial.print(side);
    Serial.println(F(" pod"));
  
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

void loop(){

  md.init();
  mesh.update();
  digitalWrite(led, LOW);
  
    //Reading
    String data;

    if(network.available()){
      while(network.available()){
        RF24NetworkHeader header;
        payload_t payload;
        network.read(header, &payload, sizeof(payload));
        data = String(payload.data);
      }

      
      //Testing
      Serial.print("Recieved: ");
      Serial.println(data);
      digitalWrite(led, HIGH);
      
      //Indexing
        ind1 = data.indexOf('/');
        ind2 = data.indexOf('/',ind1+1);
        ind3 = data.indexOf('/',ind2+1);
        ind4 = data.indexOf('/',ind3+1);
        ind5 = data.indexOf('/',ind4+1);
        endInd = data.indexOf('>');
  
      //String Data
        runningS = data.substring(startInd+1,ind1);
        xS = data.substring(ind1+1,ind2);
        yS = data.substring(ind2+1,ind3);
        zS = data.substring(ind3+1,ind4);
        f1S = data.substring(ind4+1,endInd);
        f2S = data.substring(ind5+1,endInd);
      
      //Int data
        runningB = runningS.toInt();
        x = xS.toInt();
        y = yS.toInt();
        z = zS.toInt();
        f1 = f1S.toInt();
        f2 = f2S.toInt();

      if(runningB){        
        //Control motors
          if(x < 40 && x > -40)
            x = 0;
          if(y < 40 && y > -40)
            y = 0; 
                                 
          //For left
          if(side == LEFT){
            int funct = -y+x;
            //brakes
            if (funct < 30 && funct > -30){
              md.setBrakes(0,0);
            //Drive
            }else{
              int motorSpeed = map(funct, -370, 370, -400, 400);
              md.setSpeeds(-motorSpeed, -motorSpeed);
              //stopIfFault();
              Serial.print(F("Speed set to: "));
              Serial.println(motorSpeed);
              Serial.print(F("Funct = "));
              Serial.println(funct);
            }
          }        
        
        //For right
        if(side == RIGHT){
          int funct = y+x;
          //Brakes
          if (funct < 30 && funct > -30){
            md.setBrakes(0,0);
            //Drive
          }else{
            int motorSpeed = map(funct, -370, 370, -400, 400);
            md.setSpeeds(motorSpeed, motorSpeed);
            //stopIfFault();
            Serial.print(F("Speed set to: "));
            Serial.println(motorSpeed);
            Serial.print(F("Funct = "));
            Serial.println(funct);
          }          
        }     
      }
    } 
}

void respond(String response, int target){
  
  if(!mesh.write(&response, 'M', sizeof(response), target)){
    Serial.println("Failed to send");
    //Test connection if failed
    if(!mesh.checkConnection() ){
      Serial.println("Renewing Address");
      mesh.renewAddress();
    }else{
      Serial.println("Send failed, test worked");
    }
  }else{
    Serial.print("Sent: ");Serial.print(response);Serial.print(" to ");Serial.println(target);
  }
}  `

void stopIfFault(){
  
  if (md.getM1Fault()){
    Serial.println("M1 fault");
    delay(10);
  }
  if (md.getM2Fault()){
    Serial.println("M2 fault");
    delay(10);
  }
  
}
