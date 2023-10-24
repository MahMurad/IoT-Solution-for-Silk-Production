#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define dhtpin D4
#define mqpin A0 
#define fanpin D5

int t,h,air;

WiFiClient client;
LiquidCrystal_I2C lcd(0x27,16,2); //scl=d1,sdl=d2
DHT dht(dhtpin,DHT11); //d0=dht11
long myChannelNumber = 1852937;
const char myWriteAPIKey[] = "UVC1M2W4BURW5AWO";

void setup() {
  dht.begin();
  pinMode(mqpin,INPUT);
  pinMode(fanpin,OUTPUT);
  WiFi.begin("MahM", "firef0xx");

  /*while(WiFi.status() != WL_CONNECTED)
  {
    lcd.print("Connect to WiFi");
    delay(200);
  }*/
  lcd.clear();
  /*if(WiFi.status()==WL_CONNECTED)
    lcd.print("Connected To WiFi");
    */
  delay(1000);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Welcome to Our");
  lcd.setCursor(2,1); // first character of 2nd line
  lcd.print("IoT Project!");

  ThingSpeak.begin(client);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  lcd.clear();
  delay(100);
  air = analogRead(mqpin);
  delay(100);
  h = dht.readHumidity();
  delay(100);
  t = dht.readTemperature();

  if(t<25){
    digitalWrite(fanpin,HIGH);
  }
  else{
    digitalWrite(fanpin,LOW);
  }
  
  lcd.print("Air Quality: ");
  lcd.print(air);
  delay(2000);
  lcd.clear();
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor (0,1); // go to start of 2nd line
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" degCel");
  
  
  
  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  delay(2000);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  delay(2000);
  ThingSpeak.writeField(myChannelNumber, 3, air, myWriteAPIKey);
  delay(2000);
}
