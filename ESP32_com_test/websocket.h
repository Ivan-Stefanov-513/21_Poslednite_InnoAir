#ifndef WEBSOCKET_H
#define WEBSOCKET_H

// This is commented because some idiot decided to not declare but to DEFINE functions in header files
// https://github.com/khoih-prog/WebSockets2_Generic/blob/master/src/WebSockets2_Generic_Client.hpp
//#include <WebSockets2_Generic.h>
#include <Arduino.h>

typedef enum __attribute__((__packed__))
{
	humidity,
	temperature,
	noise,
	dust
}
message_type_t;

// TODO: figure out the correct types
typedef union
{
	uint16_t humidity;
	uint16_t temperature;
	float noise;
	uint32_t dust;
}
message_data_t;

typedef struct __attribute__((__packed__))
{
	message_type_t message_type;
	message_data_t message_data;
}
message_t;

//void onMessageCallback(websockets2_generic::WebsocketsMessage message);
//void onEventsCallback(websockets2_generic::WebsocketsEvent event, String data);
void wifi_init(void);
bool websocket_init(void);
int websocket_send(message_t *msg);

#endif