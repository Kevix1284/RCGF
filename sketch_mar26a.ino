#define CH3 3 //Left & Right
#define CH9 9 //Forwards & Backwards

//For Left/Right Control
#define CH4 4         //Enable Pin,       Blue/Orange       ENABLE
#define CH8 8         //Direction Pin,    White/Brown       DIRECTION/INPUT [A]
#define CH6 6         //Step Pin; move,   Black/Yellow      STEP/INPUT      [B]
#define CH7 7         //Complete,         Green/Red         HLFB - High Level Feed Back

//For Break Control
#define CH10 10       //Enable Pin,       Blue/Orange       ENABLE
#define CH11 11       //Direction Pin,    White/Brown       DIRECTION/INPUT [A]
#define CH12 12       //Step Pin; move,   Black/Yellow      STEP/INPUT      [B]
#define CH13 13       //Complete,         Green/Red         HLFB - High Level Feed Back

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  int position = map(ch, 1000, 2000, minLimit, maxLimit);
  if(position<4 & position>-4)
  {
  position = 0;
  }
      
  Serial.println();
  Serial.print("RC position is: "); Serial.print(position);
  Serial.println();
  return position;
}


int ch3Value,ch9Value, i, a, steering_state, braking_state, last_motor_position, last_brake_position, change, steering_change,braking_change;
bool dir;

int direction(int position, bool state, int last_motor_position, int EnablePin, int DirectionPin, int StepPin, bool &dir){
  
  if(state == LOW)
  {
      dir =LOW;
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

for(int a = 0;a<4;a++)
{
  digitalWrite(StepPint, HIGH);
  delayMicroseconds(1);
  digitalWrite(StepPint, LOW);
}
//  digitalWrite(4, LOW); //Disables motor for move instructions

}

void setup() {
  dir, last_motor_position, last_brake_position, i, a, steering_change, braking_change, ch3Value, ch9Value = 0;

  Serial.begin(115200);

  pinMode(CH3, INPUT);
  pinMode(CH7, INPUT);
  pinMode(CH6, OUTPUT);
  pinMode(CH5, OUTPUT);
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
for(i == 0; i<500; i++)
{
  
  ch3Value = readChannel(CH3, -100, 100, 0);
  ch9Value = readChannel(CH9, -100, 100, 0);
  if(ch9Value>0)    //Clip off Forwards values
  {
  ch9Value=0;
  }
  Serial.print("Loop cycle is: ");
  Serial.print(i);
  Serial.println();
 // Serial.print("Desired Brake Position is: ");
 // Serial.print(ch9Value);
 // Serial.println();
  Serial.print("Desired Steering Position is: ");
  Serial.print(ch3Value);
  Serial.println();
  
  //if(abs(last_motor_position - ch3Value) > 0) //If steering motor needs to move to match RC position, enable allows it to do so
  steering_state = HIGH;
  //else
 // steering_state = LOW; 

  //if(abs(last_brake_position - ch9Value) > 0) //If break motor needs to move to match RC position, enable allows it to do so
  braking_state = HIGH;
  //else
  //braking_state = LOW; 
  
  steering_change = direction(ch3Value, steering_state, last_motor_position, 4, 8, 6, dir);
  pulsemotor(dir, steering_state, 4, 8, 6);
  
  braking_change = direction(ch9Value, braking_state, last_brake_position, 10, 11, 12, dir);
  pulsemotor(dir, braking_state, 10, 11, 12);
 
  
  last_motor_position = last_motor_position + steering_change;
  last_brake_position = last_brake_position + braking_change;
  
  Serial.print("Last Steering Position is: ");
  Serial.print(last_motor_position);
  Serial.println();
  //Serial.print("Last Braking Position is: ");
  //Serial.print(last_brake_position);
  //Serial.println();
  Serial.print("****************************");
  Serial.println();
  //delay(100);
}
  digitalWrite(4, LOW);
  digitalWrite(6, LOW);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  dir, last_motor_position, i, a, steering_change, ch3Value, ch9Value = 0;
  exit;

}
