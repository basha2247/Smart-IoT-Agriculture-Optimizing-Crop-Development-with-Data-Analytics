#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>  // Including library for dht
 
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String apiKey = "YY7W4EF1FDB45OHY"; // Enter your Write API key from ThingSpeak
const char *ssid = "POCO X3";     // replace with your wifi ssid and wpa2 key
const char *pass = "7780190077";
const char* server = "api.thingspeak.com";

#define DHTPIN 0          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
const int sensor_pin = 16;  // Connect Soil moisture analog sensor pin to A0 of NodeMCU
WiFiClient client;

void TempHum(float t,float h){

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
  {  
                      
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(t);
    postStr +="&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
  }

}
void Moist(int moisture_percentage){

    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");
    display.clearDisplay();
    display.setCursor(0,0);  //oled display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("Soil Moisture Monitor");
    display.setCursor(40,20);  //oled display
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print(moisture_percentage);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(" %");
    display.display();
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field3=";
    postStr += String(moisture_percentage);
    postStr += "r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Data Send to Thingspeak");
  }
}

void IntenSity(float voltage){
  
  // print out the value you read:
  Serial.println(voltage);
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field4=";
    postStr += String(voltage);
    postStr += "r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Data Send to Thingspeak");
  }
    client.stop();
    Serial.println("Waiting...");
    delay(500);      // thingspeak needs minimum 15 sec delay between updates.
}
void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Connecting to ");
  display.setTextSize(2);
  display.print(ssid);
  display.display();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("WiFi connected");
    display.display();
    delay(4000);
}
void loop()
{
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  TempHum(t,h);    
  int moisture_percentage;
  moisture_percentage = ( 100.00 - ( (digitalRead(sensor_pin)/1023.00) * 100.00 ) );
  Moist(moisture_percentage);

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue* (100.0 / 1023.0);
  int IntenSity(voltage);
}