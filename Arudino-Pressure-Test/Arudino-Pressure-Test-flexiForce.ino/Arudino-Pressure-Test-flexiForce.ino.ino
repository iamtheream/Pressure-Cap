#include <Bridge.h>
#include <HttpClient.h>

char DEVID1[] = "v5AD1C1EB2754169"; //Scenario : "Shave-Butter-Device"

boolean DEBUG = true;

char serverName[] = "api.pushingbox.com";

int capWeight = A0;

void setup(){
  Serial.begin(9600);
  while (!Serial);
  Bridge.begin();
}

void loop(){
  int capWeight = analogRead(capWeight); 

  Serial.println(capWeight);

  if (capWeight > 140) {
   sendToPushingBox(DEVID1); 
  } 
  delay(250);
}


void sendToPushingBox(char devid[])
{
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
