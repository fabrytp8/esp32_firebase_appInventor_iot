#include <WiFi.h>
#include <FirebaseESP32.h>
 
FirebaseConfig config;
FirebaseAuth auth;
FirebaseData firebaseData;
// URL y clave de Firebase
#define FIREBASE_HOST "https://sistfoto-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vi1UxWnBMdoGX5J3qccEgqMr9zOOtJoZzmW3OrFG"

// Credenciales de WiFi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
int cnt=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(33,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  pinMode(34,INPUT);
   Serial.begin(115200);
   Serial.print("Conectando a WiFi...");
    // Conectar a WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando a WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConectado a WiFi");
 
    // Configurar Firebase
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
   
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    Serial.println("Conectado a Firebase");


}

void loop() {
  int w=analogRead(34);
  int p1=digitalRead(33);
  int p2=digitalRead(35);
  // put your main code here, to run repeatedly:
    cnt++;
    if(p1==0){
       Firebase.setFloat(firebaseData, "cnt", cnt);
       Serial.println(cnt);       
    }
    if(p2==0){
       Firebase.setFloat(firebaseData, "w", w);
       Serial.println(w);       
    }
//    int x= Firebase.getInt(firebaseData,"/CASA/DORMITORIO/FOCO1");
//    Serial.println(x);

}
