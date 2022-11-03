int ch7Input = 2; //light on/off - top left 3 way switch
int ch8Input = 3; //camera on/off (top right 3 way switch)

//camera output connection
int camera1PowerOutput = 9; // control camera & transmitter with this connection
int camera1SwitchState = 1; //start with camera on...

//initialize RC input variables & temp vars for calculations
volatile long ch7;
volatile long count0; // temporary variable for ch7
volatile long ch8;
volatile long count1; // temporary variable for ch8

void setup() {
  Serial.begin(9600); //for diagnostics
  initRCInputPins();
  initSwitchPins();
  delay(2000); //if arduino sets up before rc tx gets connection
} 

void loop() { 
  delay(100);
  //make sure you'r getting a signal:
  //if (ch7 < 1000)  {
    serialPrintInputVars(); //used for testing -> function at page's bottom
    controlCamera();
 //}
} 


void controlCamera() {
  //ch8 = top right 3 way switch is camera & transmitter...
  if (ch8 < 1200) {
    //if camera is off, turn it on..
    if (camera1SwitchState == 0) {
      camera1SwitchState = 1;
      digitalWrite(camera1PowerOutput, LOW);
    }
  } else {
    //if camera is on, turn it off...
    if (camera1SwitchState == 1) {
      camera1SwitchState = 0;
      digitalWrite(camera1PowerOutput, HIGH);
    }
  }
}

void initSwitchPins() {
  //initialize digital output pins to HIGH so we can sink any current supplied to them (they source current immediately when pinmode set)...
  digitalWrite(camera1PowerOutput, HIGH);
  //start with camera on...
  digitalWrite(camera1PowerOutput, LOW);

}

void initRCInputPins() {
  pinMode(ch7Input, INPUT);
  pinMode(ch8Input, INPUT);
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
   ch7=micros()-count0; // Negative edge: get pulsewidth 
}

void handleInterrupt_P3() { 
  if(int1) 
    count1=micros(); // we got a positive edge 
  else 
   ch8=micros()-count1; // Negative edge: get pulsewidth 
} 


void serialPrintInputVars() {
  delay(1000); 
  //Serial.println(ch7); //up: 992, center = 1496, down = 1996
  Serial.println(ch8); //up: 992, center = 1496, down = 1996
}
