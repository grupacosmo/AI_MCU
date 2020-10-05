
#include <SoftwareSerial.h>
SoftwareSerial softSerial(2, 3); // RX, TX

void setup() {
  softSerial.begin(115200);
  Serial.begin(115200);
}
unsigned long timer = 0;

void loop() {
    handle_uart();
    if(Serial.available()){
      int reqest = Serial.parseInt();
      Serial.println("====REQEST====");
      printName(reqest);
      Serial.println("====REPLY====");
      
      softSerial.write(byte(reqest));
//      if(reqest == 115){
//        delay(100);
//        Serial.println("model changed");
//      }
    }

}

void printName(int id){
  switch(id){
    case 101: Serial.println("Postion"); break;
    case 102: Serial.println("Area Dimensions"); break;
    case 103: Serial.println("area width"); break;
    case 104: Serial.println("area height"); break;
    case 105: Serial.println("probability of detection"); break;
    case 106: Serial.println("obj class number"); break;
    case 107: Serial.println("index"); break;
    case 108: Serial.println("number of objects"); break;
    case 115: Serial.println("change model to face detection"); break;
    
  }
}
