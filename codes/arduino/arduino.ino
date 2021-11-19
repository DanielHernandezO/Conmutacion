#include <Servo.h>
#include <DHT.h>


#define red A0
#define blue A1
#define green A2
#define INB 23
#define INA 22
#define DHT11pin A3

Servo servoMotor;
DHT dht(DHT11pin,DHT11);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  servoMotor.attach(9);
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(INB,OUTPUT);
  pinMode(INA,OUTPUT);
  dht.begin();
  
}
//servoDATA A0
void leds(int r,int b, int g){
  analogWrite(red,r);
  analogWrite(blue,b);
  analogWrite(green,g);
}

void loop() {
  char aux;
  if (Serial1.available()){
    aux = Serial1.read();
    Serial.println(aux); // imprime codigo

    //puerta
    if(aux=='p'){
      while (!Serial1.available());
      aux = Serial1.read();
      if (aux=='1'){
        servoMotor.write(180);
        delay(1000);
      }else{
        servoMotor.write(0);
        delay(1000);
      }
    }else if(aux=='l'){
      while (!Serial1.available());
      aux = Serial1.read();
      if (aux=='0'){
        leds(0,0,0);
      }else if (aux=='r'){
        leds(255,0,0);
      }else if (aux=='b'){
        leds(0,255,0);
      }else if (aux == 'g'){
        leds(0,0,255);
      }
    }else if(aux=='v'){
      while (!Serial1.available());
      aux = Serial1.read();
      if (aux=='1'){
        digitalWrite(INA,0);
        digitalWrite(INB,1);
      }else{
        digitalWrite(INA,0);
        digitalWrite(INB,0);
      }
    }else if(aux == 'd'){
      Serial1.println(dht.readHumidity());
      
    }
    aux = "";
  }
  
}
