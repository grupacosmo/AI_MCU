#define DATA_ITERATION 2
#define END_OF_TRASMISSION_CODE 154
#define STRING_TRASMISSION_CODE 202
#define ERROR_STRING_TRASMISSION_CODE 203
#define REAL_TIME_DATA_SIZE 15
#define TIMEOUT 5

#define POSITION_ID 101
#define AREA_DIMENSIONS 102
#define VALUE_ID 103
#define CLASS_ID 104
#define INDEX_ID 105
#define OBJNUM_ID 106

#define X_MIN 0
#define X_MAX 640
#define Y_MIN 0
#define Y_MAX 480
#define VALUE_MIN 0.0f
#define VALUE_MAX 1.0f
#define W_MIN 0
#define W_MAX 640
#define H_MIN 0
#define H_MAX 480
#define CLASS_MIN 0
#define CLASS_MAX 10//?????
#define OBJ_NUM_MIN 0
#define OBJ_NUM_MAX 10
#define CHANGE_MODEL 115
byte j;
byte tmpBuf[4];
float tmpValA =0;
int tmpValB =0;
unsigned long tmpValC = 0;
float rangeCheckA;
int rangeCheckB;

byte byteValue;


void handle_uart(){
  while(softSerial.available()){
    Serial.println("rec");
    byte id = getID();
    Serial.println(id);
    if(id == 0){
      softSerialFlush();
      continue;
    }
    switch((int)id){
      case POSITION_ID: readPosition(); break;
      case AREA_DIMENSIONS: readAreaDimensions(); break;
      case VALUE_ID: readValue(); break;
      case CLASS_ID: readClass(); break;
      case INDEX_ID: readIndex(); break;
      case OBJNUM_ID: readObjNum(); break;
      case STRING_TRASMISSION_CODE: readText(false); break;
      case ERROR_STRING_TRASMISSION_CODE: readText(true); break;
      //case CHANGE_MODEL: readChangeModel(); break;
      default: softSerialFlush();
    }
    //yield();
    Serial.println();
  }
}

void send_reqest(byte command){
  for(int j =0; j < DATA_ITERATION; j++){
      softSerial.write(command);
  }
}

boolean readPosition(){
   if(!read(rangeCheckB)) return false;
   if(isnan(rangeCheckB)||rangeCheckB <X_MIN||rangeCheckB >X_MAX) return false;
   Serial.print("X: ");
   Serial.println(rangeCheckB);
   if(!read(rangeCheckB)) return false;
   if(isnan(rangeCheckB)||rangeCheckB <Y_MIN||rangeCheckB >Y_MAX) return false;
   Serial.print("Y: ");
   Serial.println(rangeCheckB);
   return true;
}

boolean readAreaDimensions(){
   if(!read(rangeCheckB)) return false;
   if(isnan(rangeCheckB)||rangeCheckB <W_MIN||rangeCheckB >W_MAX) return false;
   Serial.print("Area width: ");
   Serial.println(rangeCheckB);
   if(!read(rangeCheckB)) return false;
   if(isnan(rangeCheckB)||rangeCheckB <H_MIN||rangeCheckB >H_MAX) return false;
   Serial.print("Area height: ");
   Serial.println(rangeCheckB);
   return true;
}

void readValue(){
   if(!read(rangeCheckA)) return false;
   if(isnan(rangeCheckA)||rangeCheckA <VALUE_MIN||rangeCheckA >VALUE_MAX) return false;
   Serial.print("Probability of detection: ");
   Serial.println(rangeCheckA);
   return true;
}

void readClass(){
   if(!read(byteValue)) return false;
   if(isnan(byteValue)||byteValue <H_MIN||byteValue >H_MAX) return false;
   Serial.print("Obj class number: ");
   Serial.println(byteValue);
   return true;
}

void readIndex(){
   if(!read(byteValue)) return false;
   if(isnan(byteValue)||byteValue <H_MIN||byteValue >H_MAX) return false;
   Serial.print("Index: ");
   Serial.println(byteValue);
   return true;
}

void readObjNum(){
   if(!read(byteValue)) return false;
   if(isnan(byteValue)||byteValue <OBJ_NUM_MIN||byteValue >OBJ_NUM_MAX) return false;
   Serial.print("Number of objects: ");
   Serial.println(byteValue);
   return true;
}

void readText(boolean sendAsError){
  if(!checksoftSerial()) return;
  String result = softSerial.readStringUntil('\n');
  if(sendAsError){
//    sendError(result);
  }
}
boolean readChangeModel(){
  int index = (int)(softSerial.read());
  if(index == 0){
    softSerial.println("model changed to face detection");
  }
}

byte getID(){
  byte id = softSerial.read();
  for(j =0; j < DATA_ITERATION -1; j++){
      if(!checksoftSerial()) return 0;
      byte tmp_id = softSerial.read();
      if(tmp_id != id) return 0;
  }
  return id;
}

boolean read(byte &val){
  if(!checksoftSerial()) return false;
  byte tmp = softSerial.read();
  for(j =0; j < DATA_ITERATION -1; j++){
      if(!checksoftSerial()) return false;
      byte tmp_val = softSerial.read();
      if(tmp_val != tmp) return false;
  }
  val = tmp;
  return true;
}

boolean read(float &val){
  boolean flag = false;
  float tmp = readFloat(flag);
  if(flag) return false;
  for(j =0; j < DATA_ITERATION -1; j++){
   float tmp_val = readFloat(flag);
   if(flag) return false;
   if(tmp != tmp_val) return false;
  }
  val = tmp;
  return true;
}

boolean read(int &val){
  boolean flag = false;
  int tmp = readInt(flag);
  if(flag) return false;
  for(j =0; j < DATA_ITERATION -1; j++){
   int tmp_val = readInt(flag);
   if(flag) return false;
   if(tmp_val != tmp) return false;
  }
  val = tmp;
  return true;
}


int readInt(boolean &flag){
   if(!checksoftSerial()){ flag = true; return 0;}
   tmpBuf[0] = softSerial.read();
   if(!checksoftSerial()){ flag = true; return 0;}
   tmpBuf[1] = softSerial.read();
  return tmpBuf[0]|tmpBuf[1]<<8;
}

float readFloat(boolean &flag){
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[0] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[1] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[2] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[3] = softSerial.read();
      float result = *((float*)(tmpBuf));
      return result;
}
float readLong(boolean &flag){
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[0] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[1] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[2] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[3] = softSerial.read();
      unsigned long result = *((unsigned long*)(tmpBuf));
      return result;
}
uint16_t readUint16(boolean &flag){
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[0] = softSerial.read();
      if(!checksoftSerial()){ flag = true; return 0;}
      tmpBuf[1] = softSerial.read();
      uint16_t result = *((uint16_t*)(tmpBuf));
      return result;
}
boolean checksoftSerial(){
    if(!softSerial.available()){
      unsigned long timeOutCtn = millis();
      while(millis() < timeOutCtn + TIMEOUT){
        if(softSerial.available()){
          return true;
        }
        yield();
      }
      return false;
    }
    return true;
}
void softSerialFlush(){
  while(softSerial.available()){
    softSerial.read();
    yield();
  }
}
