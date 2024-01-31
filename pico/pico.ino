#include <Adafruit_TinyUSB.h>
#include <WiFi.h>
#include "nsnds.h"

static Adafruit_USBD_HID hid(HPS_DESC_REPORT, sizeof HPS_DESC_REPORT, HID_ITF_PROTOCOL_NONE, 2, false);
static WiFiUDP udp;
static char buffer[8] = {};
static bool led = HIGH;

void setup(void) {
	Serial.end();
	pinMode(LED_BUILTIN, OUTPUT);
	hid.begin();
	WiFi.beginAP(NSNDS_SSID);
	udp.begin(NSNDS_PORT);
	while (!TinyUSBDevice.mounted()) {
		delay(1);
	}
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void) {
	if (udp.parsePacket()) {
		udp.read(buffer, 8);
		hid.sendReport(0, buffer, 8);
		digitalWrite(LED_BUILTIN, led = !led);
	}
	delay(1);
}
