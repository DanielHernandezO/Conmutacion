#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


char *ssid  = "IoT-B19";
char *password = "lcontrol2020*";

char *host = "https://venom-293e9-default-rtdb.firebaseio.com/";
char *auth = "QfdBEvwB5uBhBx75W4XqAhscLSEunhC3GxzwXvGU";

FirebaseData firebasedata;
void setup() {
  pinMode(A0,INPUT);
  
  Serial.begin(115200);
  Serial.println();

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
}
//abrir puerta (voz) -> p1 -> p0
//caja fuerte(teclado matricial)
//luces -> lr -> lb -> lg -> l0 
//humo
//temperatura y humedad d
//ventilador -> v0 -> v1
void loop() {
  Firebase.getString(firebasedata,"commands/P/Estado");
  Serial.print("p"+firebasedata.stringData());
  Firebase.getString(firebasedata,"commands/l/Estado");
  Serial.print("l"+firebasedata.stringData());
  Firebase.getString(firebasedata,"commands/v/Estado");
  Serial.print("v"+firebasedata.stringData());
  
  //Firebase.getString(firebasedata,"commands/D/Estado");
  Serial.print("d");
  /*String contrasenia="";
  char aux;
  for (int i = 0; i < 4; i++){
    while (!Serial1.available());
    aux = Serial1.read();
    contrasenia += aux;
  }
  Firebase.setString(firebasedata,"commands/Contrasenia",contrasenia);
  */N
  Firebase.getString(firebasedata,"commands/Contrasenia");
  Serial.print("c"+firebasedata.stringData());
  
  delay(1000);
  //Serial.print("v0");
}
