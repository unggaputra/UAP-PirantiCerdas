#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>


char auth[] = "Auth Token Blynk";  // isikan auth token blynk
char ssid[] = "SSID Wifi";   // isikan nama wifi
char pass[] = "Password Wifi";   // isikan password wifi

#define RELAY_PIN 19      // pin modul relay dihubungkan ke pin 25
#define DHT_SENSOR_PIN  21 // ESP32 pin GIOP21 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22


int Suhuatas = 0;       // variabel untuk menyimpan nilai upper limit
int Suhubawah = 0;      // variabel untuk menyimpan nilai lower limit
int Temperature = 0;    // variabel untuk menyimpan nilai sensor temperatur
double Humidity = 0;    // variabel untuk menyimpan nilai sensor kelembaban udara

BlynkTimer timer;
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht_sensor.begin(); // initialize the DHT sensor
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  timer.setInterval(3000L, checkTemp);
}


void loop()
{
  Blynk.run();
  timer.run();
}

void checkTemp()
{ 
  Temperature = dht_sensor.readTemperature();
  Blynk.virtualWrite(V0, Temperature);
  Humidity = dht_sensor.readHumidity();
  Blynk.virtualWrite(V1, Humidity);

  if (Temperature < Suhubawah) {
    digitalWrite(RELAY_PIN, HIGH);   // jika kondisi suhu di Bawah lower limit, penghangat menyala
  }
  else if (Temperature >= Suhuatas){
    digitalWrite(RELAY_PIN, LOW);  // jika kondisi suhu di atas atau sama dengan upper limit, penghangat menyala
  }
}

BLYNK_CONNECTED() {
   Blynk.syncAll();
}

BLYNK_WRITE(V2)
{
    Suhubawah = param.asInt();
}

BLYNK_WRITE(V3)
{
    Suhuatas = param.asInt();
}
