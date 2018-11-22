#define FRAME_SIZE 20
#define MOTION_SENSOR_PIN A0
#define LED_DECREMENT 75
#define LED_DIM_LEVEL 120

//V1.0
uint8_t AxisLED[3] = {5, 6, 9};
uint8_t AxisMotion[3] = {A0, A1, A2};
uint8_t tolerance[3] = {160, 160, 254};
int Val[3] = {LED_DIM_LEVEL, LED_DIM_LEVEL, LED_DIM_LEVEL};


struct FrameBuffer {
  uint16_t motion[FRAME_SIZE];   //Array of the motion sensor readings
  uint8_t pointer;
  uint8_t average;           //the position in array to place new reading
};

FrameBuffer AxisReadings[3] = {0};



void CAL_Average(FrameBuffer *axisReading)
{
  uint16_t temp = 0;
  for (int i = 0; i < FRAME_SIZE; i++)
  {
    temp += axisReading->motion[i];
  }
  axisReading->average = temp / FRAME_SIZE;
}

// Interrupt is called once a millisecond,
SIGNAL(TIMER0_COMPA_vect)
{
  //    V1.0
  for (int i = 0; i < 3; i++)
  {
    AxisReadings[i].motion[AxisReadings[i].pointer] = analogRead(AxisMotion[i]);
    //    CAL_Average(&AxisReadings[i]);
    AxisReadings[i].pointer++;
    if (AxisReadings[i].pointer >= FRAME_SIZE)
    {
      AxisReadings[i].pointer = 0;
    }
    if (AxisReadings[i].average > tolerance[i])
    {
      Val[i] = 25500;
    }
    if (Val[i] > LED_DIM_LEVEL)
    {
      Val[i] -= LED_DECREMENT;
    }
    else
    {
      Val[i] = LED_DIM_LEVEL;
    }
    analogWrite(AxisLED[i], Val[i] / 100);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 3; i++)
  {
    pinMode(AxisMotion[i], INPUT);
  }
  for (int i = 0; i < 3; i++)
  {
    pinMode(AxisLED[i], OUTPUT);
  }
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xFF;
  TIMSK0 |= _BV(OCIE0A);


}

void loop() {
  // put your main code here, to run repeatedly:

  //Serial.print(AxisReadings[0].average);
  //Serial.print("\t");
  //Serial.print(AxisReadings[1].average);
  //Serial.print("\t");
  //Serial.println(AxisReadings[2].average);



  //V1.0
  for (int i = 0; i < 3; i++)
  {
    CAL_Average(&AxisReadings[i]);

  }

  //  V2.0

}
