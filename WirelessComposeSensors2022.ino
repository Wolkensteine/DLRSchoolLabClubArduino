// Imports
//#include <TinyXML.h>
#include <SD.h>

// Variables
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

int Counterx;
int allx;
int Countery;
int ally;
int Counterz;
int allz;


// Setup
void setup(){
  
  Serial.begin(9600); // to monitor and send commands

  pinMode(10, OUTPUT); // Pins 10 - 13 for SD reader
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
}

void loop(){
  
  // open File on sd
  myFile = SD.open("data.txt", FILE_WRITE);

  // Get data
  long x = analogRead(port_x);
  long y = analogRead(port_y);
  long z = analogRead(port_z);
  
  long acc_x = map(x,highX,normalX,1000,0);
  long acc_y = map(y,highY,normalY,1000,0);
  long acc_z = map(z,highZ,normalZ,1000,0);
  
  Serial.println("X: " + String(acc_x) + " Y: " + String(acc_y) + " Z: " + String(acc_z));
  
 
  // Get user inputs
  UserInputString = "";
  
  while(Serial.available() > 0){
    UserInput = Serial.read();
    if(Serial.available() > 0){
      UserInputString += UserInput;
    }
  }
  
  // print to file
  if(myFile) {
    myFile.print(String(acc_x) + "\t" + String(acc_y) + "\t" + String(acc_z) + "\t" + String(x) + "\t" + String(y) + "\t" + String(z) + "\t \n");
    myFile.close();
  } else {
    Serial.print("File nicht geöffnet");
  }
  
  // Callibration
  if (UserInputString == "calibX") {
    Counterx = 11;
  }
 
  if (UserInputString == "calibY") {
    Countery = 11;
  }
 
  if (UserInputString == "calibZ") {
    Counterz = 11;
  }

  if (Counterx > 1) {
     allx += x;
     ally += y;
     allz += z;
     Counterx --;
  }
  
  if (Countery > 1) {
     allx += x;
     ally += y;
     allz += z;
     Countery --;
  }
 
  if (Counterz > 1) {
     allx += x;
     ally += y;
     allz += z;
     Counterz --;
  }

  if (Counterx == 1) {
    highX = int(allx / 10);
    normalY = int(ally / 10);
    normalZ = int(allz / 10);
    Serial.println("New high x is: " + String(highX));
    //Serial.println("New normal y is: " + String(normalY));
    //Serial.println("New normal z is: " + String(normalZ));
    Counterx --;
    allx = 0;
    ally = 0;
    allz = 0;
  }
 
  if (Countery == 1) {
    normalX = int(allx / 10);
    highY = int(ally / 10);
    normalZ = int(allz / 10);
    Serial.println("New high y is: " + String(highY));
    Countery --;
    allx = 0;
    ally = 0;
    allz = 0;
  }
 
  if (Counterz == 1) {
    normalX = int(allx / 10);
    normalY = int(ally / 10);
    highZ = int(allz / 10);
    Serial.println("New high z is: " + String(highZ));
    Counterz --;
    allx = 0;
    ally = 0;
    allz = 0;
  }

  // delay because we don't want to many values
  delay(100);
  
}
