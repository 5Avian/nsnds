// https://github.com/arpruss/USBComposite_stm32f1/blob/master/USBHID.h was heavily used as a reference.

#ifndef NSNDS_H
#define NSNDS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NSNDS_SSID				"nsnds"
#define NSNDS_PORT				4020

#define HPS_AXIS_MAX			255
#define HPS_AXIS_NEUTRAL		128
#define HPS_AXIS_MIN			0

#define HPS_DPAD_TOP			0
#define HPS_DPAD_TOPRIGHT		1
#define HPS_DPAD_RIGHT			2
#define HPS_DPAD_BOTTOMRIGHT	3
#define HPS_DPAD_BOTTOM			4
#define HPS_DPAD_BOTTOMLEFT		5
#define HPS_DPAD_LEFT			6
#define HPS_DPAD_TOPLEFT		7
#define HPS_DPAD_NEUTRAL		8

#define HPS_BUTTON_Y			(1 << 0)
#define HPS_BUTTON_B			(1 << 1)
#define HPS_BUTTON_A			(1 << 2)
#define HPS_BUTTON_X			(1 << 3)
#define HPS_BUTTON_L			(1 << 4)
#define HPS_BUTTON_R			(1 << 5)
#define HPS_BUTTON_ZL			(1 << 6)
#define HPS_BUTTON_ZR			(1 << 7)
#define HPS_BUTTON_MINUS		(1 << 8)
#define HPS_BUTTON_PLUS			(1 << 9)
#define HPS_BUTTON_LSTICK		(1 << 10)
#define HPS_BUTTON_RSTICK		(1 << 11)
#define HPS_BUTTON_HOME			(1 << 12)
#define HPS_BUTTON_CAPTURE		(1 << 13)
#define HPS_BUTTON_EXTRA1		(1 << 14)
#define HPS_BUTTON_EXTRA2		(1 << 15)

#define HPS_DESC_REPORT			(uint8_t []) { \
	0x05, 0x01, \
	0x09, 0x05, \
	0xA1, 0x01, \
	0x15, 0x00, \
	0x25, 0x01, \
	0x35, 0x00, \
	0x45, 0x01, \
	0x75, 0x01, \
	0x95, 0x0e, \
	0x05, 0x09, \
	0x19, 0x01, \
	0x29, 0x0e, \
	0x81, 0x02, \
	0x95, 0x02, \
	0x81, 0x01, \
	0x05, 0x01, \
	0x25, 0x07, \
	0x46, 315 & 0xFF, 315 >> 8, \
	0x75, 0x04, \
	0x95, 0x01, \
	0x65, 20, \
	0x09, 57, \
	0x81, 66, \
	0x65, 0, \
	0x95, 0x01, \
	0x81, 1, \
	0x26, 255 & 0xFF, 255 >> 8, \
	0x46, 255 & 0xFF, 255 >> 8, \
	0x09, 48, \
	0x09, 49, \
	0x09, 50, \
	0x09, 53, \
	0x75, 0x08, \
	0x95, 0x04, \
	0x81, 0x02, \
	0x75, 0x08, \
	0x95, 0x01, \
	0x81, 0x01, \
	0xC0, }

typedef struct hps_report {
  uint16_t	buttons;
  uint8_t	dpad;
  uint8_t	lx;
  uint8_t	ly;
  uint8_t	rx;
  uint8_t	ry;
  uint8_t	pad;
} hps_report_t;

#ifdef __cplusplus
}
#endif
#endif
