#include <main.hpp>
// #define DEBUG

A4988 stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

bool stop_flag = true;
int dir = 1;
int pos = MAX_STEPS / 2;
float _flowrate; // [mL/min]
float rpm; // [/min]

unsigned long previousMillis = 0;   // 前回のシリアル通信の時間を記録
const unsigned long interval = 500; // シリアル通信の間隔 (ミリ秒)
long steps_remaining;
unsigned long previousStep = 0;
unsigned long currentStep;

#define debounce_delay 200
unsigned long last_button_press = 0;

void setup()
{
    Serial.begin(BAUD_RATE);

    pinMode(START_PIN, INPUT_PULLUP);
    pinMode(STOP_PIN, INPUT_PULLUP);

    _flowrate = 5;
    calcRPM(_flowrate, &rpm);
    stepper.begin(rpm, MICROSTEPS);
}

void loop()
{
    steps_remaining = stepper.getStepsRemaining();

    if (steps_remaining > 0)
    {
        currentStep = stepper.getStepsCompleted();
        // ステップが増えていたステップを１つ増やす
        if (currentStep != previousStep)
        {
            previousStep = currentStep; // ステップを更新
            dir = stepper.getDirection();
            pos = (dir == 1) ? pos + 1 : pos - 1;
        }
    }

    stepper.nextAction();

    if (!stop_flag)
    {
        if (dir == 1 && pos >= MAX_STEPS)
        {
            rpm = rpm * PULL_COEF;
            stepper.setRPM(rpm);
            stepper.startMove(-MAX_STEPS);
        }
     }

    // *********************************************************************************************************
    // Button control
    if ((millis() - last_button_press) > debounce_delay)
    {
        if (digitalRead(START_PIN) == LOW)
        {
            if (stop_flag)
            {
                startPump();
            }
            else
            {
                stopPump();
            }

            last_button_press = millis();
        }
    }

    //*********************************************************************************************************
    // Serial communication
    if (Serial.available() > 0)
    {
        String recievedData = Serial.readStringUntil('\n'); // 改行まで読込

        char delimiter = ' ';
        int maxWords = 5;
        String words[maxWords];
        int wordCount = splitString(recievedData, delimiter, maxWords, words);

        if (wordCount > 0)
        {
            if (words[0] == "Start")
            {
                startPump();
            }
            else if (words[0] == "Stop")
            {
                stopPump();
            }
            else if (words[0] == "Set")
            {
                setFlowrate(words[1].toFloat());
            }
            else
            {
                Serial.println("unkonwn command");
            }
        }
    }

#ifdef DEBUG
    unsigned long currentMillis = millis();

    // 一定間隔が経過した場合にシリアル通信を行う
    if ((currentMillis - previousMillis) >= interval)
    {
        previousMillis = currentMillis;
        // Serial.print("motorX_state:");
        // Serial.println(stepperX.getCurrentState());
        // Serial.print("motorY_state:");
        // Serial.println(stepperY.getCurrentState());
        Serial.print(">pos_X:");
        Serial.println(pos_X);
        Serial.print(">pos_Y:");
        Serial.println(pos_Y);

        Serial.print(">step_remaining_X:");
        Serial.println(steps_remaining_X);
        Serial.print(">step_remaining_Y:");
        Serial.println(steps_remaining_Y);
        Serial.print(">rpm_X:");
        Serial.println(rpm_X);
        Serial.print(">rpm_Y:");
        Serial.println(rpm_Y);
    }
#endif
}