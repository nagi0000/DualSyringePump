#ifndef SYRINGE_PUMP_H
#define SYRINGE_PUMP_H

#include <Arduino.h>
#include "A4988.h"

// Syringe settings
#define SYRINGE_VOLUME 10 // [mL]
#define RADIUS 14.567 / 10 * 0.5 // [cm]
#define SYRINGE_LENGTH (10 / (PI * sq(RADIUS)))

// Lead screw settings
#define LEAD 0.8

// Motor settings
#define MOTOR_STEPS 200 // Motor steps per revolution.
#define MICROSTEPS 16
#define MAX_ROTATION ((int)SYRINGE_LENGTH / LEAD)
#define MAX_STEPS (MOTOR_STEPS * MICROSTEPS * MAX_ROTATION)
#define END_STOP_STEPS MAX_STEPS
#define MOTOR_ACCEL 200
#define MOTOR_DECEL 200
#define PULL_COEF 2.0

// Stepper driver
#define DIR 8
#define STEP 9
#define SLEEP 13
#define START_PIN 2
#define STOP_PIN 3

// Serial port settings
#define BAUD_RATE 115200

// Variables for control
extern A4988 stepper;
extern float _flowrate;
extern bool stop_flag;
extern int dir;
extern int pos;
extern float rpm;

// prototype declaration
void startPump();
void stopPump();
void calcRPM(float flowrate, float *rpm);
void setFlowrate(float flowrate);
int splitString(String input, char delimiter, int arraySize, String outputArray[]);

#endif // SYRINGE_PUMP_H
