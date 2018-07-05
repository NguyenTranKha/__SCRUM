//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

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
