int sensorPins[] = { A1, A0, A3, A2 };    
int sensorValues[] = { 0, 0, 0, 0 };
unsigned long debounce[] = { 0, 0, 0, 0 }; 
int keys[] = { KEY_A, KEY_S, KEY_K, KEY_L };
bool triggerState[] = { false, false, false, false };

int ledPins[] = {3, 4, 5, 6 };      // select the pin for the LED
int trigVals[] = { 250, 400, 780, 780 };
int numSensors = 4;

void setup() {
  // declare the ledPin as an OUTPUT:
  for (int i = 0; i < numSensors; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
  analogReadAveraging(32);
  for (int i = 0; i < numSensors; i++) {
      int trigVal = analogReadAverage(sensorPins[i])-100;
      if (trigVal <= 11) {
        trigVal = 11;
      }
      trigVals[i] = analogReadAverage(sensorPins[i])-100;
  }
  Keyboard.begin();
}


int avrSize = 32;
int analogReadAverage(int channel)
{
  int sampleBuf[8];
  sampleBuf[0] = analogRead(channel);
  for (byte i = 1; i  < 8; i++) {
    int reading = analogRead(channel);
    int k = i-1;
    while ( k >= 0  &&  (reading < sampleBuf[k]) ) {
      sampleBuf[k+1] = sampleBuf[k];
      k--;
    }
    sampleBuf[k+1] = reading;
  }

  int returnval = 0;
  for (byte k = 2;k < 6; k++) {
    returnval += sampleBuf[k];
  }
  return returnval / 4;
}

unsigned long debounceT = 10;

void loop() {
  // read the value from the sensor:
  for (byte i = 0; i < numSensors; i++) {
    if (debounce[i] < millis()) {
      sensorValues[i] = analogRead(sensorPins[i]);
      
      bool triggerDown = sensorValues[i] < (trigVals[i]-10);
      bool triggerUp = sensorValues[i] > (trigVals[i]+10);
      
      if (triggerDown && triggerState[i] == false) {
        debounce[i] = millis() + debounceT;
        Keyboard.press(keys[i]);
        Keyboard.release(keys[i]);
        triggerState[i] = true;
        digitalWrite(ledPins[i], HIGH); 
      }
      
      if (triggerUp && triggerState[i] == true) {
        debounce[i] = millis() + debounceT;
        triggerState[i] = false;
        digitalWrite(ledPins[i], LOW); 
      }
    //sensorValue = analogReadAverage(sensorPin);
    
    }

    
    Serial.print(sensorValues[i]);
    Serial.print("/");
    Serial.print(trigVals[i]);
    Serial.print("   --   ");
    
  }
  Serial.println(); 
}
