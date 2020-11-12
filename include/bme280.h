/*
*/
#include "SparkFunBME280.h" // BME280 library

struct measurementDelta
{
    long startTime;
    long endTime;
};
static struct measurementDelta md;

// Class Instantiations
static BME280 BME;


void BME_INIT();
void BME_READ();

float BME_GET_HUMIDITY();
float BME_GET_TEMP_C();
float BME_GET_PRESSURE();
float BME_GET_ALT_M();
float BME_GET_ALT_F();
float BME_GET_DEWPOINT_C();

float BME_GET_REF_PRESSURE();