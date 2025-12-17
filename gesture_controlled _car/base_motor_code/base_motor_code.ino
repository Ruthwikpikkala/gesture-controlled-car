// ----------- NodeMCU INPUT PINS ----------
#define PIN_FORWARD  2
#define PIN_BACKWARD 3
#define PIN_LEFT     4
#define PIN_RIGHT    12
#define PIN_IDLE     13

// Motor A pins
int IN1 = 8;
int IN2 = 9;

int ENA = 10;  // PWM

// Motor B pins
int IN3 = 6;
int IN4 = 7;
int ENB = 5;   // PWM

void setup() {
  // Set NodeMCU inputs
  pinMode(PIN_FORWARD, INPUT);
  pinMode(PIN_BACKWARD, INPUT);
  pinMode(PIN_LEFT, INPUT);
  pinMode(PIN_RIGHT, INPUT);
  pinMode(PIN_IDLE, INPUT);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);

  // Default speed
  analogWrite(ENA, 70);
  analogWrite(ENB, 70);
}

void loop() {

  if (digitalRead(PIN_FORWARD) == HIGH) {
    forward();
  }
  else if (digitalRead(PIN_BACKWARD) == HIGH) {
    backward();
  }
  else if (digitalRead(PIN_LEFT) == HIGH) {
    left();
  }
  else if (digitalRead(PIN_RIGHT) == HIGH) {
    right();
  }
  else if (digitalRead(PIN_IDLE) == HIGH) {
    stopMotors();
  }
  else {
    // Safety: if floating or no pin HIGH → stop
    stopMotors();
  }
}

// ---------------- MOTOR FUNCTIONS ----------------

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("forward");
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("backward");
}

void left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("left");
}

void right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("right");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("stop");
}
