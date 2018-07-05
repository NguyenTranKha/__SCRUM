//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const String host = "airsniffer.org";
const int httpsPort = 443;
String url = "/api/pollutantvalues";
WiFiClientSecure client;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int DHTPIN = D0;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);
int gas_din=D4;
int gas_ain=A0;
int ad_value;

void setup()
{
  Serial.begin(9600);
  pinMode(gas_din,INPUT);
  pinMode(gas_ain,INPUT);
  WiFi.begin("TechMaster", "21112017");
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  
  dht.begin();         // Khởi động cảm biến
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(5,0);
  lcd.print("ZERO 9");
  delay(3000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("One for Nine");
  lcd.setCursor(2,1);
  lcd.print("Nine for One");
  delay(3000);
}


void loop()
{
  
  //arduino lcd.nobacklight
  int h = dht.readHumidity();    //Đọc độ ẩm
  int t = dht.readTemperature(); //Đọc nhiệt độ
  int f = dht.readTemperature(true);
  ad_value=analogRead(gas_ain);
  ad_value = 0;

  StaticJsonBuffer<1000> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSgps = JSONbuffer.createObject();
    JsonObject& JSgpsLocation = JSONbuffer.createObject();
    JsonObject& JSsum = JSONbuffer.createObject();
    JsonObject& JSvalues = JSONbuffer.createObject();
    JsonObject& JSTS1 = JSONbuffer.createObject();
    JsonObject& JSTS11 = JSONbuffer.createObject();
    JsonObject& JSTS2 = JSONbuffer.createObject();
    JsonObject& JSTS22 = JSONbuffer.createObject();
    JsonObject& JSTS3 = JSONbuffer.createObject();
    JsonObject& JSTS33 = JSONbuffer.createObject();
    JsonObject& JSTS4 = JSONbuffer.createObject();
    JsonObject& JSTS44 = JSONbuffer.createObject();

   
    JSgpsLocation["sourceId"] = "kckUd7LtaQ";
    JSgpsLocation["macAddress"] = "00-00-00-00-00-04";
    JSgps["latitude"] = "10.870385";
    JSgps["longitude"] = "106.80308";
    JSgpsLocation["gpsLocation"] = JSgps;
    JSsum["source"] = JSgpsLocation;

    JSTS11["value"] = (int)h;
    JSTS22["value"] = (int)t;
    JSTS33["value"] = (int)ad_value;
    JSTS44["value"] = (int)ad_value;
    
    
    JSTS1["code"] = "HUM";
    JSTS1["sensor"] = "Plan Tower";
    JSTS1["value"] = JSTS11;
    JSTS2["code"] = "TEMP";
    JSTS2["sensor"] = "Plan Tower";
    JSTS2["value"] = JSTS22;
    JSTS3["code"] = "PM2.5";
    JSTS3["sensor"] = "Plan Tower";
    JSTS3["value"] = JSTS33;
    JSTS4["code"] = "PM10";
    JSTS4["sensor"] = "Plan Tower";
    JSTS4["value"] = JSTS44;

    JsonArray& values = JSsum.createNestedArray("values"); //JSON array
    values.add(JSTS1); 
    values.add(JSTS2); 
    values.add(JSTS3);
    values.add(JSTS4);
 
    String JSONmessageBuffer;
    JSsum.prettyPrintTo(JSONmessageBuffer);
    Serial.println(JSONmessageBuffer);
    String a = String(JSONmessageBuffer);
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status    
    
    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
    Serial.println("Success");
  }
  client.print("POST /api/pollutantvalues HTTP/1.1\n");
  client.print("Host: airsniffer.org\n");
  client.print("Connection: close\n");
  client.print("Content-Type: application/json\n");
  client.print("Content-Length: ");
  client.print(a.length());
  client.print("\n\n");
  client.print(a);
      
  String str = client.readStringUntil('\n');
  Serial.print(str);

  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.setCursor(1,0);
    lcd.print("Failed to read from DHT sensor!");
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  String Temperature = "Temperature:" + String(t) + "'C";
  String Humidity = "Humidity:" + String(h) + "%";
  String Gas = "GAS:"+String(ad_value)+"%";
  String StateH="Normal";//////Trang thai canh bao Do Am
  String StateT="Normal";//////Trang thai canh bao Nhiet Do
  String StateG="CO not leak";
      if(t < 20)
        {
          StateT = "Cold";//////////Luu trang thai hien tai dang xay ra cua nhiet do
        }
      if(t > 30)
        {
          StateT = "Hot";///////////Luu trang thai hien tai dang xay ra cua nhiet do
        }
      if(h > 85)
        {
          StateH = "Wet";///////////Luu trang thai hien tai dang xay ra cua do am
        }
      if(h < 60)
        {
          StateH = "Dried";////////Luu trang thai hien tai dang xay ra cua do am
        }
      if(digitalRead(digitalRead(gas_din) == HIGH))
        {
          StateG = "CO leak";
        }
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(Temperature);/////Hien thi lcd trang thai canh bao nhiet do
   lcd.setCursor(0,1);
   lcd.print(StateT);/////Hien thi lcd trang thai canh bao nhiet do
   delay(3000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(Humidity);/////Hien thi lcd trang thai canh bao nhiet do
   lcd.setCursor(0,1);
   lcd.print(StateH);/////Hien thi lcd trang thai canh bao nhiet do
   delay(3000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(Gas);/////Hien thi lcd trang thai canh bao nhiet do
   lcd.setCursor(0,1);
   lcd.print(StateG);/////Hien thi lcd trang thai canh bao nhiet do
   delay(3000);
   

  Serial.print("Nhiet do: ");
  Serial.println(t);               //Xuất nhiệt độ
  Serial.print("Do am: ");
  Serial.println(h);               //Xuất độ ẩm
  
  Serial.println();                //Xuống hàng
  delay(1000);                     //Đợi 1 giây
}
