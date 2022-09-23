//int R_IS = 6;
int R_EN = 39;
int R_PWM = 10;
//int L_IS = 7;
int L_EN = 37;
int L_PWM = 13;

void setup() {
 Serial.begin(9600);
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
  int joyposHorz = 1120;
  int joyposVert = 1200;


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
  
  //initialize motor speed variables...
  int motor1Speed = 0;
  int motor2Speed = 0;
  
  //hard coded vars...
  int centerPosition = 1500;
  int signalBuffer = 40;

  
  //make sure valid signal incoming..
  if ((joyposVert > 750) && (joyposHorz > 750)) {
    //handle motor 1 controller
    //handle up position...
    if (joyposVert > (centerPosition + signalBuffer)) {
      //Determine Motor Speeds
      motor1Speed = map(joyposVert, 1500, 2000, 0, 255); // motor 1... 0 is stopped, 255 is full reverse...
      //Serial.println(motor1Speed); //center: = 1492-1496, top = 2000, bottom = 992 ////good start position might be 1550
      analogWrite(R_PWM, motor1Speed);
      analogWrite(L_PWM, 0);
      Serial.println(motor1Speed); //center: = 1492-1496, top = 2000, bottom = 992 ////good start position might be 1550

    //handle down position...
    } else if (joyposVert < (centerPosition - signalBuffer)) {
      //Determine Motor Speeds
      motor1Speed = map(joyposVert, 1500, 1000, 0, 255);// motor 1... 0 is stopped, 255 full forward...
      analogWrite(R_PWM, 0);
      analogWrite(L_PWM, motor1Speed);
    } else {
      // This is Stopped
      analogWrite(L_PWM, 0);
      analogWrite(R_PWM, 0);
    }
  
 
    //handle motor 2 controller
    //handle right position...
    if (joyposHorz > (centerPosition + signalBuffer)) {
      //Determine Motor Speeds
      motor2Speed = map(joyposHorz, 1500, 2000, 0, 255);
      analogWrite(motor2_R_PWM, motor2Speed);
      analogWrite(motor2_L_PWM, 0);

    //handle left position...
    } else if (joyposHorz < (centerPosition - signalBuffer)) {
      //Determine Motor Speeds
      motor2Speed = map(joyposHorz, 1500, 1000, 0, 255);
      analogWrite(motor2_R_PWM, 0);
      analogWrite(motor2_L_PWM, motor2Speed);
    } else {
      // This is Stopped
      analogWrite(motor2_L_PWM, 0);
      analogWrite(motor2_R_PWM, 0);
    }
   
   
  }
 

  //Serial.println(motor1Speed);
  delay(100);

}
