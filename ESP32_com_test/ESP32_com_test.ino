#include "websocket.h"

void setup(void)
{
	// UART initialisation
	Serial.begin(115200);

	// Wi-Fi initialisation
	wifi_init();

	// Secure Websocket initialisation
	websocket_init();
}

void loop(void)
{

}
