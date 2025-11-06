#include <ESP8266WiFi.h>                   // Node MCU and built -in WSP module library
#include <DHT.h>                           // Temerature and Humidity DHT-11 Sensor library 

// WiFi settings
const char* ssid = "Galaxy S20 FE 5G";     // Name of the WIFI hotspot              
const char* password = "123456789";        // Password

// Server settings
const char* server_ip = "192.168.198.100"; // Laptop IP
const int server_port = 5050;              // Port number , don't take the ports which are used by OS, it may not work 
                                           // Start the Server first before sending the data 
// DHT settings
#define DHTPIN D2                          // Sensor Input PIN
#define DHTTYPE DHT11                      // Sensor Type 
DHT dht(DHTPIN, DHTTYPE);

unsigned long seq_num = 0;                 // big variable to hold sequence number < it may go very large value , so take the largest available data type >

WiFiClient client;

/*-----------------------------------------------------------------------------------------------------------------------------------*/
void setup() 
{
  Serial.begin(115200);                   // the speed of communication
  dht.begin();                            // Initializes the DHT sensor (like DHT11 or DHT22).
  WiFi.begin(ssid, password);             // Starts connecting our board to a WiFi network as mentioned above with given credentials

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)   // Untill Not connected
  {
    delay(500);                           // delay half a second
    Serial.print(".");                    // print dot
  }
  Serial.println("Connected!");          // Print Message after connection is successful
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/

void loop() 
{
  float humidity = dht.readHumidity();        // Read the raw live values from Sensor object
  float temperature = dht.readTemperature();  // Read the raw live values from Sensor object

  if (isnan(humidity) || isnan(temperature)) // check if the values are within the rage , or a number 
  {
    Serial.println("Failed to read from DHT sensor!");  // Error message
    delay(2000);                                        // 2 second delay 
    return;
  }

  unsigned long timestamp = millis();       // millis() returns how many milliseconds have passed since the board started running.

  String data = String(seq_num) + "," + String(timestamp) + "," + String(temperature, 1) + "," + String(humidity, 1);
       // ^ Combines all sensor and timing info into one comma-separated line.
                                                                   // ^ formats the temperature with 1 decimal place (e.g., 28.3).Resulting string looks like:
  if (!client.connected())                // WiFiClient client object used for TCP communication.
  {
    if (!client.connect(server_ip, server_port)) // try again connecting 
    {
      Serial.println("Connection failed");       // print message to serial monitor 
      delay(2000);
      return;                                 // and exit
    }
  }
  // Sending the data

  client.println(data);               // sends the line (with a newline) to the connected server.
  Serial.println("Sent: " + data);    // prints to serial monitor after sending
  seq_num++;                          // increment the message counter
  delay(5000);                       // Send every 5 seconds
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/
