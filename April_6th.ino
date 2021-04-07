#define CH3 3 //Left & Right
#define CH9 9 //Forwards & Backwards

//For Left/Right Control
#define CH4 4         //Enable Pin,       Blue/Orange       ENABLE
#define CH8 8         //Direction Pin,    White/Brown       DIRECTION/INPUT [A]
#define CH6 6         //Step Pin; move,   Black/Yellow      STEP/INPUT      [B]
#define CH7 7         //Complete,         Green/Red         HLFB - High Level Feed Back
#define CH5 5         //MainMotorController                 Forward Speed Control
//For Break Control
#define CH10 10       //Enable Pin,       Blue/Orange       ENABLE
#define CH11 11       //Direction Pin,    White/Brown       DIRECTION/INPUT [A]
#define CH12 12       //Step Pin; move,   Black/Yellow      STEP/INPUT      [B]
#define CH13 13       //Complete,         Green/Red         HLFB - High Level Feed Back

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value

void dutycycle(float voltage, int PWM_Pin)    //Due to Arduino Uno properties, will want to use pin 5 or 6 for 980Hz
{
if(voltage > 4.3)
voltage = 4.3;

voltage = round((voltage/5)*255);

analogWrite(PWM_Pin, voltage);  

  Serial.print("DutyCycle Input is: ");
  Serial.print(voltage);
  Serial.println();
}


int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue, int &forward){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  int position = map(ch, 1000, 2000, minLimit, maxLimit);
  forward = position;
  if(position<3 & position>-3)
      position = 0;
      
  //Serial.println();
  //Serial.print("RC position is: "); Serial.print(position);
  //Serial.println();
  return position;
}


int ch3Value,ch9Value, i, a, steering_state, braking_state, last_motor_position, last_brake_position, change, steering_change,braking_change, dir, forward;

int direction(int position, bool state, int last_motor_position, int EnablePin, int DirectionPin, int StepPin){
  
  
  if(state == LOW)
  {
      dir =0;
      change =0;
      return change;
  }
  if(position>last_motor_position)
  {
      dir =HIGH;
      change = 1;
      pulsemotor(dir, 1, EnablePin, DirectionPin, StepPin);
      return change;
  }
  else 
  {
      dir =LOW;
      change = -1;
      pulsemotor(dir, 1, EnablePin, DirectionPin, StepPin);
      return change;
  }
}

void pulsemotor(bool direction, bool state, int EnablePin, int DirectionPin, int StepPint)
{

  digitalWrite(EnablePin, state); //Enables motor for move instructions
  digitalWrite(DirectionPin, direction);  //Sets direction

for(int a = 0;a<5;a++)
{
  digitalWrite(StepPint, HIGH);
  delayMicroseconds(1);
  digitalWrite(StepPint, LOW);
}


  
//  digitalWrite(4, LOW); //Disables motor for move instructions

}

void setup() {
  dir, last_motor_position, last_brake_position, i, a, steering_change, braking_change, ch3Value, ch9Value, forward = 0;

  Serial.begin(115200);

  pinMode(CH3, INPUT);
  pinMode(CH7, INPUT);
  pinMode(CH6, OUTPUT);
  pinMode(CH8, OUTPUT);
  pinMode(CH4, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(6, LOW);


  pinMode(CH9, INPUT);
  pinMode(CH13, INPUT);
  pinMode(CH12, OUTPUT);
  pinMode(CH11, OUTPUT);
  pinMode(CH10, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(12, LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
for(i == 0; i<1500; i++)
{
  ch3Value = readChannel(CH3, -100, 100, 0, forward);
  ch9Value = readChannel(CH9, -100, 100, 0, forward);
  if(ch9Value>0)
  ch9Value=0;
  dutycycle((forward/40)*5, 5);
  
  Serial.print("Desired Brake Position is: ");
  Serial.print(ch9Value);
  Serial.println();

 steering_state = HIGH;
 braking_state = HIGH;

  
  steering_change = direction(ch3Value, steering_state, last_motor_position, 4, 8, 6);
  pulsemotor(dir, steering_state, 4, 8, 6);
    
  braking_change = direction(ch9Value, braking_state, last_brake_position, 10, 11, 12);
  pulsemotor(dir, braking_state, 10, 11, 12);
  
  last_motor_position = last_motor_position + steering_change;
  last_brake_position = last_brake_position + braking_change;

  
  
  Serial.print("Loop cycle is: ");
  Serial.print(i);
  Serial.println();
  Serial.print("Last Steering Position is: ");
  Serial.print(last_motor_position);
  Serial.println();
  Serial.println();
  Serial.print("Last Braking Position is: ");
  Serial.print(last_brake_position);
  Serial.println();
  //Serial.print("Direction desired is: ");
  //Serial.print(dir);
  Serial.println();
  Serial.print("****************************");
  Serial.println();
  
}
  digitalWrite(4, LOW);
  digitalWrite(6, LOW);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  dir, last_motor_position, i, a, steering_change, ch3Value, ch9Value = 0;
   steering_state, braking_state = LOW;
  exit;

}
