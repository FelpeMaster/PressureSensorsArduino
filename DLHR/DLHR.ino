#include <Wire.h>
#define DLHRADD 41 //10 inchs H2O

float temperature1, pressure1; // Temperature and pressure from DLHR sensor

void setup() {
    Wire.begin();
    Serial.begin(9600);
}

void loop() {
  readDLHR();
  Serial.print("Pressure: ");
  Serial.print(pressure1);
  Serial.print(", Temperature: ");
  Serial.println(temperature1);
  delay(1000);
}

void readDLHR()
{  
uint32_t P3, T3;
uint16_t P2, T2;
uint8_t X0, P1, T1;
Wire.beginTransmission(DLHRADD); // comenzar comunicaciones
/**
 * Wire.write(0xAD);// comando de lectura de promedio de 4 datos
 * Wire.write(0xAE);// comando de lectura de promedio de 8 datos
*/
Wire.write(byte(0xAF));// comando de lectura de promedio de 16 datos
Wire.endTransmission();
delay(62);
Wire.requestFrom(DLHRADD, 7);

if (Wire.available()<=7){
  X0 = Wire.read();
  P3 = Wire.read();
  P2 = Wire.read();
  P1 = Wire.read();
  T3 = Wire.read();
  T2 = Wire.read();
  T1 = Wire.read();
}
Wire.endTransmission();
float m1 = (P3<<16)+(P2<<8)+P1;
float m2 = (T3<<16)+(T2<<8)+T1;
/** Datasheet page 3/16
 *  Link: https://www.allsensors.com/cad/DS-0350_Rev_B.PDF
 *  refered date 03-05-2019
 */
pressure1 = 1.25*((m1-0.5*pow(2,24))/(pow(2,24))  )*10*2.;
temperature1 = (m2*125/pow(2,24)) - 40;
}
