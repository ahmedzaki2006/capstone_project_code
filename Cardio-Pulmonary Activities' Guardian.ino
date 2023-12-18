#define BLYNK_TEMPLATE_ID "TMPL2sOfUYpEq"
#define BLYNK_TEMPLATE_NAME "Oxmeter sensor"
#define BLYNK_AUTH_TOKEN "Lbo8GF0Am8oiHCSGwTCjyV-D_5pFkL13"
#define BLYNK_PRINT Serial


#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <Adafruit_SSD1306.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "oxmeter project";  
char pass[] = "5555522222";
BlynkTimer timer; 

#define OLED_Address 0x3C 
Adafruit_SSD1306 oled(128, 64);

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

uint32_t irBuffer[100]; 
uint32_t redBuffer[100];

#define SDA_2 33
#define SCL_2 32

int32_t bufferLength; 
int32_t spo2; 
int8_t validSPO2;
int32_t heartRate; 
int8_t validHeartRate;

int a=0;
int ECGValue=0;
int lasta=0;
int lastb=0;
bool rising=true;
int cloudSpO2=0;
bool checkST=false;
long previous =0;
char st_segment='N';
void sendSensor()
{
    Blynk.virtualWrite(V0, cloudSpO2);
    if (cloudSpo02 < 92) {
      Blynk.virtualWrite(V2, true);
    }
    else {
      Blynk.virtualWrite(V2, false)
    }
    switch(st_segment){
      case 'N':
    Blynk.virtualWrite(V1, "Normal");
    Blynk.virtualWrite(V3, true);
    Blynk.virtualWrite(V4, false);
    Blynk.virtualWrite(V5, false);
      
      break;

    case 'E':
    Blynk.virtualWrite(V1, "Elevation");
    Blynk.virtualWrite(V3, false);
    Blynk.virtualWrite(V4, true);
    Blynk.virtualWrite(V5, false);
      break;

      case 'D':
      Blynk.virtualWrite(V1, "Depression");
    Blynk.virtualWrite(V3, false);
    Blynk.virtualWrite(V4, false);
    Blynk.virtualWrite(V5, true);
      break;
    }
    
   
    }

void setup()
{
    Serial.begin(115200); 
    Wire1.begin(SDA_2, SCL_2);
    Blynk.begin(auth, ssid, pass);
    timer.setInterval(100L, sendSensor);

    if (!particleSensor.begin(Wire1, I2C_SPEED_FAST)) 
    {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
    }


    byte ledBrightness = 60;
    byte sampleAverage = 4; 
    byte ledMode = 2; 
    byte sampleRate = 100; 
    int pulseWidth = 411; 
    int adcRange = 4096; 

    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    
    bufferLength = 100;

    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
    oled.clearDisplay();
    oled.setTextSize(1);



}

void loop()
{
    ECGValue = analogRead(A0);
    Blynk.run();
    timer.run();
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data
        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample(); //We're finished with this sample so move to next sample

        Serial.print(F("red="));
        Serial.print(redBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.print(irBuffer[i], DEC);

        Serial.print(F(", HR="));
        Serial.print(heartRate, DEC);

        Serial.print(F(", HRvalid="));
        Serial.print(validHeartRate, DEC);

        Serial.print(F(", SPO2="));
        Serial.print(spo2, DEC);

        Serial.print(F(", SPO2Valid="));
        Serial.println(validSPO2, DEC);
      
      
        if (ECGValue>= 600)
        {
        checkST = true;
        }
        long current =millis();
        if (checkST &&(current - previous >= 100)){
        if (ECGValue < 275)
        {
        st_segment = 'D';
        }
        else if (ECGValue>350)
        {
        st_segment = 'E'; 
        }
        else
        {
        st_segment = 'N'; 
        }
        
        }
        else
        {
        previous = current;
        checkST = false;
        }
        
        if (validSPO2 && validHeartRate && irBuffer[i] >50000){
        
        displayOled(spo2,st_segment);
        }
      }
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
      }
  
void displayOled(int SpO2,char state){
  if(a>127)
  {
    oled.clearDisplay();
    a=0;
    lasta=a;
  }
  oled.setTextColor(WHITE);
  int b=60-(ECGValue/16);
  oled.writeLine(lasta,lastb,a,b,WHITE);
  lastb=b;
  lasta=a;
  oled.writeFillRect(0,50,128,16,BLACK);
  oled.setCursor(0,50);
  oled.print("SpO2: ");
  oled.print(SpO2);
  oled.print("%     ST:");
  oled.print(state);
  oled.display();
  a+=5;
  number();
}
