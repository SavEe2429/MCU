int led1 = 5;  // ไฟดวงที่ 1
int led2 = 6;  // ไฟดวงที่ 2
int led3 = 7;  // ไฟดวงที่ 3

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  // ไฟดวงที่ 1 ติด 
  digitalWrite(led1, HIGH);
  delay(700);   

  // ไฟดวงที่ 2 ติด 
  digitalWrite(led2, HIGH);
  delay(700);

  // ไฟดวงที่ 3 ติด 
  digitalWrite(led3, HIGH);
  delay(700);

  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(1200);  // เว้นรอบให้ชัดเจนก่อนเริ่มรอบใหม่
}
