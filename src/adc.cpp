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


void SENSOR_ADC_INIT()
{
	sensorADC.begin();
	
	pinMode(SENSOR_DATA_READY, INPUT_PULLUP);
	
	pinMode(SENSOR_PWR_EN, OUTPUT);
	digitalWrite(SENSOR_PWR_EN, HIGH);

	sensorADC.setVoltageReference(REF_EXTERNAL);
}

void POWER_ADC_INIT()
{
	powerADC.begin();

	pinMode(PWR_DATA_READY, INPUT_PULLUP);

	//pinMode(, OUTPUT);
	//digitalWrite(, HIGH);

	powerADC.setVoltageReference(REF_EXTERNAL);
}

float SENSOR_ADC_GET_CHANNEL(int chan)
{
	return sensorADC.readSingleEnded(chan)*2.048/pow(2,23);
	//return sensorADC.readSingleEnded(chan);
}

float ADC_GET_DIFFERENTIAL_2_3()
{
	//return sensorADC.readDifferential_2_3()*2.048/pow(2,23);
	return sensorADC.readDifferential_2_3();
}