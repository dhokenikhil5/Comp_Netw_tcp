import socket
from datetime import datetime

HOST = '0.0.0.0'  # Listen on all interfaces
PORT = 5050

def parse_data(data):
    # Expect format: seq_num,timestamp,temperature,humidity
    parts = data.strip().split(",")
    if len(parts) == 4:
        seq_num, timestamp, temp, hum = parts
        return int(seq_num), int(timestamp), float(temp), float(hum)
    return None

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server listening on port {PORT}")

    while True:
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                parsed = parse_data(data.decode())
                if parsed:
                    seq_num, timestamp, temp, hum = parsed
                    now = datetime.now().strftime("%H:%M:%S")
                    print(f"[{now}] Seq:{seq_num} Time:{timestamp} Temp:{temp}°C Hum:{hum}%")
