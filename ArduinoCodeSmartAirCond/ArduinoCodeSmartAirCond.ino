#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID ""         // Tvoj Wi-Fi SSID
#define WIFI_PASSWORD ""  // Tvoja Wi-Fi šifra
#define API_KEY "AIzaSyAGMOJazMKOa2QQdaEcjcll62AdthMuhlo"
#define DATABASE_URL "https://smartesp8266-f9980-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int pinRed = D1;     // pin za LED crveni
int pinGreen = D2;   // pin za LED zeleni
int pinYellow = D3;  // pin za LED žuti
int alarm = D7;      // alarm - zvučni signal
int motor = D6;


int celsius = 0;
int ledStatus = 0;
int buzzerStatus = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  // Povezivanje na Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Firebase konfiguracija
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Serial.println("Connecting to Firebase...");
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase sign up successful");
    signupOK = true;
  } else {
    Serial.printf("Firebase sign up failed: %s\n", config.signer.signupError.message.c_str());
    while (true) {
      delay(1000);
    }
  }
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(A0, INPUT);    //analogni signal za TMP36
  pinMode(D6, OUTPUT);   // DC motor
  pinMode(D7, OUTPUT);   // buzzer - zujalica
  Serial.begin(115200);  //pozivanje Serial Monitora

  pinMode(pinGreen, OUTPUT);   // LED zelena
  pinMode(pinYellow, OUTPUT);  // LED plava
  pinMode(pinRed, OUTPUT);     // LED crvena
}
void loop() {
  if (!Firebase.ready() || !signupOK) return;
  int adcValue = analogRead(A0);              // Očitaj ADC vrijednost (0-1023)
  float voltage = adcValue * (3.3 / 1023.0);  // Preračunaj u napon (0-3.3V)
  // TMP36 daje 500mV pri 25°C, promjena napona je 20mV po °C
  // Temperatura (°C) = (Vout - 500mV) / 20mV
  float temperatureC = (voltage - 0.5) / 0.02;

  // ispisivanje vrijednosti temperature na Serial Monitoru
  Serial.print(temperatureC);
  Serial.print(" C, ");

  if (Firebase.RTDB.setFloat(&firebaseData, "/Temperature", temperatureC)) {
    Serial.println("Podaci uspješno poslani na Firebase");
  } else {
    Serial.println("Greška pri slanju podataka: " + firebaseData.errorReason());
  }
  if (temperatureC > 17 && temperatureC < 22) {  //ako je optimalna temperatura
    digitalWrite(pinGreen, HIGH);                //upali zelenu LE dioda
    digitalWrite(pinYellow, LOW);
    digitalWrite(pinRed, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
  }
  if (temperatureC > 21) {  //ako je temperatura veća od optimalne
    digitalWrite(pinGreen, LOW);
    digitalWrite(pinYellow, LOW);
    digitalWrite(pinRed, HIGH);  // upali crvenu LE diodu
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
  }
  if (temperatureC < 18) {  //ako je temperatura manja od 20
    digitalWrite(pinGreen, LOW);
    digitalWrite(pinYellow, HIGH);  //upali žutu LE diodu
    digitalWrite(pinRed, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, HIGH);  //uključuje ton za alarm
    delay(200);              // Beep for 500ms
    digitalWrite(D7, LOW);   // Turn off the buzzer
    delay(200);
  }
  delay(1000);
}