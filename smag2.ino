#include<Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
#include <DHT.h>
#define dht_spin A1
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
dht DHT;
const int rainsensorMin =140;     // sensor minimum

const int rainsensorMax = 1024;  // sensor maximum
int moistsensorPin = A2;  
int moistsensorValue = 0;  
int percent = 0;
int relaypin =9;
int state = '2';
//int flag=0; 
void setup(void) {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.clear();
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
   lcd.home ();
Serial.begin(9600);
 sensors.begin();
  pinMode(relaypin, OUTPUT);
 digitalWrite(relaypin, HIGH);
delay(1000);
}

void loop(void) {
 //int state = 2;
  int count=2;
  // put your main code here, to run repeatedly:
 int rainsensorReading = analogRead(A0);
 int range = map(rainsensorReading, rainsensorMin, rainsensorMax, 0, 3);
   
 lcd.clear();
 lcd.print("SMART"); 
 lcd.setCursor(0,1);
 lcd.print("AGRICULTURE");
 delay(1000);
// range value:

  switch (range) 
{

 case 0:  
  // Sensor getting wet
lcd.clear();
    Serial.println(" It's Raining");
    lcd.print("Raining");
   
    break;

 case 1:  
  // Sensor getting wet
  lcd.clear();
  Serial.println("Rain Warning");
  lcd.print("Rain Warning");

    break;
 
case 2:
    // Sensor dry - To shut this up delete the " Serial.println("Not Raining"); " below.
lcd.clear();
    Serial.println("Not Raining");
lcd.print("Not Raining");
    break;

  }
  delay(1000);
  lcd.clear();
 DHT.read11( dht_spin);
  Serial.print("current humidity");
  lcd.print("humidity=");
Serial.print(DHT.humidity);
lcd.print(DHT.humidity);
Serial.print("%");
lcd.print("%");
lcd.setCursor(0,1);
lcd.print("Temp=");
Serial.print("temperature=");
Serial.print(DHT.temperature);
lcd.print(DHT.temperature);
Serial.println("C");
lcd.print("C");
delay(1000);
 sensors.requestTemperatures();
   Serial.print("Soil Temperature ==: ");
   lcd.clear();
   lcd.print("SoilTemp=");
  Serial.print(sensors.getTempCByIndex(0));
  lcd.print(sensors.getTempCByIndex(0));
 Serial.println("C");
  lcd.print("C");
 moistsensorValue = analogRead(moistsensorPin);
  percent = convertToPercent(moistsensorValue);
  printValuesToSerial();
  delay(1500);
 if(Serial.available() == 1){
 state = Serial.read();

 }
 if(state=='0'){
  digitalWrite(relaypin,HIGH);
  lcd.clear();
  lcd.print("MotorPump->STOP");
  //delay(500);
 }else if(state=='1'){
  digitalWrite(relaypin,LOW);
  lcd.clear();
  lcd.print("MotorPump->START");
  //delay(500);
 if(range==0){
  lcd.clear();
  lcd.print("You Start Motor");
  lcd.setCursor(0,1);
  lcd.print("In Rain");
 delay(500);
 }
 }else if(state == '2'){
  if(range==0){
    digitalWrite(relaypin,HIGH);
    lcd.clear();
    lcd.print("MotorPump->STOP");
   // delay(500);
  }else {
    if(percent<=50){
       digitalWrite(relaypin,LOW);
    lcd.clear();
    lcd.print("MotorPump->START");
    //delay(500);
    }else if(percent>=80){
       digitalWrite(relaypin,HIGH);
    lcd.clear();
    lcd.print("MotorPump->STOP");
    //delay(500);
    }
  }
 }

  delay(1000);
}
int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 150, 0, 100);
  return percentValue;
}

void printValuesToSerial()
{
  Serial.print("\nmoisture Percent: ");
  lcd.setCursor(0,1);
  lcd.print("SoilMoist->");
  Serial.print(percent);
  lcd.print(percent);
  lcd.print("%");
  Serial.println("%");
}

