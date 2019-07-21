#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h> 
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);  
#define dht_dpin D4
#include <NTPClient.h>      
#include <WiFiUdp.h>
#define NTP_OFFSET   +19800      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

#define FIREBASE_HOST "my-solr.firebaseio.com"                         //database api url
#define FIREBASE_AUTH "Hgj0eZAqaVeir7Sx8CJvKxLHABx0qsp01Tr3V4sa"           //database secret
DHT dht(dht_dpin, DHTTYPE); 
const int LDR = A0; // Defining LDR PIN 
int input_val = 0;  // Varible to store Input values

const char* ssid="aasimahmad";
const char* password = "12345678";

void setup() {
   Serial.begin(9600); 
   dht.begin();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );
  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  s.begin(9600);
  while (!Serial) continue;
  timeClient.begin();
}

void loop() {  
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    input_val = analogRead(LDR);      // Reading Input
    StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 //Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  //Serial.print("Data 1 ");
  //Serial.println("");
  float data1=root["data1"];
  //Serial.print(data1);
  //Serial.print("   Data 2 ");
  float data2=root["data2"];
  //Serial.print(data2);
  //Serial.println("");
  //Serial.println("---------------------xxxxx--------------------");
    timeClient.update();
  unsigned long epochTime =  timeClient.getEpochTime();
 

JsonArray& array1 = jsonBuffer.createArray();
JsonObject& object1 = array1.createNestedObject();
object1["timestamp"] = epochTime;
object1["value"] = data1;

Firebase.push("allusers/-LegnEE3wBkz8ITSZb0g/data/voltage/",array1);

JsonArray& array2 = jsonBuffer.createArray();
JsonObject& object2 = array2.createNestedObject();
object2["timestamp"] = epochTime;
object2["value"] = data2;

Firebase.push("allusers/-LegnEE3wBkz8ITSZb0g/data/current/",array2);

JsonArray& array3 = jsonBuffer.createArray();
JsonObject& object3 = array3.createNestedObject();
object3["timestamp"] = epochTime;
object3["value"] = h;

Firebase.push("allusers/-LegnEE3wBkz8ITSZb0g/data/humidity/",array3);

JsonArray& array4 = jsonBuffer.createArray();
JsonObject& object4 = array4.createNestedObject();
object4["timestamp"] = epochTime;
object4["value"] = t;

Firebase.push("allusers/-LegnEE3wBkz8ITSZb0g/data/temperature/",array4);

JsonArray& array5 = jsonBuffer.createArray();
JsonObject& object5 = array5.createNestedObject();
object5["timestamp"] = epochTime;
object5["value"] = input_val;

Firebase.push("allusers/-LegnEE3wBkz8ITSZb0g/data/intensity/",array5);
delay(5000);
    
}
