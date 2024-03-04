#define SENSOR_TEMPERATURE_OUT 5  // write VCC
#define BUTTON_PROCESS_OUT 40     // write VCC
#define BUTTON_EMERGENCY_OUT 42   // write VCC

#define SENSOR_SWITCH_FRONT_OUT 22   // write VCC
#define SENSOR_SWITCH_BACK_OUT 23    // write VCC
#define SENSOR_SWITCH_TOP_OUT 24     // write VCC
#define SENSOR_SWITCH_BOTTON_OUT 25  // write VCC
#define RELAY_1_VAC 50               // write VCC
#define RELAY_2_LIG 51               // write VCC

#define VACCUM_RELAY 14        // write
#define LIGHT_YELLOW_RELAY 15  // write
#define LIGHT_BLUE_RELAY 16    // write
#define LIGHT_GREEN_RELAY 17   // write

#define STEP_MOTOR_PULSE_X 30   // write
#define STEP_MOTOR_ENABLE_X 31  // write
#define STEP_MOTOR_DIRECT_X 32  // write
#define STEP_MOTOR_PULSE_Y 34   // write
#define STEP_MOTOR_ENABLE_Y 35  // write
#define STEP_MOTOR_DIRECT_Y 36  // write

#define SENSOR_TEMPERATURE 4     // read
#define BUTTON_PROCESS 6         // read
#define BUTTON_EMERGENCY 7       // read
#define SENSOR_SWITCH_FRONT 10   // read
#define SENSOR_SWITCH_BACK 11    // read
#define SENSOR_SWITCH_TOP 12     // read
#define SENSOR_SWITCH_BOTTON 13  // read

bool IS_PROCESS_START = true;  // yellow start
bool IS_MOTOR_X_START_POSITION = false;
bool IS_MOTOR_Y_START_POSITION = false;
bool IS_BUTTON_PROCESS_CLICK = false;   // blue start
bool IS_TEMP_LIMIT = false;             // green start
bool IS_PROCESSION = false;             // start Timer
bool IS_MOTOR_X_WORK_POSITION = false;  // start Timer
bool IS_MOTOR_Y_WORK_POSITION = false;  // start Timer
bool IS_COUNT_DOWN_HEAT = false;
bool IS_COUNT_DOWN_PULL = false;
unsigned long START_TIME = 0;
unsigned long HEAT_MINUTE = 1;
unsigned long PULL_SECOND = 30;
unsigned long TIMER_HEAT = HEAT_MINUTE * 60 * 1000;
unsigned long TIMER_PULL = PULL_SECOND * 1000;
bool RETURN_X_MOTOR = false;
bool RETURN_Y_MORTOR = false;
bool IS_PULL_WORK = false;
bool STOP_BUTTON_CLICK = false;
bool RESET_WORK = false;
bool IS_UP = true;
bool IS_VACCUM_WORK = false;
bool RESULT = false;
// all process done set default

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  pinMode(SENSOR_SWITCH_FRONT_OUT, OUTPUT);
  pinMode(SENSOR_SWITCH_BACK_OUT, OUTPUT);
  pinMode(SENSOR_SWITCH_TOP_OUT, OUTPUT);
  pinMode(SENSOR_SWITCH_BOTTON_OUT, OUTPUT);

  pinMode(RELAY_1_VAC, OUTPUT);
  pinMode(RELAY_2_LIG, OUTPUT);

  pinMode(VACCUM_RELAY, OUTPUT);
  pinMode(LIGHT_YELLOW_RELAY, OUTPUT);
  pinMode(LIGHT_BLUE_RELAY, OUTPUT);
  pinMode(LIGHT_GREEN_RELAY, OUTPUT);

  pinMode(STEP_MOTOR_PULSE_X, OUTPUT);
  pinMode(STEP_MOTOR_ENABLE_X, OUTPUT);
  pinMode(STEP_MOTOR_DIRECT_X, OUTPUT);
  pinMode(STEP_MOTOR_PULSE_Y, OUTPUT);
  pinMode(STEP_MOTOR_ENABLE_Y, OUTPUT);
  pinMode(STEP_MOTOR_DIRECT_Y, OUTPUT);

  pinMode(SENSOR_TEMPERATURE_OUT, OUTPUT);
  pinMode(BUTTON_PROCESS_OUT, OUTPUT);
  pinMode(BUTTON_EMERGENCY_OUT, OUTPUT);

  pinMode(SENSOR_TEMPERATURE, INPUT);
  pinMode(BUTTON_PROCESS, INPUT);
  pinMode(BUTTON_EMERGENCY, INPUT);
  pinMode(SENSOR_SWITCH_FRONT, INPUT);
  pinMode(SENSOR_SWITCH_BACK, INPUT);
  pinMode(SENSOR_SWITCH_TOP, INPUT);
  pinMode(SENSOR_SWITCH_BOTTON, INPUT);

  digitalWrite(SENSOR_SWITCH_FRONT_OUT, HIGH);
  digitalWrite(SENSOR_SWITCH_BACK_OUT, HIGH);
  digitalWrite(SENSOR_SWITCH_TOP_OUT, HIGH);
  digitalWrite(SENSOR_SWITCH_BOTTON_OUT, HIGH);

  digitalWrite(RELAY_1_VAC, HIGH);
  digitalWrite(RELAY_2_LIG, HIGH);

  digitalWrite(VACCUM_RELAY, HIGH);  // active low
  digitalWrite(LIGHT_YELLOW_RELAY, LOW);
  digitalWrite(LIGHT_BLUE_RELAY, HIGH);
  digitalWrite(LIGHT_GREEN_RELAY, HIGH);

  digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
  digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
  digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
  digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
  digitalWrite(STEP_MOTOR_ENABLE_Y, HIGH);
  digitalWrite(STEP_MOTOR_DIRECT_Y, HIGH);  // LOW-UP

  digitalWrite(SENSOR_TEMPERATURE_OUT, HIGH);
  digitalWrite(BUTTON_PROCESS_OUT, HIGH);
  digitalWrite(BUTTON_EMERGENCY_OUT, 1);
  Serial.println("Heating And Waiting Button Action...");
}

void loop() {
  buttonGreen();
  // buttonRed();
  subHeater();
  heatingFlow();
  heatCountdown();
  pullFlow();
  vaccumFlow();
  pullCountdown();
  genPulse();
  disableMotorX();
  disableMotorY();
}

void buttonRed() {
  int value = digitalRead(BUTTON_PROCESS);
  if (value == 1) {
    Serial.println("Emergency click");
    digitalWrite(SENSOR_SWITCH_FRONT_OUT, HIGH);
    digitalWrite(SENSOR_SWITCH_BACK_OUT, HIGH);
    digitalWrite(SENSOR_SWITCH_TOP_OUT, HIGH);
    digitalWrite(SENSOR_SWITCH_BOTTON_OUT, HIGH);

    digitalWrite(RELAY_1_VAC, HIGH);
    digitalWrite(RELAY_2_LIG, HIGH);

    digitalWrite(VACCUM_RELAY, HIGH);  // active low
    digitalWrite(LIGHT_YELLOW_RELAY, HIGH);
    digitalWrite(LIGHT_BLUE_RELAY, HIGH);
    digitalWrite(LIGHT_GREEN_RELAY, HIGH);

    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    digitalWrite(STEP_MOTOR_ENABLE_Y, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_Y, HIGH);  // LOW-UP

    digitalWrite(SENSOR_TEMPERATURE_OUT, HIGH);
    digitalWrite(BUTTON_PROCESS_OUT, HIGH);
    digitalWrite(BUTTON_EMERGENCY_OUT, 1);
    resetAll();
  }
}

void buttonGreen() {
  int value = digitalRead(BUTTON_PROCESS);
  if (value == 1 && IS_PROCESS_START == true) {
    Serial.println("Process click");
    value = 0;
    IS_PROCESS_START = false;
    IS_BUTTON_PROCESS_CLICK = true;
    digitalWrite(VACCUM_RELAY, LOW);  // active low
    digitalWrite(LIGHT_YELLOW_RELAY, HIGH);
    digitalWrite(LIGHT_BLUE_RELAY, LOW);
    digitalWrite(LIGHT_GREEN_RELAY, LOW);
    moveMotorX("out");
    moveMotorY("up");
    Serial.println("wait temp");
  }
}

void subHeater() {
  if (IS_BUTTON_PROCESS_CLICK && digitalRead(STEP_MOTOR_ENABLE_X) == HIGH && digitalRead(STEP_MOTOR_ENABLE_Y) == HIGH) {
    int value = digitalRead(SENSOR_TEMPERATURE);
    Serial.println("waiting...");
    Serial.println(value);
    if (value == 1) {
      Serial.println("Temp is Limit");
      digitalWrite(VACCUM_RELAY, LOW);  // active low
      digitalWrite(LIGHT_YELLOW_RELAY, LOW);
      digitalWrite(LIGHT_BLUE_RELAY, HIGH);
      digitalWrite(LIGHT_GREEN_RELAY, LOW);
      IS_BUTTON_PROCESS_CLICK = false;
      IS_TEMP_LIMIT = true;
    }
  }
}

void heatingFlow() {
  if (IS_TEMP_LIMIT) {
    digitalWrite(VACCUM_RELAY, LOW);  // active low
    digitalWrite(LIGHT_YELLOW_RELAY, LOW);
    digitalWrite(LIGHT_BLUE_RELAY, LOW);
    digitalWrite(LIGHT_GREEN_RELAY, HIGH);
    Serial.println("Start Heating");
    moveMotorX("in");
    IS_TEMP_LIMIT = false;
    IS_COUNT_DOWN_HEAT = true;
    START_TIME = millis();
  }
}

void heatCountdown() {
  if (IS_COUNT_DOWN_HEAT && digitalRead(STEP_MOTOR_ENABLE_X) == HIGH) {
    Serial.println("start Counting");
    Serial.println((TIMER_HEAT - (millis() - START_TIME)) / 1000);
    Serial.println("=========================================");
    delay(1000);
    if (millis() - START_TIME >= TIMER_HEAT) {
      Serial.println("motor x out");
      IS_COUNT_DOWN_HEAT = false;
      moveMotorX("out");
      IS_PULL_WORK = true;
    }
  }
}

void pullFlow() {
  if (IS_PULL_WORK && digitalRead(STEP_MOTOR_ENABLE_X) == HIGH) {
    Serial.println("motor y work");
    IS_PULL_WORK = false;
    moveMotorY("down");
    IS_VACCUM_WORK = true;
  }
}

void vaccumFlow() {
  if (IS_VACCUM_WORK && digitalRead(STEP_MOTOR_ENABLE_Y) == HIGH) {
    Serial.println("vaccum work");
    IS_COUNT_DOWN_PULL = true;
    IS_VACCUM_WORK = false;
    moveVaccum("on");
    START_TIME = millis();
  }
}

void pullCountdown() {
  if (IS_COUNT_DOWN_PULL) {
    Serial.println("start Counting");
    Serial.println((TIMER_PULL - (millis() - START_TIME)) / 1000);
    Serial.println("=========================================");
    delay(1000);
    if (millis() - START_TIME >= TIMER_PULL) {
      Serial.println("work done");
      IS_COUNT_DOWN_PULL = false;
      moveVaccum("off");
      moveMotorY("up");
      RESULT = true;
    }
  }
}

void setStart() {
  if (RESULT && digitalRead(STEP_MOTOR_ENABLE_Y) == LOW) {
    resetAll();
  }
}

void resetAll() {
  Serial.println("Please Click Green Button to continute");
  IS_PROCESS_START = true;  // yellow start
  IS_MOTOR_X_START_POSITION = false;
  IS_MOTOR_Y_START_POSITION = false;
  IS_BUTTON_PROCESS_CLICK = false;   // blue start
  IS_TEMP_LIMIT = false;             // green start
  IS_PROCESSION = false;             // start Timer
  IS_MOTOR_X_WORK_POSITION = false;  // start Timer
  IS_MOTOR_Y_WORK_POSITION = false;  // start Timer
  IS_COUNT_DOWN_HEAT = false;
  IS_COUNT_DOWN_PULL = false;
  RETURN_X_MOTOR = false;
  RETURN_Y_MORTOR = false;
  IS_PULL_WORK = false;
  RESET_WORK = false;
  STOP_BUTTON_CLICK = false;
  IS_VACCUM_WORK = false;
  RESULT = false;
}

void moveMotorX(String direct) {
  Serial.println("Motor X Controller");
  if (direct == "out") {
    digitalWrite(STEP_MOTOR_DIRECT_X, LOW);
    digitalWrite(STEP_MOTOR_ENABLE_X, LOW);
  }
  if (direct == "in") {
    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_ENABLE_X, LOW);
  }
}

void moveMotorY(String direct) {
  Serial.println("Motor Y Controller");
  if (direct == "up") {
    digitalWrite(STEP_MOTOR_DIRECT_Y, LOW);
    digitalWrite(STEP_MOTOR_ENABLE_Y, LOW);
  }
  if (direct == "down") {
    digitalWrite(STEP_MOTOR_DIRECT_Y, HIGH);
    digitalWrite(STEP_MOTOR_ENABLE_Y, LOW);
  }
}

void genPulse() {
  int x_value = digitalRead(STEP_MOTOR_ENABLE_X);
  int y_value = digitalRead(STEP_MOTOR_ENABLE_Y);
  if (x_value == 0 || y_value == 0) {
    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    delay(1);
    digitalWrite(STEP_MOTOR_PULSE_X, LOW);
    digitalWrite(STEP_MOTOR_PULSE_Y, LOW);
  }
}

void disableMotorX() {
  if (digitalRead(STEP_MOTOR_ENABLE_X) == LOW) {
    if (digitalRead(STEP_MOTOR_DIRECT_X) == LOW) {
      if (digitalRead(SENSOR_SWITCH_FRONT) != digitalRead(SENSOR_SWITCH_FRONT_OUT)) {
        Serial.println("motor x stop");
        digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
      } else {
        int value = digitalRead(SENSOR_SWITCH_FRONT_OUT);
        digitalWrite(SENSOR_SWITCH_FRONT_OUT, !value);
      }
    } else {
      if (digitalRead(SENSOR_SWITCH_BACK) != digitalRead(SENSOR_SWITCH_BACK_OUT)) {
        Serial.println("motor x stop");
        digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
      } else {
        int value = digitalRead(SENSOR_SWITCH_BACK_OUT);
        digitalWrite(SENSOR_SWITCH_BACK_OUT, !value);
      }
    }
  }
}

void disableMotorY() {
  if (digitalRead(STEP_MOTOR_ENABLE_Y) == LOW) {
    if (digitalRead(STEP_MOTOR_DIRECT_Y) == LOW) {
      if (digitalRead(SENSOR_SWITCH_TOP) != digitalRead(SENSOR_SWITCH_TOP_OUT)) {
        Serial.println("motor y stop");
        digitalWrite(STEP_MOTOR_ENABLE_Y, HIGH);
      } else {
        int value = digitalRead(SENSOR_SWITCH_TOP_OUT);
        digitalWrite(SENSOR_SWITCH_TOP_OUT, !value);
      }
    } else {
      if (digitalRead(SENSOR_SWITCH_BOTTON) != digitalRead(SENSOR_SWITCH_BOTTON_OUT)) {
        Serial.println("motor y stop");
        digitalWrite(STEP_MOTOR_ENABLE_Y, HIGH);
      } else {
        int value = digitalRead(SENSOR_SWITCH_BOTTON_OUT);
        digitalWrite(SENSOR_SWITCH_BOTTON_OUT, !value);
      }
    }
  }
}

void moveVaccum(String swt) {
  if (swt == "on") {
    digitalWrite(VACCUM_RELAY, HIGH);  // active low
    digitalWrite(LIGHT_YELLOW_RELAY, LOW);
    digitalWrite(LIGHT_BLUE_RELAY, LOW);
    digitalWrite(LIGHT_GREEN_RELAY, HIGH);
  } else {
    digitalWrite(VACCUM_RELAY, LOW);  // active low
    digitalWrite(LIGHT_YELLOW_RELAY, HIGH);
    digitalWrite(LIGHT_BLUE_RELAY, LOW);
    digitalWrite(LIGHT_GREEN_RELAY, LOW);
  }
}