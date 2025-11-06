
import socket     # built-in Python module that allows network communication 
                  # (sending and receiving data over TCP or UDP).
from datetime import datetime   
                  # Used to get the current date and time (for logging when data arrives).

HOST = '0.0.0.0'  # means the server listens on all available network interfaces 
                  # (so any device on the same network can connect to it)
PORT = 5050       # start listening on PORT number 5050

#--------------------------------------------------------------------------------------------------#
# Function to parse incoming data

def parse_data(data):  # Takes the raw text received from the client (like "12,1699182300,28.4,65.1").
    # Expect format: seq_num,timestamp,temperature,humidity
    parts = data.strip().split(",")
    # strip() removes any extra spaces or newline characters.
    # split(",") breaks the string into a list of 4 parts based on commas

    if len(parts) == 4:       
        seq_num, timestamp, temp, hum = parts
        return int(seq_num), int(timestamp), float(temp), float(hum)   # convert the raw data to integer and float
    return None
#--------------------------------------------------------------------------------------------------#
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:  # Creates a TCP socket (AF_INET = IPv4, SOCK_STREAM = TCP).
                                                              # Using with ensures the socket is automatically closed when the block ends.
 
    s.bind((HOST, PORT))                  # bind() attaches the socket to the specified host and port (so the OS knows which network interface and port number to listen on).         
    s.listen()                            # listen() tells the socket to start waiting for incoming connections.
    print(f"Server listening on port {PORT}")

    while True:
        conn, addr = s.accept()          # s.accept() waits until a client connects.
        with conn:                       # conn: a new socket object used for communication with that client.
                                         # with conn: ensures the client connection closes properly when done.
            print(f"Connected by {addr}") 
            while True:
                data = conn.recv(1024)  # conn.recv(1024) waits for up to 1024 bytes of data from the client.
                if not data:            # If no data is received (client disconnects), it breaks the inner loop.
                    break
                parsed = parse_data(data.decode()) # data.decode() converts the raw bytes into a readable string (e.g., '12,1699182300,28.4,65.1').
                                                   # Passes it to parse_data() to extract individual values.
                if parsed:                         # Gets the current local time (now).
                    seq_num, timestamp, temp, hum = parsed
                    now = datetime.now().strftime("%H:%M:%S")
                    print(f"[{now}] Seq:{seq_num} Time:{timestamp} Temp:{temp}°C Hum:{hum}%")
#--------------------------------------------------------------------------------------------------#
