#include <SPI.h>
#include <DualVNH5019MotorShield.h>
#include <nRF24L01.h>
#include <RF24.h>


/*
 * Mini Pod Code
 * V 0.2
 * 
 * Carter Watts
 * 
 * Notes:
 */

//Comm
  RF24 radio (7, 8); // CE, CSN
  const byte addresses[][6] = {"XXXXX", "XXXXX"};
  char dataIn[28] = "";
  String data = "";
  boolean dataOutB = false;
  String dataOut;
  int startInd, ind1, ind2, ind3, ind4, ind5, endInd;
  int runningB, x, y, z, f1, f2;
  String runningS, xS ,yS, zS, f1S, f2S;

//Motor
  DualVNH5019MotorShield md;
  //Which side of the robot the pod is - needed for arcade control
  const String LEFT = "left";
  const String RIGHT = "right";

//SET SIDE HERE FOR EVERY POD MADE-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-NOTICE-\-\-\-\-\-\-\-\-\-|
  //String side = LEFT;
  String side = RIGHT;
  
void setup() {
  
  Serial.begin(115200);

  //Radio
    radio.begin();
    radio.openWritingPipe(addresses[0]);     // Disregard 00001
    radio.openReadingPipe(1, addresses[1]);  // Disregard 00002
    radio.setPALevel(RF24_PA_MIN);
  //Motor Driver  
    md.init();

  Serial.print(F("Started "));
  Serial.print(side);
  Serial.println(F(" pod"));
}

void loop(){

  //Control
  delay(5);
  radio.startListening();
    //Reading
    if(radio.available()){
      while(radio.available())
        radio.read(&dataIn, sizeof(dataIn));
  
      //Testing
      //Serial.print("Recieved: ");
      //Serial.println(dataIn);

      data = String(dataIn);
  
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

      //Control motors
        if(x < 30 && x > -30)
          x = 0;
        if(y < 30 && y > -30)
          y = 0;
        
        //For left
        if(side == LEFT){
          int funct = -y+x;
            //if (funct < 30 && funct > -30) For setting brakes
          int motorSpeed = map(funct, -370, 370, -400, 400);
          md.setSpeeds(motorSpeed, motorSpeed);
          /*
          Serial.print(F("Speed set to: "));
          Serial.println(motorSpeed);
          Serial.print(F("Funct = "));
          Serial.println(funct);
          */
        }
        
        //For right
        if(side == RIGHT){
          int funct = y+x;
            //if (funct < 30 && funct > -30) For setting brakes
          int motorSpeed = map(funct, -370, 370, -400, 400);
          md.setSpeeds(motorSpeed, motorSpeed);
          /*
          Serial.print(F("Speed set to: "));
          Serial.println(motorSpeed);
          Serial.print(F("Funct = "));
          Serial.println(funct);
          */
        }
    }

  //Response test after motor success
  /*
    delay(5);
    radio.stopListening();
    if(dataOutB)
      radio.write(&dataOut, sizeof(dataOut));
  */
}

void respond(String response){
  dataOutB = true;
  dataOut = response;

}

void stopIfFault(){
  
  if (md.getM1Fault()){
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault()){
    Serial.println("M2 fault");
    while(1);
  }
  
}
