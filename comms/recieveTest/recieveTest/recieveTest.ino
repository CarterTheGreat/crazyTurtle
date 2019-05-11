#include <RH_ASK.h>
#include <SPI.h>

//Comm-------------------------------------------------------
RH_ASK driver;
char key;
String data = "";
String message;
int startInd, ind1, ind2, ind3, ind4, ind5, endInd;
int runningB, x, y, z, f1, f2;
String runningS, xS ,yS, zS, f1S, f2S;

//-------------------------------------------------------------

void setup() {
  
  Serial.begin(9600);
  if(!driver.init())
    Serial.println("Radio driver init failed");
  else
    Serial.println("Initialized");
    
  delay(500);
  
}

void loop(){
  readRadio();
  printData();
}

boolean readRadio(){
  //Reads radio data sent
  uint8_t buf[28];
  uint8_t buflen = sizeof(buf);
  while(driver.recv(buf, &buflen)){
    
    //Reading
    data = buf;

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

    //Message
    message = "<";
    message.concat(runningS);
    message.concat("/");
    message.concat(x);
    message.concat("/");
    message.concat(y);
    message.concat("/");
    message.concat(z);
    message.concat("/");
    message.concat(f1);
    message.concat("/");
    message.concat(f2);
    message.concat(">");
    
  }
}

void printData() {
  
  Serial.println("Recieved: "+message);

}
