int count, sensor = 3;
double rpm;
float val = -1, motorspeed, prevmotorspeed = -1;
int valentered = 0;
long time1, time2, flag = 0, maxrpm = 0, flagy = 0, x = 1, flagx = 0;
float optkp = 0, dif = 0, kp = 0.5;
void setup() {
  // put your setup code here, to run once:
  pinMode(sensor, INPUT);
  pinMode(9, OUTPUT);
  count = 0;
  flag = 0;
  Serial.begin(9600);
  time1 = 0;
  attachInterrupt(digitalPinToInterrupt(sensor), countrpm, RISING);
  analogWrite(9,255);
}
void countrpm() {
  count++;
  //Serial.println(count);
}
void calcrpm() {
  rpm = (60000 * count) / (millis() - time1);
  count = 0;
  time1 = millis();
  time2 = millis();
  Serial.print("millis:");
  Serial.println(time2);
  Serial.print("rpm:");
  Serial.println(rpm);
  //Serial.print("kp:");
  //Serial.println(optkp);
}
void motormap() {
  float error = rpm - val,prevval = val;
  if(flagx == 0) optkp = kp;
  dif = 0.06; 
  if(prevval != val){
    kp = 0;
    if(prevval >= val)
    prevmotorspeed = -1;
    else 
     prevmotorspeed = 12345;
  }
  kp -= dif;
  if( kp < 0 ) kp = 0;
    motorspeed = motorspeed - (optkp * error * 0.31875);
  if (motorspeed > 255) motorspeed = 255;
  if(motorspeed <= 0) motorspeed = 0;
  if (motorspeed < 20 && motorspeed != 0) motorspeed = 20;
  if(val==-1) motorspeed = 255;
  if(error < 0) {
  if((rpm > prevmotorspeed/0.31875 )){
      kp /= 3;
      if(flagy == 0) {
          flagy = 1;
          optkp = kp;
      }
      flagx = 1;
  }
  }
  if(error > 0) {
  if(rpm < prevmotorspeed/0.31875){
      kp /= 3;
      if(flagy == 0) {
          flagy = 1;
          optkp = kp;
      }
      flagx = 1;
  }
  }
  prevmotorspeed = motorspeed;
  analogWrite(9, motorspeed);
}

void loop() {
  if (Serial.available() > 0) {
    val = Serial.parseInt();
    if(val != -1) valentered = 1;
    flagy = 0;
    count = 0;
    flagx = 0;
    kp = 1;
    if (flag == 0) {
      motorspeed = map(val, 0, maxrpm, 0, 255);
      flag = 1;
    }
    time2 = 0;
    motormap();
  }
  if (count >= x)
  {
    calcrpm();
    motormap();
  }
  if(maxrpm < rpm) maxrpm = rpm;
}
