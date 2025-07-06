from flask import Flask, send_from_directory
import random

app = Flask(__name__)

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route("/distance")
def get_distance():
    # ส่งค่าระยะจำลองระหว่าง 5-30 cm
    return str(random.randint(1, 45))

@app.route("/led/on")
def led_on():
    return "LED turned ON"

@app.route("/led/off")
def led_off():
    return "LED turned OFF"

if __name__ == "__main__":
    app.run(debug=True)