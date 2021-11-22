#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


#define red A0
#define blue A1
#define green A2
#define INB 23
#define INA 22
#define nodeCom A4
#define buzzer 2
#define smoke A3
int sensorThreshold = 400;
const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

byte rowPins[numRows] = {33,35,37,39};
byte colsPins[numRows] = {25,27,29,31};

Keypad teclado = Keypad(makeKeymap(keymap),rowPins,colsPins, numRows, numCols);
LiquidCrystal_I2C lcd(0x27,16,2); //(0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2)
Servo servoMotor;
char tecla;
String contrasenia="";
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  servoMotor.attach(9);
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(INB,OUTPUT);
  pinMode(INA,OUTPUT);
  pinMode(nodeCom,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(smoke, INPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("> Conmutacion <");
  delay(1000);
  lcd.clear();
  lcd.noBacklight();
  digitalWrite(red,0);
  digitalWrite(blue,0);
  digitalWrite(green,0);
  digitalWrite(INB,0);
  digitalWrite(INA,0);
  servoMotor.write(0);
}
//servoDATA A0
void leds(int r,int b, int g){
  analogWrite(red,r);
  analogWrite(blue,b);
  analogWrite(green,g);
}

void loop() {
  
  char aux;
  //Serial.println(analogRead(smoke));
  if (analogRead(smoke) > sensorThreshold){
    tone(buzzer, 1000, 200);
  }
  if (Serial1.available()){
    aux = Serial1.read();
    //Serial.println(aux);
    //puerta
    if(aux=='p'){
      while (!Serial1.available());
      aux = Serial1.read();
      if (aux=='1'){
        servoMotor.write(90);
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
      }else if (aux=='1'){ //red
        leds(255,0,0);
      }else if (aux=='2'){ //blue
        leds(0,255,0);
      }else if (aux == '3'){ //green
        leds(0,0,255);
      }
    }else if(aux=='v'){
      while (!Serial1.available());
      aux = Serial1.read();
      Serial.println(aux);
      if (aux=='1'){
        digitalWrite(INA,0);
        digitalWrite(INB,1);
      }else{
        digitalWrite(INA,0);
        digitalWrite(INB,0);
      }
    }else if(aux == 'd'){
      //falta
    }else if(aux == 'c'){
      contrasenia="";
      for (int i = 0; i < 4; i++){
        while (!Serial1.available());
        aux = Serial1.read();
        contrasenia += aux;
      }
    }
    aux = "";
  }
  tecla = teclado.getKey();
  if (tecla){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("==> Password <==");
    int i = 0;
    String pass = "";
    while(true){
      while(!(tecla = teclado.getKey()));
      if (tecla=='D' || tecla=='C') break;
      lcd.setCursor(i,1);
      lcd.print("*");
      i++;
      pass += tecla;
    }
    lcd.clear();
    lcd.noBacklight();
    if (tecla=='D'){
      //Serial.println("c: "+pass);
      if (pass == contrasenia){
        servoMotor.write(90);
        delay(2000);
        servoMotor.write(0);
        delay(1000);
      }else{
        Serial.println("Error");
        tone(buzzer, 2000,3000);
      }
    }else{
      Serial.println("Canceled");
    }
  }
}
