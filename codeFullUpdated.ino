#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);
#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float QOV =   0.5 * 5.0; // set quiescent Output voltage of 0.5V
int offset =20;// set the correction offset value
float voltage; //internal variable for voltage
float current;

void setup()
 {
    Serial.begin(9600);
    s.begin(9600);
}
void loop()
 {
  float voltage_raw =   (5.0 / 1023.0)* analogRead(VIN);// Read the voltage from sensor
  float voltag =  voltage_raw - QOV + 0.012 ;// 0.000 is a value to make voltage zero when there is no current
  float current = voltag / 0.1;
    Serial.print("Current: ");
    Serial.print(current,2); // print the current with 2 decimal places
    Serial.println("A");
  int volt = analogRead(A1);// read the input
  double voltage = map(volt,0,1023, 0, 2500) + offset; // map 0-1023 to 0-2500 and add correction offset
  voltage /=100; // divide by 100 to get the decimal values
  Serial.print("Voltage: ");
    Serial.print(voltage); // print the current with 2 decimal places
    Serial.println("V");
    Serial.println("\n");
    Serial.println("---------------------------------------------------------------");
    delay(500);
   StaticJsonBuffer<1000> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
  root["data1"] = voltage;
  root["data2"] = current;
if(s.available()>0)
{
 root.printTo(s);
}
 }
