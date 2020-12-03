/* 
	Title: Init.cpp
	Description: 
	Date: 11/11/2020
	Author: Ethan Bellmer 
*/

#include "init.h"

void I2C_INIT()
{
    Wire.pins(PIN_PA2, PIN_PA3);
	Wire.begin(); // Start the I2C interface
	//Wire.setClock(400000); //Increase to fast I2C speed!
	Wire.setClock(100000);
}