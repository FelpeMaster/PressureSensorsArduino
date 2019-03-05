#include <Wire.h>
#define ASDXADD 120 //5 inchs H2O
#define DLHRADD 41 //10 inchs H2O
float pressure2,temperature1, pressure1; // pressure from ASDX sensor 005ND7A5



void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  readASDX();
  readDLHR();
  Serial.print("Temperatura 1: ");
  Serial.print(temperature1);
  Serial.print(", Presion 1: ");
  Serial.print(pressure1);
  Serial.print(", Presion 2: ");
  Serial.println(pressure2);
  delay(5000);
}


void readASDX(){
  uint32_t P1;
  uint16_t P2;
    
  Wire.beginTransmission(ASDXADD); // comenzar comunicaciones
  Wire.requestFrom(ASDXADD, 2);
  
  if (Wire.available()<=3){
    
    P2 = Wire.read();
    P1 = Wire.read();
  }

  float Pmin = -5.;
  float Pmax = 5.;
  Wire.endTransmission();
  float O_p_2_14 = ((P2<<8) + P1)/(16384.); // output % of 2^14
  pressure2 = ((O_p_2_14 - 0.05)/(.9))*(Pmax - Pmin) + Pmin;
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
