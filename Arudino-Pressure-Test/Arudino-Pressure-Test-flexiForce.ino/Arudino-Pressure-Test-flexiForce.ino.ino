#include <Bridge.h>
#include <HttpClient.h>

char DEVID1[] = "v5AD1C1EB2754169"; //Scenario : "Shave-Butter-Device"
boolean DEBUG = true;
char serverName[] = "api.pushingbox.com";
const int tiltSensor = 2; 
int capWeight = A0;
int tiltReading;
int tiltState;  // the current reading from the input pin
int lastTiltState = HIGH; // the previous reading from the input pin

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;  // the debounce time; increase if the output flickers

const int statusTrigger = A1;
const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not
int statusReading = 0;  // variable to store the value read from the sensor pin
 

void setup(){
  Serial.begin(9600);
  pinMode(capWeight, INPUT);
  pinMode(tiltSensor, INPUT);
  while (!Serial);
  Bridge.begin();
}

void loop() {
  int capWeight = analogRead(capWeight); 
  tiltReading = digitalRead(tiltSensor);
  statusReading = analogRead(statusTrigger);
  Serial.println(capWeight);
  Serial.println(tiltReading);
  Serial.println(statusReading);

     // check to ignore any noise 
  if (tiltReading != lastTiltState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
   
 if (statusReading >= threshold) {
    readValues();
  }
  delay(250);
}

void readValues() {
     if (capWeight >= 140 && capWeight <= 200 && tiltReading == LOW) { 
     delay(1250);
      if (capWeight >= 140 && capWeight <= 200 && tiltReading == LOW) {
        sendToPushingBox(DEVID1);
     }
     else {
       return;
      }
    } 
}

void sendToPushingBox(char devid[]) {
  String APIRequest;
  
  Serial.println("Connecting to PushingBox ...");
  HttpClient client; 
  Serial.println("connected");
  Serial.println("sending request");
  
  //build API request
  APIRequest = String(serverName) + "/pushingbox?devid=" + String(devid);
  
  client.get (APIRequest);
    //DEBUG part
    // This write the response from PushingBox Server.
    // You should see a "200 OK"
    if (client.available()) 
    {
      char c = client.read();
      if(DEBUG){Serial.print(c);}
    }  
}
