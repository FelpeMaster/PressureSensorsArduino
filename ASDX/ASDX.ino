#include <Wire.h>
#define ASDXADD 120 //5 inchs H2O
float pressure2; // pressure from ASDX sensor 005ND7A5

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  readASDX();
  Serial.print("Presion ASDX: ");
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

  Serial.print(P2);
  Serial.print(" ");
  Serial.println(P1);
  float Pmin = -5.;
  float Pmax = 5.;
  Wire.endTransmission();
  float O_p_2_14 = ((P2<<8) + P1)/(16384.); // output % of 2^14
  Serial.print("Numero ese: ");
  Serial.println(O_p_2_14);
  pressure2 = ((O_p_2_14 - 0.05)/(.9))*(Pmax - Pmin) + Pmin;
}
