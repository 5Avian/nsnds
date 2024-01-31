#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "nsnds.h"

// button zones
#define VBLANKS_DEC		(rect_t) {32, 64, 54, 86}
#define VBLANKS_INC		(rect_t) {200, 64, 222, 86}
#define PACKETS_DEC		(rect_t) {32, 96, 54, 118}
#define PACKETS_INC		(rect_t) {200, 96, 222, 118}

typedef struct rect {
	int x1;
	int y1;
	int x2;
	int y2;
} rect_t;

static bool touch_pressed(touchPosition pos, rect_t rect) {
	return (pos.px >= rect.x1 && pos.px <= rect.x2) && (pos.py >= rect.y1 && pos.py <= rect.y2);
}

static void printf_addr(char *fmt, uint32_t addr) {
	printf(fmt, (addr >> 0) & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
}

static void print_header(char *hdr) {
	static char str[33] = "- ______________________________";
	strlcpy(str + 2, hdr, sizeof str - 2);
	strlcat(str, " -----------------------------", sizeof str);
	printf("%s", str);
}

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;
	if (!consoleDemoInit()) {
		return 1;
	}
	if (!Wifi_InitDefault(true)) {
		printf("failed to connect to WiFi\n");
		return 1;
	}
	struct in_addr gateway_addr;
	struct in_addr addr = Wifi_GetIPInfo(&gateway_addr, NULL, NULL, NULL);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = gateway_addr.s_addr;
	sockaddr.sin_port = htons(NSNDS_PORT);
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	uint16_t keys = 0;
	char buf[8] = {0, 0, HPS_DPAD_NEUTRAL, HPS_AXIS_NEUTRAL, HPS_AXIS_NEUTRAL, HPS_AXIS_NEUTRAL, HPS_AXIS_NEUTRAL, 0};
	char prev_buf[8] = {0, 0, HPS_DPAD_NEUTRAL, HPS_AXIS_NEUTRAL, HPS_AXIS_NEUTRAL, HPS_AXIS_NEUTRAL, HPS_AXIS_NEUTRAL, 0};
	int vblanks = 2; // amount of additional vblanks to wait for
	int packets = 2; // amount of additional packets to send
	int packets_to_send = packets + 1;
	int packets_sent = 0;
	bool can_press = true;
	
	while (true) {
		for (int i = -1; i < vblanks; i++) {
			swiWaitForVBlank();
		}

		// touch screen controls
		touchPosition touch_pos;
		touchRead(&touch_pos);
		if (touch_pressed(touch_pos, VBLANKS_DEC) && vblanks > 0) {
			vblanks -= can_press;
			can_press = false;
		} else if (touch_pressed(touch_pos, VBLANKS_INC) && vblanks < 255) {
			vblanks += can_press;
			can_press = false;
		} else if (touch_pressed(touch_pos, PACKETS_DEC) && packets > 0) {
			packets -= can_press;
			can_press = false;
		} else if (touch_pressed(touch_pos, PACKETS_INC) && packets < 255) {
			packets += can_press;
			can_press = false;
		} else {
			can_press = true;
		}

		// hid payload
		scanKeys();
		uint16_t keys = keysCurrent();
		if (keys & KEY_LID) {
			continue;
		}
		uint16_t sent_keys =
			((keys & KEY_A) != 0) * HPS_BUTTON_A |
			((keys & KEY_B) != 0) * HPS_BUTTON_B |
			((keys & KEY_X) != 0) * HPS_BUTTON_X |
			((keys & KEY_Y) != 0) * HPS_BUTTON_Y |
			((keys & KEY_L) != 0) * HPS_BUTTON_L |
			((keys & KEY_R) != 0) * HPS_BUTTON_R |
			((keys & KEY_START) != 0) * HPS_BUTTON_PLUS |
			((keys & KEY_SELECT) != 0) * HPS_BUTTON_MINUS;
		memcpy(buf, &sent_keys, sizeof sent_keys);
		if (keys & KEY_RIGHT) {
			buf[3] = HPS_AXIS_MAX;
		} else if (keys & KEY_LEFT) {
			buf[3] = HPS_AXIS_MIN;
		} else {
			buf[3] = HPS_AXIS_NEUTRAL;
		}
		if (keys & KEY_DOWN) {
			buf[4] = HPS_AXIS_MAX;
		} else if (keys & KEY_UP) {
			buf[4] = HPS_AXIS_MIN;
		} else {
			buf[4] = HPS_AXIS_NEUTRAL;
		}

		// buffer packets
		if (memcmp(buf, prev_buf, sizeof buf) != 0) {
			packets_to_send = packets + 1;
			memcpy(prev_buf, buf, sizeof buf);
		}
		if (packets_to_send > 0) {
			sendto(sock, buf, sizeof buf, 0, (struct sockaddr *) &sockaddr, sizeof sockaddr);
			packets_sent++;
			packets_to_send--;
		}

		consoleClear();
		printf("\x1b[0;0H"); // reset to start
		printf("nsnds Switch Controller\n");
		printf("by 5Avian\n");
		print_header("SETTINGS");
		printf("Additional VBlanks %i\n", vblanks);
		printf("Additional Packets %i\n", packets);
		printf("Packets Sent       %i\n", packets_sent);

		// decrement/increment vblanks
		printf("\x1b[8;4H---\x1b[8;25H+++");
		printf("\x1b[9;4H---\x1b[9;25H+++");
		printf("\x1b[10;4H---\x1b[10;25H+++");
		// decrement/increment packets
		printf("\x1b[12;4H---\x1b[12;25H+++");
		printf("\x1b[13;4H---\x1b[13;25H+++");
		printf("\x1b[14;4H---\x1b[14;25H+++");

		printf("\x1b[16;0H");
		print_header("NETWORK");
		printf_addr("IP         %u.%u.%u.%u\n", addr.s_addr);
		printf_addr("Gateway IP %u.%u.%u.%u\n", gateway_addr.s_addr);
		print_header("INPUTS");
		printf("Payload ");
		for (int i = 0; i < 8; i++) {
			printf("%02X", buf[i]);
		}

		// individual buttons
		printf("\x1b[21;0HSTART\x1b[21;6H%i SEL\x1b[21;14H%i L\x1b[21;22H%i R\x1b[21;30H%i",
				(keys & KEY_START) != 0, (keys & KEY_SELECT) != 0, (keys & KEY_L) != 0, (keys & KEY_R) != 0);
		printf("\x1b[22;0HUP\x1b[22;6H%i DOWN\x1b[22;14H%i LEFT\x1b[22;22H%i RIGHT\x1b[22;30H%i",
				(keys & KEY_UP) != 0, (keys & KEY_DOWN) != 0, (keys & KEY_LEFT) != 0, (keys & KEY_RIGHT) != 0);
		printf("\x1b[23;0HX\x1b[23;6H%i Y\x1b[23;14H%i B\x1b[23;22H%i A\x1b[23;30H%i",
				(keys & KEY_X) != 0, (keys & KEY_Y) != 0, (keys & KEY_B) != 0, (keys & KEY_A) != 0);
	}
}
