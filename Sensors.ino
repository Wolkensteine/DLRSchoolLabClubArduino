#include <TinyXML.h>
#include <SD.h>
 
File myFile;
#define port_x A0
#define port_y A1
#define port_z A2

char UserInput;
String UserInputString;
long normalX = 341;
long highX = 355;
long normalY = 340;
long highY = 355;
long normalZ = 341;
long highZ = 351;

int Counterz;
int allz;

void setup(){
  
  Serial.begin(9600);

  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
}

void loop(){

  myFile = SD.open("data.txt", FILE_WRITE);

  long x = analogRead(port_x);
  long y = analogRead(port_y);
  long z = analogRead(port_z);
  
  long acc_x = map(x,highX,normalX,1000,0);
  long acc_y = map(y,highY,normalY,1000,0);
  long acc_z = map(z,highZ,normalZ,1000,0);
  
  Serial.println("X: " + String(acc_x) + " Y: " + String(acc_y) + " Z: " + String(acc_z));
  
  UserInputString = "";
  
  while(Serial.available() > 0){
    UserInput = Serial.read();
    if(Serial.available() > 0){
      UserInputString += UserInput;
    }
  }
  
  
  if(myFile) {
    myFile.print(String(acc_x) + "\t" + String(acc_y) + "\t" + String(acc_z) + "\t \n");
    myFile.close();
  } else {
    Serial.print("File nicht geöffnet");
  }
  
  if (UserInputString == "calibZ"){
    Counterz = 11;
  }

  if (Counterz > 1) {
    allz += x;
    Counterz --;
  }

  if (Counterz == 1) {
    highZ = allz / 10;
    Serial.println(highZ);
  }

  delay(100);
  
}
