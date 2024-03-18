#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16,2); 
const int AirValue = 616;   //replace the value from calibration in air
const int WaterValue = 335;  //replace the value from calibration in water
int soilMoistureValue = 0;
int soilmoisturepercent=0;
#define POTENTIOMETER_PIN A2
#define RELAY_PIN 3
#define RELAY_FAN_PIN 5 //FOR FAN
#define RELAY_HEATER_PIN 13 //FOR HEATER
#define RELAY_PUMP_PIN 4
#define SOIL_LOWER_THRESHOLD 75
#define mois_pin A3
#define TEMP_UPPER_THRESHOLD 24  // upper temperature threshold
#define TEMP_LOWER_THRESHOLD 23 // LOWER TEMP THRESHOLD 
#include "DHT.h"
#define DHT11PIN 2 //DHT SENSOR
DHT dht(DHT11PIN, DHT11);
struct DHTRead {
    float humi, temp;
}; 
typedef struct DHTRead Struct;
Struct Reading()
{
  Struct S;
S.humi = dht.readHumidity();
S.temp = dht.readTemperature();
   
return S;
}
void water()
{
  int data = analogRead(POTENTIOMETER_PIN);
  int percentage = map(data, 0, 1023, 0, 100);

  if(percentage >18)
  {
  digitalWrite(RELAY_PIN,LOW );   
  delay(1000);                                  
  }
  else{
     digitalWrite(RELAY_PIN, HIGH);
    delay(1000);
  }  
}
int soil()
{
  soilMoistureValue = analogRead(mois_pin);  //Mention where the analog pin is connected on arduino
//Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
return soilmoisturepercent;
//if(soilmoisturepercent < 10)  // change this at what level the pump turns on
//{
 // Serial.println("Nearly dry, Pump turning on");
 // digitalWrite(13,HIGH);  // Low percent high signal to relay to turn on pump
//}
//else if(soilmoisturepercent >85) // max water level should be
//{
 // Serial.println("Nearly wet, Pump turning off");
 // digitalWrite(13,low);  // high percent water high signal to relay to turn on pump
//}
 
  // delay(400); 
}
void display(Struct x,int y)
{
 lcd. setCursor (0, 0);
  lcd.print("temp");
    lcd. setCursor (6, 0);
  lcd.print(x.temp);
    lcd. setCursor (10, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd. print ( "Hu" );
  lcd.setCursor(3, 1);
  lcd. print (x.humi);
lcd.setCursor(7, 1);
  lcd. print ( "So" );
  lcd.setCursor(10, 1);
  lcd. print (y);
  lcd. print ( "%" );  
}

void setup()
{
   lcd. begin ();
   lcd. backlight ();
   pinMode(mois_pin,INPUT);
   pinMode(RELAY_FAN_PIN,OUTPUT);
   pinMode(RELAY_HEATER_PIN,OUTPUT);
   pinMode(RELAY_PUMP_PIN,OUTPUT);
 
  pinMode(RELAY_PIN, OUTPUT);
  //Serial.begin(115200);
/* Start the DHT11 Sensor */
  dht.begin();
}

void loop()
{
  Struct A;
  int B;
 // float humi = dht.readHumidity();
  //float temp = dht.readTemperature();
 // Serial.print("Temperature: ");
 // Serial.print(temp);
 // Serial.print("C ");
 // Serial.print("Humidity: ");
 // Serial.println(humi);
 // delay(500);
 A=Reading();
 B=soil();
if (A.temp > TEMP_UPPER_THRESHOLD) {
     // Serial.println("Turn the fan on");
      digitalWrite(RELAY_FAN_PIN, LOW);  // turn on
    } 
      else if (A.temp <= TEMP_UPPER_THRESHOLD) {
     // Serial.println("Turn the fan off");
      digitalWrite(RELAY_FAN_PIN,HIGH);}
if (A.temp < TEMP_LOWER_THRESHOLD) {    
      digitalWrite(RELAY_HEATER_PIN,LOW);  // turn on
    } 
      else if (A.temp >= TEMP_LOWER_THRESHOLD) {
      digitalWrite(RELAY_HEATER_PIN,HIGH);}
if (B < SOIL_LOWER_THRESHOLD) {
     // Serial.println("Turn the fan on");
      digitalWrite(RELAY_PUMP_PIN,LOW);  // turn on
      }
      else if (B >= SOIL_LOWER_THRESHOLD) {
      digitalWrite(RELAY_PUMP_PIN,HIGH);}
    
 display(A,B);
 water();

 // lcd. setCursor (0, 0);
  //lcd.print("temp");
   // lcd. setCursor (6, 0);
  //lcd.print(A.temp);
   // lcd. setCursor (10, 0);
 // lcd.print("C");
 // lcd.setCursor(0, 1);
 // lcd. print ( "Humi" );
 // lcd.setCursor(6, 1);
  //lcd. print (A.humi);
delay(1000);
}