/* 
	Title: ADC.h
	Description: 
	Date: 13/11/2020
	Author: Ethan Bellmer 
*/

void ADC_INIT_ALL();
void SENSOR_ADC_INIT();
void POWER_ADC_INIT();
float SENSOR_ADC_GET_CHANNEL(int);
float POWER_ADC_GET_CHANNEL(int);
bool SENSOR_ADC_GET_RAIN();