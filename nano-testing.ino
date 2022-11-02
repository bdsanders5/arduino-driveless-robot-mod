#include <Servo.h>

Servo myservo;  // create servo object to control a servo


//define RC input connections (all attach interupt pins) -> nano attach interupt pins = 2, 3
int ch1Input = 2; //right joystick vertical
int ch2Input = 3; //right joystick horizontal

int servoOutPin = 9;


int servoPos = 90;
int centerPosition = 1500; // rc tx joystick center value for both vertical & horizontal
int joyposVert = 1500; //start out as centered
int joyposHorz = 1500; //start out as centered
int signalBuffer = 40; // used for joystick sensitivity



//initialize RC input variables & temp vars for calculations
volatile long ch1; 
volatile long count0; // temporary variable for ch7
volatile long ch2;
volatile long count1; // temporary variable for ch8



void setup() {
  //Serial.begin(9600); //for diagnostics
  myservo.attach(servoOutPin);  // attaches the servo on pin 9 to the servo object
  initRCInputPins();
  delay(2000); //if arduino sets up before rc tx gets connection
} 

void loop() { 
  delay(100);
  //serialPrintInputVars(); //used for testing -> function at page's bottom
  controlMotors();
} 




void controlMotors() {
  
  joyposVert = ch1;
  joyposHorz = ch2;

  //make sure values received from rc tx are in range that we're mapping...
  if (joyposVert > 2000) {
    joyposVert = 2000;
  } else if (joyposVert < 1000) {
    joyposVert = 1000;
  }
  if (joyposHorz > 2000) {
    joyposHorz = 2000;
  } else if (joyposHorz < 1000) {
    joyposHorz = 1000;
  }
    
  //make sure valid signal incoming..
  if ((joyposVert > 750) && (joyposHorz > 750)) {
    
 
    //handle servo position...
    if (joyposHorz > (centerPosition + signalBuffer)) {
      servoPos = map(joyposHorz, 1500, 2000, 91, 180);
    //handle left position...
    } else if (joyposHorz < (centerPosition - signalBuffer)) {
      servoPos = map(joyposHorz, 1500, 1000, 0, 89);
    } else {
      // This is Stopped
      servoPos = 90;
    }


   
  } else {
    servoPos = 90;
    myservo.write(servoPos);
  }
}








void initRCInputPins() {
  pinMode(ch1Input, INPUT);
  pinMode(ch2Input, INPUT);
  // Catch up and down 
  #define int0 (PINE & 0x10) // PIN 2  
  #define int1 (PINE & 0x20) // PIN 3
  attachInterrupt(0,handleInterrupt_P2,CHANGE);  // PIN 2
  attachInterrupt(1,handleInterrupt_P3,CHANGE); // PIN 3
}




//helper functions...

void handleInterrupt_P2() { 
  if(int0) 
    count0=micros(); // we got a positive edge 
  else 
   ch1=micros()-count0; // Negative edge: get pulsewidth 
}

void handleInterrupt_P3() { 
  if(int1) 
    count1=micros(); // we got a positive edge 
  else 
   ch2=micros()-count1; // Negative edge: get pulsewidth 
} 


void serialPrintInputVars() {
  delay(1000); 
  Serial.println(ch1); //center: = 1492-1496, top = 2000, bottom = 992 ////good start position might be 1550
  Serial.println(ch2); //center = 1492-1500, left = 992, right = 1992-1996
}

