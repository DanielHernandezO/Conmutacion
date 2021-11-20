#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


#define red A0
#define blue A1
#define green A2
#define INB 23
#define INA 22
#define DHT11pin A3
#define nodeCom A4
const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

byte rowPins[numRows] = {25,27,29,31};
byte colsPins[numRows] = {33,35,37,39};

Keypad teclado = Keypad(makeKeymap(keymap),rowPins,colsPins, numRows, numCols);
LiquidCrystal_I2C lcd(0x27,16,2); //(0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2)

Servo servoMotor;
DHT dht(DHT11pin,DHT11);
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
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("> Conmutacion <");
  delay(1000);
  lcd.clear();
  lcd.noBacklight();
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
      if (aux=='1'){
        digitalWrite(INA,0);
        digitalWrite(INB,1);
      }else{
        digitalWrite(INA,0);
        digitalWrite(INB,0);
      }
    }else if(aux == 'd'){
      Serial1.print("0000");
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
    Serial.println("c: "+contrasenia);
    if (tecla=='D' && pass == contrasenia){
      servoMotor.write(90);
      delay(2000);
      servoMotor.write(0);
      delay(1000);
    }else{
      Serial.println("Canceled");
    }
  }
}
