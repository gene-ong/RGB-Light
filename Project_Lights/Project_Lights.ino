#include <Adafruit_NeoPixel.h>

#define LED_DECREMENT 10
#define LED_DIM_LEVEL 2000
#define NEO_PIX_NB 5
#define NEO_PIX_PIN 4
#define AVERAGE_NB 10
#define ACCELERATION_THRESHOLD 7 //Threshold of ms above tilt threshold 


//V1.0
uint8_t AxisMotion[3] = {A0, A1, A2};
const uint16_t TILT_THRESHOLD_VALUES [3] = {420 , 500, 600};
uint16_t LEDbrightness [3] = {LED_DIM_LEVEL, LED_DIM_LEVEL, LED_DIM_LEVEL};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_PIX_NB, NEO_PIX_PIN, NEO_GRB + NEO_KHZ800);

struct FrameBuffer {
  uint16_t motion[AVERAGE_NB];   //Array of the motion sensor readings
  uint8_t pointer;
  uint16_t average;           //the position in array to place new reading
  uint16_t countAboveThreshold;
};

FrameBuffer AxisReadings[3] = {0};

// Interrupt is called once a millisecond,
SIGNAL(TIMER0_COMPA_vect)
{
  //for each axis
  //if the pin is above the threshold
  //Store a bunch of values
  //Average those values
  //If they're above a threshold, increment a counter
  //If they're below a threshol, reset a counter
  for (int i = 0; i < 3; i++)
  {
    //if the pin is above the threshold
    if (analogRead(AxisMotion[i]) > TILT_THRESHOLD_VALUES [i])
    {
      AxisReadings[i].countAboveThreshold++;
    }
    //If they're below a threshold, reset a counter
    else
    {
      AxisReadings[i].countAboveThreshold = 0;
    }

    if (AxisReadings[i].countAboveThreshold > ACCELERATION_THRESHOLD)
    {
      //      Serial.println("Number of times average has been higher than tilt threshold, creater than acceleration threshold");
      LEDbrightness [i] = 25500;
       AxisReadings[i].countAboveThreshold = 0;
    }


    if (LEDbrightness [i] > LED_DIM_LEVEL)
    {
      //      Serial.println("LEDbrightness greater than led dim level");
      LEDbrightness [i] -= LED_DECREMENT;
    }
    else
    {
      //      Serial.println("LEDbrightness set to dim level");
      LEDbrightness [i] = LED_DIM_LEVEL;
    }
  }
}

void setup()
{
  //beging serial communication
  //    Serial.begin(9600);

  // Initialize all pixels to 'off'
  strip.begin();
  strip.show();

  //Initialise pins for sensing tilt sensor
  for (int i = 0; i < 3; i++)
  {
    pinMode(AxisMotion[i], INPUT);
  }
  pinMode(NEO_PIX_PIN, OUTPUT);
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xFF;
  TIMSK0 |= _BV(OCIE0A);
}

void loop()
{
  // put your main code here, to run repeatedly:
  //

  for (uint8_t i = 0; i < 3; i++)
  {

  }
  colorWipe(strip.Color(LEDbrightness [0]/100, LEDbrightness [1]/100, LEDbrightness [2]/100)); // Red;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
