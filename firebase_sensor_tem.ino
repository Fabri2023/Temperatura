#include <DallasTemperature.h>
#include <OneWire.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Configuración de la red Wi-Fi
#define WIFI_SSID "MARIA PAZ"
#define WIFI_PASSWORD "Gema12345"

// Configuración de Firebase
#define FIREBASE_HOST "https://proyecto-firebase-e1817-default-rtdb.firebaseio.com/"  // Reemplaza con tu URL de Firebase
#define FIREBASE_AUTH "hWgSiKhdc0SG6SlKxPmwhP4ygSEGMFolVLAR0lb7"                      // Reemplaza con tu Firebase Auth (Legacy token)
#define API_KEY "AIzaSyCFmB1aB7-eVano_9qZFIKCPtai1Uh9WGk"                             // Reemplaza con tu API Key

#define ONE_WIRE_BUS 23  // pin sensor
#define LED1_PIN 19      // sube temp
#define LED2_PIN 12      // temp NORMAL
#define LED3_PIN 18      // baja normal

// Inicializa las instancias de Firebase y Wi-Fi
//FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
FirebaseData firebaseData;
//FirebaseAuth auth;
//FirebaseConfig config;


//#define TRIG 2
//#define ECHO 4

//int duracion;
//int distancia;
//bool sensorEncendido = true;

void setup() {

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  Serial.begin(115200);

  // Conexión a la red Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado");

  // Configuración de Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.api_key = API_KEY;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    Serial.println("Conectado a Firebase");
  } else {
    Serial.println("Error al conectar a Firebase");
  }
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  // Apagar todos los LEDs inicialmente

  //digitalWrite(LED1_PIN, LOW);
  //digitalWrite(LED2_PIN, LOW);
  //digitalWrite(LED3_PIN, LOW);

  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperatura: ");
    Serial.println(tempC);

    if (Firebase.setFloat(firebaseData, "/temperatura", tempC)) {
      Serial.print("Temperatura enviada a Firebase: " + String(tempC));
      
      if (tempC > 30) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
      } else if (tempC >= 20 && tempC <= 30) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW);
      } else if (tempC < 20) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, HIGH);
      }

    
      /*if (tempC > 30.0) {
        digitalWrite(LED1_PIN, HIGH);  // Encender LED1
      } else {
        digitalWrite(LED1_PIN, LOW);  // Apagar LED1
      }

      if (tempC > 20.0) {
        digitalWrite(LED2_PIN, HIGH);  // Encender LED2
      } else {
        digitalWrite(LED2_PIN, LOW);  // Apagar LED2
      }

      if (tempC < 20.0) {
        digitalWrite(LED3_PIN, HIGH);  // Encender LED3
      } else {
        digitalWrite(LED3_PIN, LOW);  // Apagar LED3
      }*/


    } else {
      Serial.println("Error al enviar temperatura a Firebase");
      //Firebase.setFloat(firebaseData, "/temperatura", 0);
      Serial.println(firebaseData.errorReason());
    }
  } else {
    Serial.println("Error al leer la temperatura del sensor DS18B20");
    Firebase.setFloat(firebaseData, "/temperatura", 0);
  }

  delay(1000);  // Espera entre lecturas  
}