
//define RC input connections (all attach interupt pins) -> mega attach interupt pins = 2, 3, 18, 19, 20, 21
int ch1Input = 21; //joystick vertical
int ch2Input = 20; //joystick horizontal
int ch5Input = 19; //trigger safety
int ch6Input = 18; //camera on/off
int ch7Input = 2; //trigger pull
int ch8Input = 3; //light on/off

//empty connection -> this shouldgo to white relay.
int emptySwitchOutput1 = A4;

//motor enables...
int motor1_R_EN = ?;
int motor1_L_EN = ?;
int motor2_R_EN = ?;
int motor2_L_EN = ?;

//Motor output Connections - need changedto analog pins...
int panUpOutput = 42;
int rotateRightOutput = 40;
int panDownOutput = 38;
int rotateLeftOutput = 41;



//camera output connection
int camera1PowerOutput = A6; // control camera & transmitter with this connection

//trigger output connections
int pullTriggerOutput = 43;
//light switch output pin
int lightSwitchOutput = 44;



//initialize startup variables
int motor1Rpmw = 0;
int motor1Lpmw = 0;
int motor2Rpmw = 0;
int motor2Lpmw = 0;

int triggerSwitchState = 0;
int camera1SwitchState = 0;
int lightSwitchState = 0;
int safetySwitchState = 0;



//initialize RC input variables & temp vars for calculations
volatile long ch7; 
volatile long count0; // temporary variable for ch7
volatile long ch8;
volatile long count1; // temporary variable for ch8
volatile long ch6;
volatile long count2; // temporary variable for ch6 
volatile long ch5; // servo value 
volatile long count3; // temporary variable for ch5
volatile long ch2; // servo value 
volatile long count4; // temporary variable for ch2
volatile long ch1; // servo value 
volatile long count5; // temporary variable for ch1

// Define controller motors Joystick Values - Start at 1500 (middle position)
int joyposVert = 1500;
int joyposHorz = 1500;

int noSignalLoopCounter = 0;




void setup() { 
  Serial.begin(9600);
  initRCInputPins();
  initSwitchPins();
  //initTiltPanPins();
} 

void loop() { 
  delay(100); 
  /*
  if ( ( (ch7 > 1250) && (ch7 < 1750) ) || (ch7 == 0) ) {
    noSignalLoopCounter++;
  } else {
    noSignalLoopCounter = 0;
  }
  if (noSignalLoopCounter > 5) {
    killRobot();
  }
  */
  
  controlDriverMotors();

} 



void initRCInputPins() {
  pinMode(ch1Input, INPUT);
  pinMode(ch2Input, INPUT);
  pinMode(ch5Input, INPUT);
  pinMode(ch6Input, INPUT);
  pinMode(ch7Input, INPUT);
  pinMode(ch8Input, INPUT);
  // Catch up and down 
  #define int0 (PINE & 0x10) // PIN 2  
  #define int1 (PINE & 0x20) // PIN 3
  #define int2 (PIND & 0b00001000) // PIN 18 
  #define int3 (PIND & 0b00000100) // PIN 19
  #define int4 (PIND & 0b00000010) //pin 20
  #define int5 (PIND & 0b00000001) //pin 21
  attachInterrupt(2,handleInterrupt_P21,CHANGE); // PIN 21
  attachInterrupt(3,handleInterrupt_P20,CHANGE); // PIN 20
  attachInterrupt(4,handleInterrupt_P19,CHANGE); // PIN 19
  attachInterrupt(5,handleInterrupt_P18,CHANGE); // PIN 18
  attachInterrupt(0,handleInterrupt_P2,CHANGE);  // PIN 2
  attachInterrupt(1,handleInterrupt_P3,CHANGE); // PIN 3
}


void initSwitchPins() {
  //initialize digital output pins to HIGH so we can sink any current supplied to them (they source current immediately when pinmode set)...
  
  digitalWrite(pullTriggerOutput, HIGH);
  digitalWrite(camera1PowerOutput, HIGH);
  digitalWrite(motor1_R_EN, HIGH);
  digitalWrite(motor1_L_EN, HIGH);
  digitalWrite(motor2_R_EN, HIGH);
  digitalWrite(motor2_L_EN, HIGH);
  digitalWrite(lightSwitchOutput, HIGH);
  digitalWrite(emptySwitchOutput1, HIGH);

  //set output pins...
  pinMode(pullTriggerOutput, OUTPUT);
  pinMode(camera1PowerOutput, OUTPUT);
  pinMode(motor1_R_EN, OUTPUT);
  pinMode(motor1_L_EN, OUTPUT);
  pinMode(motor2_R_EN, OUTPUT);
  pinMode(motor2_L_EN, OUTPUT);
  pinMode(lightSwitchOutput, OUTPUT);
  pinMode(emptySwitchOutput1, OUTPUT);

  //turn camera on by default...
  digitalWrite(camera1PowerOutput, LOW);
  digitalWrite(camera1SigalOutput, LOW);

}






void controlTiltPan() {
  
  
  //control motor one....................................................................
  //you need to find the range of ch1 ///this sketch assumes 0 to 1023
  
  //should prolly add a buffer aswell
  
  //repeat code below for motor 2
  
  //int sensorValue = ch1; //changed to ch1 in code
 
  // sensor value is in the range 0 to 1023
  // the lower half of it we use for reverse rotation; the upper half for forward rotation
  if (ch1 < 512)
  {
        
    // reverse rotation
    digitalWrite(motor1_R_EN, LOW);
    digitalWrite(motor1_L_EN, HIGH);

    int reversePWM = -(ch1 - 511) / 2; //max speed 255 / adjust accordingly...
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, reversePWM);
  }
  else
  {
    
    // forward rotation
    digitalWrite(motor1_L_EN, LOW);
    digitalWrite(motor1_R_EN, HIGH);

    int forwardPWM = (ch1 - 512) / 2; //max speed 255 / adjust accordingly...
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, forwardPWM);
  }
  //.............................................................................................
  
  
  //old stuff...................
  if (ch1 > 1600) {
    upSpeed = map(joyposVert, 1500, 1000, 0, 255); // motor 1... 0 is stopped, 255 is full reverse...
    analogWrite(panUpOutput, upSpeed);
    analogWrite(panDownOutput, 0);
  if (ch1 <= 1600) {
    joyUpState = 0;
  }
  if ((ch1 < 1350) && (ch1 > 900)) {
    joyDownState = 1;
  } else {
    joyDownState = 0;
  }
  if (ch4 > 1600)  {
    joyRightState = 1;
  } else {
    joyRightState = 0;
  }
  if ( (ch4 < 1450) && (ch4 > 900) ) {
    joyLeftState = 1;
  } else {
    joyLeftState = 0;
  }

    
  
  }
  if (joyRightState == 1) {
    digitalWrite(rotateRightOutput, LOW);
  } else {
    digitalWrite(rotateRightOutput, HIGH);
  }
  if (joyDownState == 1) {
    digitalWrite(panDownOutput, LOW);
  } else {
    digitalWrite(panDownOutput, HIGH);
  }
  if (joyLeftState == 1) {
    digitalWrite(rotateLeftOutput, LOW);
  } else {
    digitalWrite(rotateLeftOutput, HIGH);
  }

}



void controlSwitches() {
  //ch6 = top right rotation thing // clockwise = 1983 //counter clockwise = 990
  if (ch6 > 1500) {
    camera1SwitchState = 0;
  } else {
    camera1SwitchState = 1;
  }
  //ch8 = top left rotation thing // clockwise = 1983 //counter clockwise = 990
  if (ch8 > 1500) {
    lightSwitchState = 1;
  } else {
    lightSwitchState = 0;
  }

  if (camera1SwitchState == 1) {
    digitalWrite(cameraTXSwitchOutput, LOW);
    digitalWrite(camera1PowerOutput, LOW);
  } else {
    digitalWrite(cameraTXSwitchOutput, HIGH);
    digitalWrite(camera1PowerOutput, HIGH);
  }

  if (lightSwitchState == 1) {
    digitalWrite(lightSwitchOutput, LOW);
  } else {
    digitalWrite(lightSwitchOutput, HIGH);
  }

}



void controlTrigger() {
  if (ch5 > 1200) { //top right toggle switch in down position...
    //ch7 = upper left toggle // up 990 , down 1982
    if (ch7 > 1500) {
      triggerSwitchState = 1;
    } else {
      triggerSwitchState = 0;
    }
    if (triggerSwitchState == 1) {
      digitalWrite(pullTriggerOutput, LOW);
      delay(50);
    } else {
      digitalWrite(pullTriggerOutput, HIGH);
    }
  } else {
      digitalWrite(pullTriggerOutput, HIGH);
  }
}






void controlDriverMotors() {

  //disable trigger...
  triggerSwitchState = 0;
  digitalWrite(pullTriggerOutput, HIGH);
  
  joyposVert = ch1;
  joyposHorz = ch2;
  int centerPositionVert = 1500;
  int centerPositionHorz = 1492;
  int signalBuffer = 25;

  if ((joyposVert > 750) && (joyposHorz > 750)) {
    if (joyposVert < (centerPositionVert - signalBuffer)) {
      // This is Backward
      //Determine Motor Speeds
      // As we are going backwards we need to reverse readings
      driverMotorsStateX = map(joyposVert, 1500, 1000, 64, 1); // motor 1... 64 is stopped, 1 is full reverse...
      driverMotorsStateY = map(joyposVert, 1500, 1000, 192, 128); // motor 2... 192 is stopped, 128 is full reverse
    } else if (joyposVert > (centerPositionVert + signalBuffer)) {
      // This is Forward
      //Determine Motor Speeds
      driverMotorsStateX = map(joyposVert, 1500, 2000, 64, 127);// motor 1... 64 is stopped, 127 full forward...
      driverMotorsStateY = map(joyposVert, 1500, 2000, 192, 255); // motor 2... 192 is stopped, 255 full forward.
    } else {
      // This is Stopped
      driverMotorsStateX = 64;
      driverMotorsStateY = 192;
    }


  // Now do the steering
  // The Horizontal position will "weigh" the motor speed
  // Values for each motor
    if (joyposHorz < (centerPositionHorz - signalBuffer)) {
      // Move Left
      // As we are going left we need to reverse readings
      // Map the number to a value of 64 maximum
      joyposHorz = map(joyposHorz, 1492, 992, 0, 64);
      driverMotorsStateX = driverMotorsStateX - joyposHorz;
      driverMotorsStateY = driverMotorsStateY + joyposHorz;
      // Don't exceed max range for motor speeds
      if (driverMotorsStateX < 0)driverMotorsStateX = 0;
      if (driverMotorsStateY > 255)driverMotorsStateY = 255;
    } else if (joyposHorz > (centerPositionHorz + signalBuffer)) {
      // Move Right
      // Map the number to a value of 64 maximum
      joyposHorz = map(joyposHorz, 1492, 1984, 0, 64);
      driverMotorsStateX = driverMotorsStateX + joyposHorz;
      driverMotorsStateY = driverMotorsStateY - joyposHorz;
      // Don't exceed max range for motor speeds
      if (driverMotorsStateX > 127)driverMotorsStateX = 127;
      if (driverMotorsStateY < 0)driverMotorsStateY = 0;      
    }

    if (driverMotorsStateX == 0) {
      driverMotorsStateX = 64;
    }
    if (driverMotorsStateY == 0) {
      driverMotorsStateY = 192;
    }

    // Set Motor Direction
    Serial2.write(driverMotorsStateX);
    Serial2.write(driverMotorsStateY);

  }

}










//helper functions...


void killRobot() {
  digitalWrite(pullTriggerOutput, HIGH);
  //digitalWrite(cameraTXSwitchOutput, HIGH); leave a camera on
  digitalWrite(lightSwitchOutput, HIGH);
  digitalWrite(panUpOutput, HIGH);
  digitalWrite(rotateRightOutput, HIGH);
  digitalWrite(panDownOutput, HIGH);
  digitalWrite(rotateLeftOutput, HIGH);

}







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

void handleInterrupt_P18() { 
  if(int2) 
    count2=micros(); // we got a positive edge 
  else 
   ch6=micros()-count2; // Negative edge: get pulsewidth 
}

void handleInterrupt_P19() { 
  if(int3) 
    count3=micros(); // we got a positive edge 
  else 
   ch5=micros()-count3; // Negative edge: get pulsewidth 
}


void handleInterrupt_P20() { 
  if(int4) 
    count4=micros(); // we got a positive edge 
  else 
   ch4=micros()-count4; // Negative edge: get pulsewidth 
}

void handleInterrupt_P21() { 
  if(int5) 
    count5=micros(); // we got a positive edge 
  else 
   ch3=micros()-count5; // Negative edge: get pulsewidth 
}
