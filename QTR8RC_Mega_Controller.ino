#include <QTRSensors.h>

/* ================= QTR ================= */
#define NUM_SENSORS 8

uint8_t qtrPins[NUM_SENSORS] = {
  36, 34, 32, 30, 28, 26, 24, 22
};

uint16_t qtrValues[NUM_SENSORS];
QTRSensors qtr;

/* ================= MOTOR (TB6612FNG) ================= */
#define PWMA 2
#define AIN1 4
#define AIN2 3

#define PWMB 8
#define BIN1 6
#define BIN2 7

#define STBY 5

/* ================= CONTROL ================= */
int speedVal = 150;

/* ================= SETUP ================= */
void setup() {
  Serial3.begin(9600);   // ESP8266

  // Motor pins
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);   // enable driver

  // QTR init
  qtr.setTypeRC();
  qtr.setSensorPins(qtrPins, NUM_SENSORS);
  qtr.setTimeout(2500);

  // QTR calibration (simple)
  for (int i = 0; i < 150; i++) {
    qtr.calibrate();
    delay(10);
  }
}

/* ================= LOOP ================= */
void loop() {

  /* ---- RECEIVE COMMAND FROM ESP ---- */
  if (Serial3.available()) {
    String cmd = Serial3.readStringUntil('\n');
    cmd.trim();
    handleCommand(cmd);
  }

  /* ---- SEND QTR DATA (CLEAN & SLOW) ---- */
  qtr.read(qtrValues);

  Serial3.print("QTR: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial3.print(qtrValues[i]);
    if (i < NUM_SENSORS - 1) Serial3.print(" | ");
  }
  Serial3.println();

  delay(150);   // slow = readable
}

/* ================= COMMAND HANDLER ================= */
void handleCommand(String c) {

  if (c == "F")       move(speedVal, speedVal);
  else if (c == "B")  move(-speedVal, -speedVal);
  else if (c == "L")  move(-speedVal, speedVal);
  else if (c == "R")  move(speedVal, -speedVal);
  else if (c == "S")  move(0, 0);

  else if (c.startsWith("SPD:")) {
    speedVal = constrain(c.substring(4).toInt(), 0, 255);
  }
}

/* ================= MOTOR CONTROL ================= */
void move(int left, int right) {
  motor(PWMA, AIN1, AIN2, left);
  motor(PWMB, BIN1, BIN2, right);
}

void motor(int pwm, int in1, int in2, int val) {
  if (val > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if (val < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    val = -val;
  }
  else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(pwm, val);
}
