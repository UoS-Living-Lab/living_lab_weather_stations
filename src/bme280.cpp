/*
*/

#include "init.h"
#include "bme280.h"

void BME_INIT()
{
	BME.setI2CAddress(0x76);
	BME.beginI2C(); // Start the I2C interface on the BME
	BME.setMode(MODE_SLEEP); // Set BME to sleep
}

void BME_READ()
{
	BME.setMode(MODE_FORCED); //Wake up sensor and take reading

	//long startTime = millis();
	md.startTime = millis();
	while(BME.isMeasuring() == false) ; //Wait for sensor to start measurment
	while(BME.isMeasuring() == true) ; //Hang out while sensor completes the reading    
	//long endTime = millis();
	md.endTime = millis();
	//Sensor is now back asleep but we get get the data
}

float BME_GET_HUMIDITY()
{
	return BME.readFloatHumidity();
}
float BME_GET_TEMP_C()
{
	return BME.readTempC();
}
float BME_GET_PRESSURE()
{
	return BME.readFloatPressure();
}
float BME_GET_ALT_M()
{
	return BME.readFloatAltitudeMeters();
}
float BME_GET_ALT_F()
{
	return BME.readFloatAltitudeFeet();
}
float BME_GET_REF_PRESSURE()
{
	return BME.getReferencePressure();
}
float BME_GET_DEWPOINT_C()
{
	return BME.dewPointC();
}
