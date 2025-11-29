const int red = D5;     // LED ดวงสีแดง
const int yellow = D6;  // LED ดวงสีเหลือง


void setup() {
  // กำหนดขา LED เป็น OUTPUT เพื่อให้สามารถจ่ายไฟได้
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);

}

void loop() {
  // ไฟดวงที่ 1 ติด 
  digitalWrite(red, HIGH);  // เปิด LED ดวงสีแดง
  delay(700);               // หน่วงเวลา 700 ms -> 0.7 วินาที

  // ไฟดวงที่ 2 ติด
  digitalWrite(yellow, HIGH); // เปิด LED ดวงสีเหลือง
  delay(700);                 // หน่วงเวลา 700 ms -> 0.7 วินาที
              // หน่วงเวลา 700 ms -> 0.7 วินาที

  // ดับไฟทั้งหมด 
  digitalWrite(red, LOW);     // ปิด LED ดวงสีแดง
  digitalWrite(yellow, LOW);  // ปิด LED ดวงสีเหลือง

  delay(1200);                // หน่วงเวลา 1200 ms -> 1.2 วินาที
}
