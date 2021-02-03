int sensorPins[] = { A0, A1 };    
int sensorValues[] = { 0, 0 };
unsigned long debounce[] = { 0, 0 }; 
int keys[] = { KEY_A, KEY_S };
bool triggerState[] = { false, false };

int ledPin = 13;      // select the pin for the LED

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  analogReadAveraging(32);
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

int trigVal = 1010;
unsigned long debounceT = 10;

void loop() {
  // read the value from the sensor:
  for (byte i = 0; i < 2; i++) {
    if (debounce[i] < millis()) {
      sensorValues[i] = analogRead(sensorPins[i]);
      bool triggerDown = sensorValues[i] > (trigVal+10);
      bool triggerUp = sensorValues[i] < (trigVal-10);
      if (triggerDown && triggerState[i] == false) {
        debounce[i] = millis() + debounceT;
        Keyboard.press(keys[i]);
        Keyboard.release(keys[i]);
        triggerState[i] = true;
        digitalWrite(ledPin, HIGH); 
      }
      if (triggerUp && triggerState[i] == true) {
        debounce[i] = millis() + debounceT;
        triggerState[i] = false;
        digitalWrite(ledPin, LOW); 
      }
    //sensorValue = analogReadAverage(sensorPin);
    }
    Serial.print(sensorValues[i]);
    Serial.print(" -- ");
  }
  Serial.println(); 
}
