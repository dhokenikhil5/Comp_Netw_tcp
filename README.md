Built an IoT network communication project integrating NodeMCU (ESP8266) with a Python TCP server to demonstrate low-level socket programming and real-time sensor data transfer. The NodeMCU reads temperature and humidity from a DHT11 sensor and transmits it via TCP/IP over Wi-Fi to a custom backend server running on my laptop. This project highlights my ability to design, debug, and analyze client–server architectures, manage network reliability, and apply theoretical concepts from Computer Networking (Kurose & Ross) in a practical IoT system.


Start the server (bind + listen).
Wait for a client to connect.
When a client connects:
Continuously receive data.
Decode and parse each message.
Print formatted output.
When client disconnects → wait for a new one.
