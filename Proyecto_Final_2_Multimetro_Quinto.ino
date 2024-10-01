/*
   Fundacion Kinal
   Centro educativo tecnico laboral kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5AV
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: Proyecto Final II Arduino Electronica
   Dev: Diego Alejandro Felipe Chis
   Profesor: Alejandro Navas
   Fecha: 01 de octubre de 2024
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define widht_oled 128
#define height_oled 64
#define InputVoltage A1
#define dividerInput A2
#define RC 1000

Adafruit_SSD1306 oled_display(widht_oled, height_oled, &Wire, -1);

int Sensor = A3;
int pot = A0;
int Valpot;
int LEDI = 3;
int LEDV = 4;
int LEDR = 5;

float sensibilidad = 0.185;
float I = 0.0000;

void setup() {
  Serial.begin(9600);
  pinMode(LEDI, OUTPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(LEDR, OUTPUT);
  if(!oled_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("ERROR");
    while(1);
  }
  oled_display.clearDisplay();
  oled_display.setTextSize(1);
  oled_display.setTextColor(SSD1306_WHITE);
  oled_display.display();
}

float getVoltage(int analogPin){
  int bits = analogRead(analogPin);
  float dividerVoltage = (bits * 5.0) / 1023;
  return dividerVoltage;
}

float getAvgVoltages(float &avgDividerVoltage, float &avgInputVoltage){
  float dividerVoltage = 0;
  float referenceVoltage = 0;
  for(int i = 0; i < 10; i++){
    dividerVoltage += getVoltage(dividerInput);
    referenceVoltage += getVoltage(InputVoltage);
  }
  avgDividerVoltage = dividerVoltage/10.0;
  avgInputVoltage = referenceVoltage/10.0;
}

float getResistanceValue(float v2, float v1){
  float resistance = (v2*RC)/(v1-v2);
  return resistance;
}

void Oled(float R){
  oled_display.println(R);
  oled_display.display();
  Serial.print("Resistencia: ");
  Serial.print(R);
  Serial.println("Ohms");
}

float promedio_I(int muestras_I){
  float sensorA6;
  float intencidad = 0;
  for(int i = 0; i < muestras_I; i++){
    sensorA6 = analogRead(A6) * (5.0 / 1023.0);
    intencidad = intencidad + (sensorA6 - 2.5)/sensibilidad;
  }
  intencidad = intencidad / muestras_I;
  return(intencidad);
}

void loop() {
  float avgDividerVoltage = 0;
  float avgInputVoltage = 0;
  getAvgVoltages(avgDividerVoltage, avgInputVoltage);
  float resistance = getResistanceValue(avgDividerVoltage, avgInputVoltage);
  Valpot = analogRead(pot);
  oled_display.clearDisplay();
  oled_display.setCursor(0, 0);
  oled_display.println("Multimetro");
  oled_display.setCursor(0, 10);

  if(Valpot >= 0&&Valpot <= 340){
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDV, LOW);
    digitalWrite(LEDI, LOW);
    oled_display.println("Val. de resistencia:");
    oled_display.setCursor(0, 20);
    Oled(resistance);
    delay(1000);
  }
  if(Valpot >= 341&&Valpot <= 680){
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDV, HIGH);
    digitalWrite(LEDI, LOW);
    float voltaje = (float)25 * analogRead(A3)/1023;
    Serial.print("Voltaje: ");
    Serial.print(voltaje);
    Serial.println("V");
    oled_display.println("Val. de voltaje:");
    oled_display.setCursor(0,20);
    oled_display.println(voltaje);
    oled_display.display();
    delay(1000);
  }
  if(Valpot >= 681&&Valpot <= 1023){
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDV, LOW);
    digitalWrite(LEDI, HIGH);
    oled_display.println("Val. de corriente:");
    oled_display.setCursor(0,20);
    oled_display.println(I);
    oled_display.display();

    I = promedio_I(500);
    if(I >= 0.01){
      Serial.print("Corriente: ");
      Serial.print(I);
      Serial.println("A");
      delay(1000);
    }else{
      Serial.println("0.0000");
      I = 0.0000;
      delay(1000);
    }
  }
}