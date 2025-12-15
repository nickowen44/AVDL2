
try:
    import serial
except ModuleNotFoundError:
    print("pyserial not found. Try `pip install pyserial`")
    exit(1)

import math
import random
import time

SERIAL_PORT = 'COM3'
BAUD_RATE = 115200

def send_serial_data():
    print(f"Opening port {SERIAL_PORT} at {BAUD_RATE} baud...")
    
    try:
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=1  # Read timeout in seconds
        )

        if not ser.is_open:
            raise RuntimeError("Failed to open")
        print("Success! Opened.")
    
        def wave(t):
            return math.sin(t)

        t0 = time.time()
        LTNCY = 0.5
        while True:
            t = time.time() - t0

            msgs = {}

            msgs["ID"] = "A42"
            msgs["RES"] = int((t % 5) < 2.5)
            msgs["AMI"] = int(t % 6)
            msgs["EBS"] = int(t % 4)
            msgs["AST"] = int(t % 6)

            msgs["MMA"] = int(wave(t-LTNCY) * 50 + 50)
            msgs["MMT"] = int(wave(t) * 50 + 50)
            
            msgs["SA"] = int(wave(t-LTNCY) * 120)
            msgs["ST"] = int(wave(t) * 120)
            
            msgs["BRT"] = int(wave(t-LTNCY) * 50 + 50)
            msgs["BRA"] = int(wave(t) * 50 + 50)
            
            msgs["STT"] = int(wave(t-LTNCY) * 120)
            msgs["STA"] = int(wave(t) * 120)

            # Drop some keys randomly
            for _ in range(random.randint(0, 4)):
                known_keys = list(msgs.keys())
                chosen_key = known_keys[random.randint(0, len(known_keys)-1)]
                del msgs[chosen_key]

            # Shuffle them sometimes
            if random.random() < 0.2:
                new_keys = list(msgs.keys())
                random.shuffle(new_keys)
                msgs = {k: msgs[k] for k in new_keys}

            payload = ("|".join(f"{k}={v}" for k, v in msgs.items()) + "\r\n").encode()


            PERIOD = 1 / 20.0

            rng_final_choice = random.random()

            if rng_final_choice > 0.66:
                # send it 1 char at a time
                mini_sleep_time = PERIOD / len(payload)
                for i in range(len(payload)):
                    ser.write(payload[i:i+1])
                    time.sleep(mini_sleep_time)
            elif rng_final_choice > 0.33:
                # Split somewhere in the middle, send a large chunk
                i_split = int((random.random() * 0.5 + 0.5) * len(payload))

                ser.write(payload[:i_split])
                time.sleep(1 * (PERIOD / 4))
                ser.write(payload[i_split:])
                time.sleep(3 * (PERIOD / 4))
            else:
                ser.write(payload)
                time.sleep(PERIOD)

    except serial.SerialException as e:
        print(f"ERROR: Could not connect to {SERIAL_PORT}. Check if the port is in use or the name is correct.")
        print(f"Details: {e}")
        
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    send_serial_data()
