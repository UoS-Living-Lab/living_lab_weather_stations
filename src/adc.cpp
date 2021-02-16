/* 
	Title: ADC.cpp
	Description: 
	Date: 13/11/2020
	Author: Ethan Bellmer 
*/

// Library Includes
#include "adc.h"
#include "init.h"

#include <ADS1219.h>

// Variable Declarations
#define SENSOR_ADDRESS 0x40
#define POWER_ADDRESS 0x41
#define SENSOR_PWR_EN PIN_PD4
#define SENSOR_DATA_READY PIN_PA5
#define PWR_DATA_READY PIN_PD3

// Class instantiations
ADS1219 sensorADC(SENSOR_DATA_READY, SENSOR_ADDRESS);
ADS1219 powerADC(PWR_DATA_READY, POWER_ADDRESS);

void ADC_INIT_ALL();
void SENSOR_PWR_PULLUP();
void SENSOR_ADC_INIT();
void POWER_ADC_INIT();
float SENSOR_ADC_GET_CHANNEL(int chan);
float POWER_ADC_GET_CHANNEL(int chan);


void ADC_INIT_ALL()
{
	SENSOR_PWR_PULLUP();
	SENSOR_ADC_INIT();
	POWER_ADC_INIT();
}

void SENSOR_PWR_PULLUP()
{
	pinMode(SENSOR_PWR_EN, OUTPUT);
	digitalWrite(SENSOR_PWR_EN, HIGH);
}

void SENSOR_ADC_INIT()
{
	sensorADC.begin();
	pinMode(SENSOR_DATA_READY, INPUT_PULLUP);
	sensorADC.setVoltageReference(REF_INTERNAL);
}

void POWER_ADC_INIT()
{
	powerADC.begin();
	pinMode(PWR_DATA_READY, INPUT_PULLUP);
	powerADC.setVoltageReference(REF_INTERNAL);
}

float SENSOR_ADC_GET_CHANNEL(int chan)
{
	return sensorADC.readSingleEnded(chan)*2.048/pow(2,23);
}

float POWER_ADC_GET_CHANNEL(int chan)
{
	float powerValue = powerADC.readSingleEnded(chan)*2.048/pow(2,23);
	switch(chan)
	{
		case 0:
			return powerValue * 4;
			break;
		case 1:
			return powerValue;
			break;
		case 2:
			return powerValue;
			break;
		case 3:
			return powerValue * 3;
			break;
	}
}