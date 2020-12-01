/* 
	Title: Init.h
	Description: 
	Date: 11/11/2020
	Author: Ethan Bellmer 
*/

#include <Arduino.h>
#include "usart.h"
#include <Wire.h> // I2C library
//#include <SPI.h>
#include "led.h"
#include "rtc.h"
#include <avr/sleep.h>

#define freqPlan TTN_FP_EU868

void I2C_INIT();