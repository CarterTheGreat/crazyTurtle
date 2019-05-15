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
 *  Added breaking TEST
 *  Tune comfortable hand gestures
 *  
 */
 
  int led = 19;
  
//Comm
  RF24 radio (7, 8); // CE, CSN
  const byte addresses[][6] = {"AAAAA", "AAAAA"};
  char dataIn[28] = "";
  boolean dataOutB = false;
  int startInd, ind1, ind2, ind3, ind4, ind5, endInd;
  int runningB = 0;
  int x, y, z, f1, f2;
  String runningS, xS ,yS, zS, f1S, f2S;

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
                          
  //Which side of the robot the pod is - needed for arcade control
  const String LEFT = "left";
  const String RIGHT = "right";

//SET SIDE HERE FOR EVERY POD MADE-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-NOTICE-\-\-\-\-\-\-\-\-\-|
  String side = LEFT;
  //String side = RIGHT;
  
void setup() {
  
  Serial.begin(115200);
  
  pinMode(led, OUTPUT);
  
  //Radio
    radio.begin();
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
    radio.setPALevel(RF24_PA_MAX);
    
    delay(1000);
    
  //Motor Driver  
    md.init();
    Serial.println("Motor Driver Starting");

  //Startup display
    Serial.print(F("Started "));
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
  digitalWrite(led, LOW);
  
  radio.startListening();
    //Reading
    if(radio.available()){
      while(radio.available())
        radio.read(&dataIn, sizeof(dataIn));

      
      //Testing
      Serial.print("Recieved: ");
      Serial.println(dataIn);
      digitalWrite(led, HIGH);

      String data = String(dataIn);
  
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
              md.setSpeeds(motorSpeed, motorSpeed);
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

void respond(String response){
  dataOutB = true;
  //Send response;

}

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
