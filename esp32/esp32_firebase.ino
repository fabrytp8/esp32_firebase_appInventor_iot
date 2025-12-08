#include <WiFi.h>                // Librería para manejar la conexión WiFi en la ESP32
#include <FirebaseESP32.h>       // Librería para conectarse con Firebase Realtime Database

FirebaseConfig config;           // Objeto de configuración para Firebase
FirebaseAuth auth;               // Objeto de autenticación (para token)
FirebaseData firebaseData;       // Objeto para enviar/recibir datos

// URL y clave de Firebase (Realtime Database)
// IMPORTANTE: Esta es la URL completa del proyecto y el token de acceso
#define FIREBASE_HOST "https://sistfoto-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vi1UxWnBMdoGX5J3qccEgqMr9zOOtJoZzmW3OrFG"

// Credenciales WiFi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

int cnt = 0;                     // Contador que se enviará a Firebase

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
  int p1 = digitalRead(33);       // Botón 1 (activo en LOW)
  int p2 = digitalRead(35);       // Botón 2 (activo en LOW)

  cnt++;                          // Incrementa contador en cada ciclo

  // --- Enviar contador a Firebase cuando se presiona botón 1 ---
  if (p1 == 0) {
    Firebase.setFloat(firebaseData, "cnt", cnt);
    Serial.println(cnt);
  }

  // --- Enviar valor analógico a Firebase cuando se presiona botón 2 ---
  if (p2 == 0) {
    Firebase.setFloat(firebaseData, "w", w);
    Serial.println(w);
  }

  // Lectura ejemplo (comentado):
  // int x = Firebase.getInt(firebaseData, "/CASA/DORMITORIO/FOCO1");
  // Serial.println(x);
}
