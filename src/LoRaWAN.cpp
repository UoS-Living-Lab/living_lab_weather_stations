/*

*/

#include "init.h"
#include "LoRaWAN.h"
#include "ttn-otaa-credentials.h"
//#include "ttn-apb-credentials.h"


void INIT_TTN()
{
		// Wait a maximum of 10s for Serial Monitor
	while (!usbUSART && millis() < 10000)
		;

	usbUSART.println("-- STATUS");
	ttn.showStatus();

	usbUSART.println("-- JOIN");
	ttn.join(appEui, appKey);
}