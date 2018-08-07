#include <M5Stack.h>

typedef struct SNS
{
	char *name;
	char *id;
} SNS;

typedef struct Profile
{
	// MECARD (in qr code)
	char *name;
	char *phone;
	char *mail;
	char *web;
	SNS *sns;
	uint8_t sns_count;

	// display
	uint16_t color;
	void *icon;
	char *title;
} Profile;

char *createProfileText(Profile *profile);