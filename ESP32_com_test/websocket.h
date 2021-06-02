#ifndef WEBSOCKET_H
#define WEBSOCKET_H

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
	uint16_t station_id;
}
message_t;

void wifi_init(void);
bool websocket_init(void);
int websocket_send(message_t *msg);

#endif