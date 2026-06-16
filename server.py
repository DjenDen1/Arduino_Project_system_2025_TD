import threading
import time
from flask import Flask, render_template, jsonify, request
import serial


app = Flask(__name__, template_folder='.')

BLUETOOTH_PORT = '/dev/rfcomm7'  
BAUD_RATE = 9600

latest_data = {
    "lmtemp": "0.0",
    "humidity": "0.0",
    "pressure": "0.0"
}

data_buffer = ""
def parse_arduino_data(raw_text):
    global latest_data, data_buffer
    try:
        data_buffer += raw_text
        
        if "\n" in data_buffer:
            lines = data_buffer.split("\n")
            
            data_buffer = lines[-1]
            
            complete_line = lines[-2].replace('\r', '').strip()
            
            print(f"[БЛЮТУЗ РЕГИСТРАТОР] Получена полная строка: '{complete_line}'")
            
            if ";" in complete_line:
                parts = complete_line.split(';')
                
                if len(parts) >= 1 and parts[0].strip():
                    latest_data["lmtemp"] = parts[0].strip()
                if len(parts) >= 2 and parts[1].strip():
                    latest_data["humidity"] = parts[1].strip()
                if len(parts) >= 3 and parts[2].strip():
                    latest_data["pressure"] = parts[2].strip()
                    
                print(f"[УСПЕХ] Данные в словаре обновлены: {latest_data}")
            else:
                print(f"[ПРЕДУПРЕЖДЕНИЕ] Строка без разделителей: '{complete_line}'")
                
    except Exception as e:
        print(f"[ОШИБКА ПАРСИНГА]: {e}")

def bluetooth_worker():
    print(f"[ПОТОК] Фоновый поток Bluetooth запущен. Слушаем порт {BLUETOOTH_PORT}...")
    
    while True:
        try:
            with serial.Serial(BLUETOOTH_PORT, BAUD_RATE, timeout=2) as ser:
                print(f"[БЛЮТУЗ] Успешно подключились к порту {BLUETOOTH_PORT}!")
                
                while True:
                    ser.write(b'?')
                    time.sleep(0.4)  
                    
                    
                    if ser.in_waiting > 0:
                        raw_data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
                        
                        with open("bluetooth.log", "a", encoding="utf-8") as log_file:
                            log_file.write(f"--- {time.strftime('%Y-%m-%d %H:%M:%S')} ---\n")
                            log_file.write(raw_data + "\n")
                        
                        parse_arduino_data(raw_data)
                        
                    time.sleep(1)
                    
        except (serial.SerialException, OSError):
            print(f"[ИНФО] Порт {BLUETOOTH_PORT} недоступен. Проверьте питание HC-05. Повтор через 5 сек...")
            time.sleep(5)
        except Exception as e:
            print(f"[КРИТИЧЕСКАЯ ОШИБКА В ПОТОКЕ]: {e}")
            time.sleep(5)


@app.route('/')
def index():
    return render_template('client.html')

@app.route('/get_data', methods=['GET', 'POST'])
def get_data():
    global latest_data
    return jsonify(latest_data)

if __name__ == '__main__':
    bt_thread = threading.Thread(target=bluetooth_worker, daemon=True)
    bt_thread.start()
    
    app.run(host='0.0.0.0', port=5000, debug=False)
