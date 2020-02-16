#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
//#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#define REDLED D1
#define HEARTBEATSENSOR A0
#define UpperThreshold 560
#define LowerThreshold 530
#define FIREBASE_HOST "https://save-the-world-v1.firebaseio.com/"
#define FIREBASE_KEY "OzJXJAl7Hr42r4oV4grXITSwAlPVLY7GdnMkP0nB"
int x=0;
int lastx=0;
int lasty=0;
int ThisTime;
int LastTime=0;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;

const char* ssid = "treehacks_24ghz";

void setup() {
  pinMode(REDLED,OUTPUT);
  pinMode(HEARTBEATSENSOR, OUTPUT);
  Serial.begin(9600);
  Serial.print("Attempting to connect\n");
  Serial.println();
  Serial.print("Attempting now");
  Serial.println();
  WiFi.begin(ssid);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("Still Waiting\n");
  }
  Serial.print("Connected!\n");
  for(int i = 0; i < 3; i++)
  {
  digitalWrite(REDLED, HIGH);
  delay(200);
  digitalWrite(REDLED, LOW);
  delay(200);  
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.setFloat("number", 42.0); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
   
  // update value 
  Firebase.setFloat("number", 43.0); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000);  
  //.setString("Hello", "hello");
Firebase.setString("message", "hello world"); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /message failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
}

void loop() 
{
  /*float t = 99.9;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["temperature"] = t;
  String hai = Firebase.push("/sensor/dht", root);
*/

 if(x>127)  
  {
    //oled.clearDisplay();
    x=0;
    lastx=x;
  }
 
  ThisTime=millis();
  int value=analogRead(HEARTBEATSENSOR);
  
  // calc bpm
 
  if(value>UpperThreshold)
  {
    if(BeatComplete)
    {
      BPM=ThisTime-LastTime;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      BeatComplete=false;
      //tone(8,1000,250);
    }
    if(BPMTiming==false)
    {
      delay(20);
      LastTime=millis();
      BPMTiming=true;
    }
  }
  if((value<LowerThreshold)&(BPMTiming))
    BeatComplete=true;
    
  
if(BPM >= 60 && BPM <= 175)
{
  digitalWrite(REDLED, HIGH);
  Serial.print("BPM ");
  Serial.print(BPM);
  Serial.println();
}
else
  digitalWrite(REDLED, LOW);

}
