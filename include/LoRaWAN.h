/*
*/
//#include <rn2xx3.h> // RN2483 LoRa radio library
#include "init.h"
#include <TheThingsNetwork.h>

// Class Instantiations
static TheThingsNetwork ttn(loraUSART, usbUSART, freqPlan);

void TTN_INIT();
void TTN_SEND_BYTES(uint8_t payload[], port_t port = 1);
int8_t TTN_GET_BYTES(port_t port = 1);