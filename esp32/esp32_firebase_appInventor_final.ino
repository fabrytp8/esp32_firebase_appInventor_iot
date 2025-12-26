#include <WiFi.h>                // Librería para manejar la conexión WiFi en la ESP32
#include <FirebaseESP32.h>       // Librería para conectarse con Firebase Realtime Database

FirebaseConfig config;           // Objeto de configuración para Firebase
FirebaseAuth auth;               // Objeto de autenticación (para token)
FirebaseData firebaseData;       // Objeto para enviar/recibir datos

// URL y clave de Firebase (Realtime Database)
// IMPORTANTE: Esta es la URL completa del proyecto y el token de acceso
#define FIREBASE_HOST "****************************************"
#define FIREBASE_AUTH "****************************************"

// Credenciales WiFi
#define WIFI_SSID "nombre de la red wifi"
#define WIFI_PASSWORD "clave de red wifi"

int cnt = 0;                     // Contador que se enviará a Firebase
unsigned long lastReadTime = 0;    // Tiempo de la última lectura de Firebase

void setup() {

  // Configurar pines
  pinMode(18, OUTPUT);           // Pin como salida digital
  pinMode(19, OUTPUT);           // Pin como salida digital
  pinMode(33, INPUT_PULLUP);     // Botón 1 con resistencia interna pull-up
  pinMode(35, INPUT_PULLUP);     // Botón 2 con resistencia interna pull-up
  pinMode(34, INPUT);            // Entrada analógica

  Serial.begin(115200);

  // --- Conexión a WiFi ---
  Serial.print("Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConectado a WiFi");

  // --- Configuración de Firebase ---
  config.host = FIREBASE_HOST;                       // Asignar URL del proyecto
  config.signer.tokens.legacy_token = FIREBASE_AUTH; // Token legado (modo antiguo)

  Firebase.begin(&config, &auth);      // Iniciar Firebase con la configuración
  Firebase.reconnectWiFi(true);        // Reconexión automática si se pierde WiFi

  Serial.println("Conectado a Firebase");
}

void loop() {

  // Leer señal analógica del pin 34
  int w = analogRead(34);

  // Leer botones
  int p1 = digitalRead(25);       // Botón 1 (activo en LOW)
  int p2 = digitalRead(35);       // Botón 2 (activo en LOW)

  cnt++;                          // Incrementa contador en cada ciclo

  // --- Enviar contador a Firebase cuando se presiona botón 1 ---
  if (p1 == 0) {
    Firebase.setFloat(firebaseData, "cnt", cnt);
    Serial.println(cnt);
    delay(200);
  }

  // --- Enviar valor analógico a Firebase cuando se presiona botón 2 ---
  if (p2 == 0) {
    Firebase.setFloat(firebaseData, "w", w);
    Serial.println(w);
    delay(200);
  }

  // --- Leer de Firebase cada 2 segundos ---
  if (millis() - lastReadTime >= 2000) {
    // Lectura ejemplo:
    Firebase.getString(firebaseData, "/led1");
    String x = firebaseData.stringData();
    Serial.print("x=");
    Serial.println(x);
    int x1=x.toInt();
    if(x1==0){
      digitalWrite(18,LOW); 
    }else{
      digitalWrite(18,HIGH); 
    }

    Firebase.getString(firebaseData, "/led2");
    String y = firebaseData.stringData();
    Serial.print("y=");
    Serial.println(y);
    int y1=y.toInt();
    if(y1==0){
      digitalWrite(19,LOW); 
    }else{
      digitalWrite(19,HIGH); 
    }

    lastReadTime = millis();
  }

  delay(10);
}
