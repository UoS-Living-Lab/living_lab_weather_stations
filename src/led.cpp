/*
*/

#include "led.h"
#include "init.h"

// Variable Definitiions
#define LED_PIN_0 PIN_PD0
#define LED_PIN_1 PIN_PD1

void ledInit();
void LED_0_ON();
void LED_0_OFF();
void LED_1_ON();
void LED_1_OFF();
void ledBlink();

void ledInit()
{
	// initialize LED digital pin as an output.
	pinMode(LED_PIN_0, OUTPUT);
    digitalWrite(LED_PIN_0, HIGH); // Pulling the pin high turns off the LED
	pinMode(LED_PIN_1, OUTPUT);
    digitalWrite(LED_PIN_1, HIGH);
}

void LED_0_ON()
{
    digitalWrite(LED_PIN_0, LOW);
}

void LED_0_OFF()
{
    digitalWrite(LED_PIN_0, HIGH);
}

void LED_1_ON()
{
    digitalWrite(LED_PIN_1, LOW);
}

void LED_1_OFF()
{
    digitalWrite(LED_PIN_1, HIGH);
}

void ledBlink()
{
	// turn the LED on (HIGH is the voltage level)
    LED_0_ON();
    LED_1_OFF();

	// wait for a second
	delay(1000);
	
	// turn the LED off by making the voltage LOW
	LED_0_OFF();
    LED_1_ON();
}