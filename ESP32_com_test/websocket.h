#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>

typedef enum __attribute__((__packed__))
{
	humidity,
	temperature,
	pressure,
	noise,
	dust,
	air_quality,
	co2
}
message_type_t;

typedef union
{
	uint32_t humidity;
	int32_t temperature;
	uint32_t pressure;
	uint32_t noise;
	uint32_t dust;
	uint32_t air_quality;
	uint32_t co2;
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