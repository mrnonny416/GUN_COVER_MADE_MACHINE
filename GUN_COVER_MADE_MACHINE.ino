#define SENSOR_TEMPERATURE_OUT 5  // write VCC
#define BUTTON_PROCESS_OUT 7      // write VCC
#define BUTTON_EMERGENCY_OUT 9    // write VCC

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
#define BUTTON_EMERGENCY 8       // read
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
int HEAT_MINUTE = 3;
int PULL_MINUTE = 3;
unsigned long TIMER_HEAT = HEAT_MINUTE * 60 * 1000;
unsigned long TIMER_PULL = PULL_MINUTE * 60 * 1000;
bool RETURN_X_MORTOR = false;
bool RETURN_Y_MORTOR = false;
bool PULL_WORK = false;
bool RESET_WORK = false;
// all process done set default

void setup() {
  Serial.begin(115200);
  Serial.println();
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
  digitalWrite(BUTTON_EMERGENCY_OUT, HIGH);
}

void loop() {
  if (IS_PROCESS_START) {
    digitalWrite(LIGHT_YELLOW_RELAY, LOW);
  }

  if (digitalRead(BUTTON_PROCESS) == HIGH && IS_PROCESS_START == true && IS_TEMP_LIMIT == false) {
    IS_BUTTON_PROCESS_CLICK = true;
    IS_PROCESS_START = false;
  }

  if (IS_BUTTON_PROCESS_CLICK) {
    digitalWrite(LIGHT_YELLOW_RELAY, HIGH);
    digitalWrite(LIGHT_BLUE_RELAY, LOW);

    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_Y, LOW);  // up

    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    delay(1);
    digitalWrite(STEP_MOTOR_PULSE_X, LOW);

    if (digitalRead(SENSOR_SWITCH_FRONT) == LOW) {
      IS_MOTOR_X_START_POSITION = true;
    }
    if (digitalRead(SENSOR_SWITCH_TOP) == LOW) {
      IS_MOTOR_Y_START_POSITION = true;
    }

    if (!IS_MOTOR_X_START_POSITION) {
      digitalWrite(STEP_MOTOR_ENABLE_X, LOW);
    } else {
      digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
    }

    if (!IS_MOTOR_Y_START_POSITION) {
      digitalWrite(STEP_MOTOR_ENABLE_Y, LOW);
    } else {
      digitalWrite(STEP_MOTOR_ENABLE_Y, HIGH);
    }

    if (digitalRead(SENSOR_TEMPERATURE) == HIGH && IS_MOTOR_X_START_POSITION && IS_MOTOR_Y_START_POSITION) {
      IS_TEMP_LIMIT = true;
      IS_BUTTON_PROCESS_CLICK = false;
    }
  }

  if (IS_TEMP_LIMIT) {
    digitalWrite(LIGHT_BLUE_RELAY, LOW);
    digitalWrite(LIGHT_GREEN_RELAY, LOW);

    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_Y, HIGH);  // up

    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    delay(1);
    digitalWrite(STEP_MOTOR_PULSE_X, LOW);
    digitalWrite(STEP_MOTOR_PULSE_Y, LOW);
    // x sensor
    IS_MOTOR_X_START_POSITION = false;
    if (digitalRead(SENSOR_SWITCH_BACK) == LOW) {
      IS_MOTOR_X_WORK_POSITION = true;
    }
    // run motor
    if (!IS_MOTOR_X_WORK_POSITION) {
      digitalWrite(STEP_MOTOR_ENABLE_X, LOW);
    } else {
      digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
      IS_TEMP_LIMIT = false;
      IS_COUNT_DOWN_HEAT = true;
      START_TIME = millis();
    }
  }

  if (IS_COUNT_DOWN_HEAT) {
    if (millis() - START_TIME >= TIMER_HEAT) {
      IS_COUNT_DOWN_HEAT = false;
      RETURN_X_MORTOR = true;
    }
  }

  if (RETURN_X_MORTOR) {
    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_Y, LOW);  // up

    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    delay(1);
    digitalWrite(STEP_MOTOR_PULSE_X, LOW);
    digitalWrite(STEP_MOTOR_PULSE_Y, LOW);
    // x sensor
    IS_MOTOR_X_START_POSITION = false;
    if (digitalRead(SENSOR_SWITCH_FRONT) == LOW) {
      IS_MOTOR_X_START_POSITION = true;
    }
    // run motor
    if (!IS_MOTOR_X_START_POSITION) {
      digitalWrite(STEP_MOTOR_ENABLE_X, LOW);
    } else {
      digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
      RETURN_X_MORTOR = false;
      PULL_WORK = true;
    }
  }

  if (PULL_WORK) {
    digitalWrite(VACCUM_RELAY, LOW);
    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_Y, HIGH);  // down

    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    delay(1);
    digitalWrite(STEP_MOTOR_PULSE_X, LOW);
    digitalWrite(STEP_MOTOR_PULSE_Y, LOW);

    IS_MOTOR_Y_WORK_POSITION = false;
    if (digitalRead(SENSOR_SWITCH_BOTTON) == LOW) {
      IS_MOTOR_X_WORK_POSITION = true;
    }
    // run motor
    if (!IS_MOTOR_X_WORK_POSITION) {
      digitalWrite(STEP_MOTOR_ENABLE_X, LOW);
    } else {
      digitalWrite(STEP_MOTOR_ENABLE_X, HIGH);
      PULL_WORK = false;
      IS_COUNT_DOWN_PULL = true;
      START_TIME = millis();
    }
  }

  if (IS_COUNT_DOWN_PULL) {
    if (millis() - START_TIME >= TIMER_PULL) {
      IS_COUNT_DOWN_PULL = false;
      RETURN_Y_MORTOR = true;
    }
  }

  if (RETURN_Y_MORTOR) {
    digitalWrite(STEP_MOTOR_DIRECT_X, HIGH);
    digitalWrite(STEP_MOTOR_DIRECT_Y, LOW);  // up

    digitalWrite(STEP_MOTOR_PULSE_X, HIGH);
    digitalWrite(STEP_MOTOR_PULSE_Y, HIGH);
    delay(1);
    digitalWrite(STEP_MOTOR_PULSE_X, LOW);
    digitalWrite(STEP_MOTOR_PULSE_Y, LOW);
    // x sensor
    IS_MOTOR_Y_START_POSITION = false;
    if (digitalRead(SENSOR_SWITCH_FRONT) == LOW) {
      IS_MOTOR_Y_START_POSITION = true;
    }
    // run motor
    if (!IS_MOTOR_Y_START_POSITION) {
      digitalWrite(STEP_MOTOR_ENABLE_Y, LOW);
    } else {
      digitalWrite(STEP_MOTOR_ENABLE_Y, HIGH);
      RETURN_Y_MORTOR = false;
      digitalWrite(VACCUM_RELAY, HIGH);
      RESET_WORK = true;
    }
  }

  if (RESET_WORK) {
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
    RETURN_X_MORTOR = false;
    RETURN_Y_MORTOR = false;
    PULL_WORK = false;
    RESET_WORK = false;
  }
}
