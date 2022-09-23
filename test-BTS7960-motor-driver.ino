
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
