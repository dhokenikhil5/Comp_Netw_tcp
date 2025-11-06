Built an IoT network communication project integrating NodeMCU (ESP8266) with a Python TCP server to demonstrate low-level socket programming and real-time sensor data transfer. The NodeMCU reads temperature and humidity from a DHT11 sensor and transmits it via TCP/IP over Wi-Fi to a custom backend server running on my laptop. This project highlights my ability to design, debug, and analyze client–server architectures, manage network reliability, and apply theoretical concepts from Computer Networking (Kurose & Ross) in a practical IoT system.


Start the server (bind + listen).
Wait for a client to connect.
When a client connects:
Continuously receive data.
Decode and parse each message.
Print formatted output.
When client disconnects → wait for a new one.

------------- [ detail working of algorithm ]---------------------------------------------------------------------------
Let’s dive deeper into what’s actually happening under the hood when this line of code runs on your ESP board:
client.println(data);
…and your Python TCP server receives it with:
data = conn.recv(1024)
We’ll go step-by-step through the network internals, how packets move, and what’s happening inside the hardware and software layers.
-----------------------------------------------------------------------------------------------------------------------------
🌐 1️⃣ What is actually happening conceptually
When you call client.connect(server_ip, server_port) on your ESP (ESP8266 or ESP32):
WiFi stack (inside ESP’s firmware) uses your WiFi credentials to join your local network.
It gets an IP address (via DHCP from your router).
When you connect to your Python server (e.g., 192.168.1.10:5050), the ESP:
Creates a TCP socket locally.
Performs a TCP 3-way handshake with the server:
SYN → request connection
SYN-ACK → server acknowledges
ACK → connection established
Once this handshake completes, the two devices have a virtual communication channel — a TCP connection.
-----------------------------------------------------------------------------------------------------------------------------
⚙️ 2️⃣ When you send data: client.println(data)
Let’s assume your data looks like:
"12,1234567,28.3,65.2\n"
Here’s what happens internally:
client.println() converts the text to bytes (ASCII).
31 32 2C 31 32 33 34 35 36 37 2C 32 38 2E 33 2C 36 35 2E 32 0A
(Those are the hex values of each character.)
The ESP’s WiFiClient library hands these bytes to the TCP stack in the firmware.
The TCP stack:
Breaks your message into packets (if it’s large; here it’s small enough for one packet).
Adds a TCP header (source port, destination port, sequence numbers, checksum, etc.).
Wraps that inside an IP packet (with source IP and destination IP).
Wraps that inside a WiFi frame (with MAC addresses and CRC).
The WiFi hardware sends this frame over the air to your WiFi router.
The router recognizes that the destination IP (e.g., 192.168.1.100) is on your local network and forwards it via Ethernet/Wi-Fi to your Python server machine.
-----------------------------------------------------------------------------------------------------------------------------
🧾 3️⃣ On the Python side
Your server, written with socket, is already listening on PORT = 5050.
When the TCP handshake completes, it has an open connection:
conn, addr = s.accept()
When a packet arrives:
The OS network stack reassembles the TCP data stream (it may have arrived in multiple small packets).
conn.recv(1024) reads whatever bytes have arrived in the buffer — in this case, your "12,1234567,28.3,65.2\n".
Python gives that to your code as a bytes object.
You decode() it back to a string, then parse and print it.
So you can think of TCP as giving you a reliable byte stream — it guarantees:
Data arrives in order.
Data arrives without loss (or it retries automatically).
You don’t see individual packets, only a continuous stream of bytes.
-----------------------------------------------------------------------------------------------------------------------------
🧱 4️⃣ Under the Hood: TCP Guarantees
Feature	How it Works
Reliability	Every packet has a sequence number and is acknowledged by the receiver (ACK). If an ACK doesn’t arrive, the sender retransmits.
Flow Control	Receiver advertises how much buffer space it has (window size).
Congestion Control	TCP dynamically adjusts speed based on network conditions.
Ordering	The OS reorders out-of-sequence packets before delivering to your program.
That’s why you can just call client.println() and not worry about packet size or order — TCP does all that automatically.
-----------------------------------------------------------------------------------------------------------------------------
⚡ 5️⃣ So one cycle looks like this
Step	Sender (ESP)	Network	Receiver (Python Server)
1	client.println(data)	↓	
2	Convert to bytes	↓	
3	Packetized into TCP segments	↓	
4	Sent via WiFi to router	↓	
5	Routed to server’s IP	↓	
6	OS reassembles stream	↓	
7	conn.recv(1024)	Receives the bytes	
8	Python decodes → "12,1234567,28.3,65.2"	✅	
-----------------------------------------------------------------------------------------------------------------------------
🧠 6️⃣ Quick Analogy
Think of TCP as a guaranteed postal service:
Each message is put into numbered envelopes (packets).
The receiver confirms (“ACKs”) each envelope.
If one gets lost, the sender resends it.
Finally, the receiver delivers the reassembled message (all envelopes in order) to your Python app.
-----------------------------------------------------------------------------------------------------------------------------
📈 7️⃣ Optional Experiment
You can see the actual packets yourself using a tool like Wireshark on your computer:
Start capturing on your Wi-Fi/Ethernet interface.
Filter: tcp.port == 5050
You’ll see packets from your ESP’s IP to your PC’s IP.
You can even open each packet and see your "12,1234567,28.3,65.2" data inside the TCP payload.
