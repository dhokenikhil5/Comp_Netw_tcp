#include <ESP8266WiFi.h>
#include <DHT.h>

// WiFi settings
const char* ssid = "Galaxy S20 FE 5G";
const char* password = "123456789";

// Server settings
const char* server_ip = "192.168.198.100"; // Laptop IP
const int server_port = 5050;

// DHT settings
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long seq_num = 0;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void loop() 
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  unsigned long timestamp = millis();
  String data = String(seq_num) + "," + String(timestamp) + "," + String(temperature, 1) + "," + String(humidity, 1);

  if (!client.connected()) 
  {
    if (!client.connect(server_ip, server_port)) 
    {
      Serial.println("Connection failed");
      delay(2000);
      return;
    }
  }

  client.println(data);
  Serial.println("Sent: " + data);
  seq_num++;
  delay(5000); // Send every 5 seconds
}
