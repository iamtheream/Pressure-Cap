const char * DEVID1 = "v5AD1C1EB2754169"; //Scenario : "Shave-Butter-Device"
boolean DEBUG = true;
const char * serverName = "api.pushingbox.com";
const int tiltSensor = D2; 
int capWeight = A0;
int tiltReading;
int tiltState;
int lastTiltState = HIGH;

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

const int statusTrigger = A1;
const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not
int statusReading = 0;  // variable to store the value read from the sensor pin

TCPClient client;


void setup(){
  Serial.begin(9600);
  pinMode(capWeight, INPUT);
  pinMode(tiltSensor, INPUT);
}

void loop(){
  int capWeight = analogRead(capWeight); 
  tiltReading = digitalRead(tiltSensor);
  statusReading = analogRead(statusTrigger);
  Serial.println(capWeight);
  Serial.println(tiltReading);
  Serial.println(statusReading);

    // checking for noice
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
        notification();
     }
     else {
       return;
      }
    } 
}

void notification()
{
  if (client.connect("api.pushingbox.com", 80))
  {
    out("GET /pushingbox?devid=v5AD1C1EB2754169  HTTP/1.1\r\n");
    out("Host: api.pushingbox.com\r\n");
    out("User-Agent: Spark/1.0\r\n");
    out("Content-Type: text/html\r\n");
    out("Connection: close\r\n\r\n");
    //DEBUG_PRINTLN("Closing Connection...");
    Serial.println("Complete");
  }
      else{
        Serial.println("connection failed");
    }
}

void out(const char *s)
{
  client.write( (const uint8_t*)s, strlen(s) );
  //Serial.println("Writing");
}

void in(char *ptr, uint8_t timeout)
{
  int pos = 0;
  unsigned long lastTime = millis();
  while( client.available()==0 && millis()-lastTime<timeout)
  {
  }  //do nothing
  lastTime = millis();
  unsigned long lastdata = millis();
  while ( client.available() || (millis()-lastdata < 500))
  {
    if (client.available())
    {
      char c = client.read();
      //DEBUG_PRINTLN(c);
      lastdata = millis();
      ptr[pos] = c;
      pos++;
    }
    if (pos >= 512 - 1)
      break;
  }
  ptr[pos] = '\0'; //end the char array
  while (client.available()) client.read(); // makeshift client.flush()
  client.flush();  //for safety
  delay(400);
  client.stop();
}

