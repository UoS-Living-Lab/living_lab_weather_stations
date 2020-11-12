/*
*/
//#include <rn2xx3.h> // RN2483 LoRa radio library
#include "init.h"
#include <TheThingsNetwork.h>

// Class Instantiations
static TheThingsNetwork ttn(loraUSART, usbUSART, freqPlan);

void INIT_TTN();