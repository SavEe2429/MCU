#include <ESP8266WiFi.h>

const char *ssid = "Prawet";
const char *password = "abcdefabcd";

WiFiServer server(80);

const int trigPin = D1;
const int echoPin = D2;
const int ledPin = D3;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

long readDistanceCM()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
    return;

  while (!client.available())
    delay(1);
  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/ON") != -1)
  {
    digitalWrite(ledPin, HIGH);
  }
  else if (request.indexOf("/OFF") != -1)
  {
    digitalWrite(ledPin, LOW);
  }

  if (request.indexOf("/distance") != -1)
  {
    long distance = readDistanceCM();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    client.print(distance);

    return;
  }

  // หน้าเว็บหลัก
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>ESP8266 Control</title>
      <style type="text/css">
            html { 
                font-family: monospace;
                margin: 0px auto;
                text-align: center;
                background-color: #f0f0f0;
            }
            content {
                display: flex;
                flex-wrap: wrap;         /* ✅ ให้ wrap ได้เมื่อจอแคบ */
                justify-content: center;
                gap: 20px;
                padding: 20px;
                box-sizing: border-box;
                max-width: 100%;
            }
            #left, #right {
                border: 1px solid #aaa;
                padding: 16px;
                width: 100%;             /* ✅ ใช้ 100% บนจอเล็ก */
                max-width: 300px;        /* ✅ จำกัดความกว้างไม่ให้ใหญ่เกินไป */
                background-color: #fff;
                box-sizing: border-box;
            }
            #right{
                display: flex;
                flex-direction: column;
                justify-content: center;   
                align-items: center;       
                text-align: center;  
            }
            #message {
                display: flex;
                width: 100%;
                justify-content: center;
            }
            .alert-box{
                color: black;
                padding: 10px;
                /* display: flex; */
                /* justify-content: space-around; */
                font-weight: bold;
                font-size: 2em;
                border-radius: 6px;
                margin: 10px auto;
                width: 100%; 
                max-width: 250px; 
                transition: opacity 0.3s ease;
            }
            #warning{
                background-color: #ffffcc;
                border-left: 5px solid #ff9800;
            }
            #danger{
                background-color: #ffcccc;
                border-left: 5px solid #ff0000;
            }
            #safe{
                background-color: #ccffcc;
                border-left: 5px solid #2e7d32; ;
            }
            .center-wrapper {
                flex: 1; 
                display: flex;
                align-items: center;  
                justify-content: center;
            }
            .value-wrapper {
                font-size: 4em;
                font-weight: bold;
            }

            .switch {
                position: relative;
                display: inline-block;
                width: 80px;
                height: 34px;
                margin-top: 5%;
            }
            
            /* Hide the default checkbox */
            .switch input {
                opacity: 0;
                width: 0;
                height: 0;
            }
            /* Slider */
            .slider {
                position: absolute;
                cursor: pointer;
                top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background-color: red;
                transition: 0.4s;
                border-radius: 34px;
            }
            /* Slider Circle */
            .slider:before {
                position: absolute;
                content: "";
                height: 26px;
                width: 26px;
                left: 4px;
                bottom: 4px;
                background-color: white;
                transition: 0.4s;
                border-radius: 50%;
            }
            /* Checked State */
            input:checked + .slider {
                background-color: #4caf50;
            }

            input:checked + .slider:before {
                transform: translateX(46px);
            }
        </style>
      <script>
        function updateDistance() {
        fetch('/distance')
          .then(response => response.text())
            .then(data => {
                const distance_Float = parseFloat(data);
                document.getElementById('distance').innerText = data + ' cm';

                const warnbox = document.getElementById('warning');
                const safebox = document.getElementById('safe');
                const dangerbox = document.getElementById('danger');
                dangerbox.style.display = 'none';
                warnbox.style.display = 'none';
                safebox.style.display = 'none';
                if (distance_Float < 10) {
                    dangerbox.style.display = 'block';
                } else if (distance_Float < 30) {
                    warnbox.style.display = 'block';
                } else {
                    safebox.style.display = 'block';
                }
            })
            .catch(error => {
                console.error("Fetch failed:", error);
            });
        }
        setInterval(updateDistance, 1000); // อัปเดตทุก 1 วินาที
        window.onload = function () {
        updateDistance();

        const toggle = document.getElementById("toggle-switch");
        toggle.addEventListener("change", () => {
          fetch(toggle.checked ? "/ON" : "/OFF")
            .catch(err => console.error("LED toggle failed:", err));
        });
      };
      </script>
    </head>
    <body id="body">
        <header>
          <h1>ESP8266 Control</h1>
        </header>

        <content>
            <div id="left">
                <h3 >Control LED State</h3>
			    <hr>
                <label class="switch">
                    <input type="checkbox" id="toggle-switch">
                    <span class="slider"></span>
                </label>
            </div>

            <div id="right">
                <h3>Ultrasonic Sensor Data</h3>
                <hr style="width: 250px;">
                <div class="center-wrapper">
                    <div class="value-wrapper">
                        <!-- <span id="distance">--</span> -->
                        <span id="distance">--</span>
                    </div>
                </div>
            </div>
            
        </content>
        <div id="message">
            <div class="alert-box" id="warning">⚠️ Warning ⚠️</div>
            <div class="alert-box" id="danger">❗️ DANGER!!! ❗️</div>
            <div class="alert-box" id="safe">✔️ SAFE ✔️</div>
        </div>
        <!-- <p><a href="/ON"><button>เปิดไฟ</button></a></p>
        <p><a href="/OFF"><button>ปิดไฟ</button></a></p>
        <h2>ระยะจาก HC-SR04: <span id="distance">--</span></h2> -->
        
    </body>
    </html>
  )rawliteral";

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.print(html);
  delay(1);
}
