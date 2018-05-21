//declare the necessary variables
int count, interruptSensor = nonLinearDifference, motorSensor = 9;
double rpm;
float enteredVal = -1, motorSpeed, prevMotorSpeed = -1;
int enteredValFlag = 0;
long initialTime, finalTime, mapFlag = 0, maxRpm = 0, overshootKpFlag = 0, numberOfRevolutions = 1, optimizedKpFlag = 0;
int UPPER_BOUND = 255, LOWER_BOUND = 20;

//priors for the computation of proportional gain parameter
float optimizedKp = 0, linearDifference = 0.06, kp = 1, nonLinearDifference = 3, errorFactor = 0.31875;

void setup() {
  // put your setup code here, to run once:
  pinMode(interruptSensor, INPUT);
  pinMode(motorSensor, OUTPUT);
  count = 0;
  mapFlag = 0;
  Serial.begin(9600);
  initialTime = 0;
  attachInterrupt(digitalPinToInterrupt(interruptSensor), countRpm, RISING);
  analogWrite(motorSensor,255);
}

void countRpm() {
  count++;
  //Serial.println(count);
}

void calcRpm() {
  rpm = (60000 * count) / (millis() - initialTime);
  count = 0;
  initialTime = millis();
  finalTime = millis();
  Serial.print("millis:");
  Serial.println(finalTime);
  Serial.print("rpm:");
  Serial.println(rpm);
  //Serial.print("kp:");
  //Serial.println(optimizedKp);
}
void motorMap() {
  float error = rpm - enteredVal,prevval = enteredVal;
  if(optimizedKpFlag == 0) optimizedKp = kp;
  linearDifference = 0.06; 
  if(prevval != enteredVal){
    kp = 0;
    if(prevval >= enteredVal)
    prevMotorSpeed = -1;
    else 
     prevMotorSpeed = 12345;
  }
  kp -= linearDifference;
  if( kp < 0 ) kp = 0;
    motorSpeed = motorSpeed - (optimizedKp * error * errorFactor);
  if (motorSpeed > UPPER_BOUND) motorSpeed = UPPER_BOUND;
  if(motorSpeed <= 0) motorSpeed = 0;
  if (motorSpeed < LOWER_BOUND && motorSpeed != 0) motorSpeed = LOWER_BOUND;
  if(enteredVal==-1) motorSpeed = UPPER_BOUND;
  if(error < 0) {
  if((rpm > prevMotorSpeed/errorFactor)){
      kp /= nonLinearDifference;
      if(overshootKpFlag == 0) {
          overshootKpFlag = 1;
          optimizedKp = kp;
      }
      optimizedKpFlag = 1;
  }
  }
  if(error > 0) {
  if(rpm < prevMotorSpeed/errorFactor){
      kp /= nonLinearDifference;
      if(overshootKpFlag == 0) {
          overshootKpFlag = 1;
          optimizedKp = kp;
      }
      optimizedKpFlag = 1;
  }
  }
  prevMotorSpeed = motorSpeed;
  analogWrite(motorSensor, motorSpeed);
}

void loop() {
  if (Serial.available() > 0) {
    enteredVal = Serial.parseInt();
    if(enteredVal != -1) enteredValFlag = 1;
    overshootKpFlag = 0;
    count = 0;
    optimizedKpFlag = 0;
    if (mapFlag == 0) {
      motorSpeed = map(enteredVal, 0, maxRpm, 0, 255);
      mapFlag = 1;
    }
    finalTime = 0;
    motorMap();
  }
  if (count >= numberOfRevolutions)
  {
    calcRpm();
    motorMap();
  }
  if(maxRpm < rpm) maxRpm = rpm;
}
