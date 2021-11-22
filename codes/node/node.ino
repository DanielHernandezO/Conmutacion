#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

char *ssid  = "IoT-B19";
char *password = "lcontrol2020*";

//char *ssid  = "SOSA PENA";
//char *password = "1BX27e8JDU1";

char *host = "https://venom-293e9-default-rtdb.firebaseio.com/";
char *auth = "QfdBEvwB5uBhBx75W4XqAhscLSEunhC3GxzwXvGU";
String p,l,v,d,c;
FirebaseData firebasedata;
DHT dht(D5,DHT11);
int t;
void setup() {
  pinMode(A0,INPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println();
  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("\nConectado al Wi-Fi:");
  Serial.println(WiFi.localIP());
  Firebase.begin(host,auth);
  delay(2000);
  p = "0";
  l = "0";
  v = "0";
  d = "0";
  c = "0000";
  t = millis();
}
//abrir puerta (voz) -> p1 -> p0
//caja fuerte(teclado matricial)
//luces -> lr -> lb -> lg -> l0 
//humo
//temperatura y humedad d
//ventilador -> v0 -> v1
void fire(){
  int aux;
  aux = millis();
  if (aux-t >= 5000){
    t = aux;
    Firebase.setDouble(firebasedata,"commands/D/Humedad",dht.readHumidity());
    Firebase.setDouble(firebasedata,"commands/D/Temperatura",dht.readTemperature());
  }
    
}
void loop() {
  fire();
  Firebase.getString(firebasedata,"commands/P/Estado");
  if (firebasedata.stringData()!= p){
    Serial.print("p"+firebasedata.stringData());
    p = firebasedata.stringData();
  }
  Firebase.getString(firebasedata,"commands/l/Estado");
  if (firebasedata.stringData()!= l){
    Serial.print("l"+firebasedata.stringData());
    l = firebasedata.stringData();
  }
  Firebase.getString(firebasedata,"commands/v/Estado");
  if (firebasedata.stringData()!= v){
    Serial.print("v"+firebasedata.stringData());
    v = firebasedata.stringData();
  }
  
  Firebase.getString(firebasedata,"commands/Contrasenia");
  if (firebasedata.stringData()!= c){
    Serial.print("c"+firebasedata.stringData());
    c = firebasedata.stringData();
  }

  delay(1000);
  //Serial.print("v0");
}
