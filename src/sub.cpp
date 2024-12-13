#include "main.hpp"

void startPump()
{
    stepper.startMove(dir * MAX_STEPS);
    stepper.enable();
    stop_flag = false;
}

void stopPump()
{
    stepper.stop();
    stepper.disable();
    stop_flag = true;
}

void setFlowrate(float flowrate)
{
    calcRPM(flowrate, &rpm);

    rpm = (dir == 1) ? rpm : rpm * PULL_COEF;
    stepper.setRPM(rpm);
    stepper.startMove(dir * MAX_STEPS);

    Serial.print("Flowrate: ");
    Serial.print(_flowrate);
    Serial.println(" mL/min");
}

void calcRPM(float flowrate, float *rpm)
{
    *rpm = (flowrate / (PI * sq(RADIUS))) / LEAD;
}

// Split a string by a delimiter
int splitString(String input, char delimiter, int arraySize, String outputArray[])
{
    int startIndex = 0;
    int endIndex = 0;
    int wordCount = 0;

    while ((endIndex = input.indexOf(delimiter, startIndex)) != -1)
    {
        if (wordCount >= arraySize)
        {
            break;
        }
        outputArray[wordCount] = input.substring(startIndex, endIndex);
        wordCount++;
        startIndex = endIndex + 1;
    }

    // Add the last segment (after the last delimiter) only if there's room
    if (wordCount < arraySize)
    {
        outputArray[wordCount] = input.substring(startIndex);
        wordCount++;
    }

    return wordCount;
}