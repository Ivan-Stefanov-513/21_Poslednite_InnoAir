#include "websocket.h"
#include "secure.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WebSockets2_Generic.h>

using namespace websockets2_generic;

void onMessageCallback(websockets2_generic::WebsocketsMessage message);
void onEventsCallback(websockets2_generic::WebsocketsEvent event, String data);

// KH, This certificate was updated 15.04.2021, 
// Issued on Mar 15th 2021, expired on June 13th 2021
const char echo_org_ssl_ca_cert[] PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEZTCCA02gAwIBAgIQQAF1BIMUpMghjISpDBbN3zANBgkqhkiG9w0BAQsFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTIwMTAwNzE5MjE0MFoXDTIxMDkyOTE5MjE0MFow\n" \
"MjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxCzAJBgNVBAMT\n" \
"AlIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuwIVKMz2oJTTDxLs\n" \
"jVWSw/iC8ZmmekKIp10mqrUrucVMsa+Oa/l1yKPXD0eUFFU1V4yeqKI5GfWCPEKp\n" \
"Tm71O8Mu243AsFzzWTjn7c9p8FoLG77AlCQlh/o3cbMT5xys4Zvv2+Q7RVJFlqnB\n" \
"U840yFLuta7tj95gcOKlVKu2bQ6XpUA0ayvTvGbrZjR8+muLj1cpmfgwF126cm/7\n" \
"gcWt0oZYPRfH5wm78Sv3htzB2nFd1EbjzK0lwYi8YGd1ZrPxGPeiXOZT/zqItkel\n" \
"/xMY6pgJdz+dU/nPAeX1pnAXFK9jpP+Zs5Od3FOnBv5IhR2haa4ldbsTzFID9e1R\n" \
"oYvbFQIDAQABo4IBaDCCAWQwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8E\n" \
"BAMCAYYwSwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5p\n" \
"ZGVudHJ1c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTE\n" \
"p7Gkeyxx+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEE\n" \
"AYLfEwEBATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2Vu\n" \
"Y3J5cHQub3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0\n" \
"LmNvbS9EU1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYf\n" \
"r52LFMLGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjANBgkqhkiG9w0B\n" \
"AQsFAAOCAQEA2UzgyfWEiDcx27sT4rP8i2tiEmxYt0l+PAK3qB8oYevO4C5z70kH\n" \
"ejWEHx2taPDY/laBL21/WKZuNTYQHHPD5b1tXgHXbnL7KqC401dk5VvCadTQsvd8\n" \
"S8MXjohyc9z9/G2948kLjmE6Flh9dDYrVYA9x2O+hEPGOaEOa1eePynBgPayvUfL\n" \
"qjBstzLhWVQLGAkXXmNs+5ZnPBxzDJOLxhF2JIbeQAcH5H0tZrUlo5ZYyOqA7s9p\n" \
"O5b85o3AM/OJ+CktFBQtfvBhcJVd9wvlwPsk+uyOy2HI7mNxKKgsBTt375teA2Tw\n" \
"UdHkhVNcsAKX1H7GNNLOEADksd86wuoXvg==\n" \
"-----END CERTIFICATE-----\n";
//const char* websockets_connection_string = "wss://212.5.155.204/";
const char* websockets_connection_string = "wss://10.21.42.2/";

WebsocketsClient client;

void onMessageCallback(WebsocketsMessage message)
{
	Serial.print("Got Message: ");
	Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data)
{
	(void) data;
	
	if (event == WebsocketsEvent::ConnectionOpened) 
	{
		Serial.println("Connnection Opened");
	}
	else if (event == WebsocketsEvent::ConnectionClosed) 
	{
		Serial.println("Connnection Closed");
	}
	else if (event == WebsocketsEvent::GotPing) 
	{
		Serial.println("Got a Ping!");
	}
	else if (event == WebsocketsEvent::GotPong) 
	{
		Serial.println("Got a Pong!");
	}
}

void wifi_init(void)
{
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	Serial.print("Connecting to ");
	Serial.print(WIFI_SSID);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print('.');
	}
	Serial.println('\n');
	Serial.println("Connection established!");
	Serial.print("IP address:\t");
	Serial.println(WiFi.localIP());
}

bool websocket_init(void)
{
	bool connected;

	client.onMessage(onMessageCallback);
	client.onEvent(onEventsCallback);
	// TODO: get a signed certificate and the corresponding CA certificate
	// In the meantime, don't verify the server certificate
	//client.setCACert(echo_org_ssl_ca_cert);
	client.setInsecure();
	connected = client.connect(websockets_connection_string);
	if (connected)
	{
		Serial.println("Connected!");
		//client.send("Hello to Server from the other side...");
		//client.ping();
		client.close(CloseReason_GoingAway);
	}
	else
	{
		Serial.println("Not Connected!");
	}

	return connected;
}

int websocket_send(message_t *msg)
{
	if (msg == NULL)
	{
		return -1;
	}
	if (!client.connect(websockets_connection_string))
	{
		return -2;
	}
	// Should be void * but it throws an error
	if (!client.sendBinary((char *) msg, sizeof(message_t)))
	{
		client.close(CloseReason_AbnormalClosure);
		return -3;
	}
	client.close(CloseReason_GoingAway);

	return 0;
}

//Cheatsheat
//bool WebsocketsClient::sendBinary(const char* data, const size_t len)
//WebsocketsMessage WebsocketsClient::readNonBlocking()
//void WebsocketsClient::close(const CloseReason reason)