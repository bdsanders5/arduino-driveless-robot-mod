
//int R_IS = 6;
int R_EN = 39;
int R_PWM = 10;
//int L_IS = 7;
int L_EN = 37;
int L_PWM = 13;

void setup() {
  // put your setup code here, to run once:
 pinMode(R_EN, OUTPUT);
 pinMode(R_PWM, OUTPUT);
 pinMode(L_EN, OUTPUT);
 pinMode(L_PWM, OUTPUT);
 digitalWrite(R_EN, HIGH);
 digitalWrite(L_EN, HIGH);
}

void loop() {
  //testHardCodedSequence();
  testFauxRcInputs();
}

void testHardCodedSequence() {
    // put your main code here, to run repeatedly:
    int i;
    for(i = 0; i <= 255; i= i+10){ //clockwise rotation
     analogWrite(R_PWM, i);
     analogWrite(L_PWM, 0);
     delay(500);
    }
    delay(500);
    for(i = 0; i <= 255; i= i+10){ //counter clockwise rotation
     analogWrite(R_PWM, 0);
     analogWrite(L_PWM, i);
     delay(500);
    }
    delay(500);
}

void testFauxRcInputs() {
  
  ///joyposVert = ch1;
  //joyposHorz = ch2;
  //simulate...
  int joyposVert = 1750;
  int joyposHorz = 1120;
  
  int motor1Speed = 0;
  
  
  //hard coded vars...
  int centerPositionVert = 1500;
  int centerPositionHorz = 1500;
  int signalBuffer = 40;

  //make sure valid signal incoming..
  if ((joyposVert > 750) && (joyposHorz > 750)) {
    
    //handle up position...
    if (joyposVert > (centerPositionVert + signalBuffer)) {
      //Determine Motor Speeds
      motor1Speed = map(joyposVert, 1500, 2025, 0, 255); // motor 1... 0 is stopped, 255 is full reverse...
      analogWrite(R_PWM, motor1Speed);
      analogWrite(L_PWM, 0);

    //handle down position...
    } else if (joyposVert < (centerPositionVert - signalBuffer)) {
      //Determine Motor Speeds
      motor1Speed = map(joyposVert, 1500, 975, 0, 255);// motor 1... 0 is stopped, 255 full forward...
      analogWrite(R_PWM, 0);
      analogWrite(L_PWM, motor1Speed);
    } else {
      // This is Stopped
      analogWrite(L_PWM, 0);
      analogWrite(R_PWM, 0);
    }
  }
}
