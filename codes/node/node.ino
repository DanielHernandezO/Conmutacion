#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


char *ssid  = "SOSA PENA";
char *password = "1BX27e8JDU1";

char *host = "https://venom-293e9-default-rtdb.firebaseio.com/";
char *auth = "QfdBEvwB5uBhBx75W4XqAhscLSEunhC3GxzwXvGU";

FirebaseData firebasedata;
void setup() {
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
  Firebase.getString(firebasedata,"commands/comprar/accion");
  //Serial.println(firebasedata.stringData());
  Serial.print("d");
  while (!Serial1.available());
  Serial.print(Serial.read());
  delay(1000000);
}
