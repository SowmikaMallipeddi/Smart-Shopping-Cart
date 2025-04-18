#define ENA 9
#define ENB 10
#define IN1 8
#define IN2 7
#define IN3 6
#define IN4 5
#define TRIG 3
#define ECHO 2

int esp_pin = 13;

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(esp_pin, INPUT);
    
    Serial.begin(9600);
}

long getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    long duration = pulseIn(ECHO, HIGH);
    long distance = duration * 0.034 / 2; // Convert to cm
    return distance;
}

void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}


void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void loop() {
  if(digitalRead(esp_pin) == HIGH)
  {
    long distance = getDistance();
    Serial.print("Distance: ");
    Serial.println(distance);
    
    if (distance > 50) {
        moveForward();
        delay(2000);
        stopMotors();
    } else {
        stopMotors();
        delay(1000);
        }
}
else if(digitalRead(esp_pin) == LOW)
{
  stopMotors();
  delay(1000);
}
}
