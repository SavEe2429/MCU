#include <ESP8266WiFi.h>

const char* ssid = "Norsor_2.4G";
const char* password = "0624098555";

WiFiServer server(80);

const int ledPin = D1;
const int trigPin = D5;
const int echoPin = D6;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/ON") != -1) {
    digitalWrite(ledPin, HIGH);
  } else if (request.indexOf("/OFF") != -1) {
    digitalWrite(ledPin, LOW);
  }

  if (request.indexOf("/distance") != -1) {
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
      <script>
        function updateDistance() {
          fetch('/distance')
            .then(response => response.text())
            .then(data => {
              document.getElementById('distance').innerText = data + ' cm';
            });
        }
        setInterval(updateDistance, 1000); // อัปเดตทุก 1 วินาที
        window.onload = updateDistance;
      </script>
    </head>
    <body>
      <h1>ควบคุมไฟจาก ESP8266</h1>
      <p><a href="/ON"><button>เปิดไฟ</button></a></p>
      <p><a href="/OFF"><button>ปิดไฟ</button></a></p>
      <h2>ระยะจาก HC-SR04: <span id="distance">--</span></h2>
    </body>
    </html>
  )rawliteral";

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.print(html);
  delay(1);

}
