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

void setup()
{
  Serial.begin(9600);
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
  int h = dht.readHumidity();    //Đọc độ ẩm
  int t = dht.readTemperature(); //Đọc nhiệt độ
  int f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.setCursor(1,0);
    lcd.print("Failed to read from DHT sensor!");
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if(t < 20 || t > 30 || h < 60 || h > 85) /////////////////////tap cac truong hop xay ra
  {
    String StateH="Normal:";//////Trang thai canh bao Do Am
    String StateT="Normal:";//////Trang thai canh bao Nhiet Do
    if(t < 20)
    {
      StateT = "IT'S COLD";//////////Luu trang thai hien tai dang xay ra cua nhiet do
    }
    if(t > 30)
     {
      StateT = "IT'S HOT";///////////Luu trang thai hien tai dang xay ra cua nhiet do
    }
    if(h > 85)
     {
      StateH = "IT'S WET";///////////Luu trang thai hien tai dang xay ra cua do am
    }
    if(h < 60)
     {
      StateH = "IT'S DRIED:";////////Luu trang thai hien tai dang xay ra cua do am
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(StateT);/////Hien thi lcd trang thai canh bao nhiet do
    lcd.setCursor(11,0);
    lcd.print(t,DEC);/////Hien thi lcd nhiet do luc dang canh bao
    lcd.setCursor(14,0);
    lcd.print("'C");
    lcd.setCursor(0,1);
    lcd.print(StateH);//Hien thi lcd trang thai canh bao do am
    lcd.setCursor(11,1);
    lcd.print(h,DEC);/////Hien thi lcd nhiet do luc dang canh bao
    lcd.setCursor(14,1);
    lcd.print("%");
  }else{////////////////////////xuat he so moi truong
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("T:");
        lcd.setCursor(2,0);
        lcd.print(t,DEC);
        lcd.setCursor(5,0);
        lcd.print("'C");
        lcd.setCursor(10,0);
        lcd.print("H:");
        lcd.setCursor(12,0);
        lcd.print(h,DEC);
        lcd.setCursor(15,0);
        lcd.print("%");
    }
  Serial.print("Nhiet do: ");
  Serial.println(t);               //Xuất nhiệt độ
  Serial.print("Do am: ");
  Serial.println(h);               //Xuất độ ẩm
  
  Serial.println();                //Xuống hàng
  delay(1000);                     //Đợi 1 giây
}
