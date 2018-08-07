#include "profile.hpp"
#include <utility/qrcode.h>

#define PROFILE_QR_VERSION 8
#define PROFILE_QR_SIZE (4 * PROFILE_QR_VERSION + 17)
#define PROFILE_QR_PIXEL_NUM (PROFILE_QR_SIZE * PROFILE_QR_SIZE)
#define PROFILE_QR_STREAM_SIZE ((PROFILE_QR_PIXEL_NUM + 7) / 8)
#define PROFILE_QR_SCALE 1
#define PROFILE_QR_ECC ECC_HIGH

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// Prototype
char *createProfileText(Profile *profile);
void createQRStream(const char *text);
void drawQRStream(uint16_t offsetX, uint16_t offsetY);

char title[32];
char name[16];
uint8_t profileQRStream[PROFILE_QR_STREAM_SIZE];
const uint16_t back = 65535;
const uint16_t fore = 0;

void drawProfile()
{
	M5.Lcd.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, back);

	M5.Lcd.setTextColor(fore);
	M5.Lcd.setCursor(5, 3);
	M5.Lcd.setTextSize(4);
	M5.Lcd.print(title);
	M5.Lcd.setCursor(5, 64);
	M5.Lcd.setTextSize(3);
	M5.Lcd.print(name);

	drawQRStream(DISPLAY_WIDTH - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 10, DISPLAY_HEIGHT - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 10);

	M5.Lcd.fillCircle(80, DISPLAY_HEIGHT - 80, 64, fore);
}

void createProfile(Profile *profile)
{
	createQRStream(createProfileText(profile));
	memcpy(title, profile->title, 32 - 1);
	title[32 - 1] = 0;
	memcpy(name, profile->name, 16 - 1);
	name[16 - 1] = 0;
}

void createQRStream(const char *text)
{
	QRCode code;
	qrcode_initText(&code, profileQRStream, PROFILE_QR_VERSION, PROFILE_QR_ECC, text);
}

void drawQRStream(uint16_t x, uint16_t y)
{
	uint8_t dx = 0;
	uint16_t index = 0;
	int8_t bitPosition = 7;
	for (uint16_t i = 0; i < PROFILE_QR_PIXEL_NUM; i++)
	{
		uint8_t h = profileQRStream[index] & (1 << bitPosition);
		if (--bitPosition < 0)
		{
			bitPosition = 7;
			index++;
		}
		if (h > 0)
			M5.Lcd.fillRect(x + dx * PROFILE_QR_SCALE, y, PROFILE_QR_SCALE, PROFILE_QR_SCALE, fore);
		if (++dx >= PROFILE_QR_SIZE)
		{
			dx = 0;
			y += PROFILE_QR_SCALE;
		}
	}
}

char *createProfileText(Profile *profile)
{
	uint8_t length = 8;

	uint8_t len_mail = strlen(profile->mail);
	uint8_t len_name = strlen(profile->name);
	uint8_t len_phone = strlen(profile->phone);
	uint8_t len_web = strlen(profile->web);
	//uint8_t len_sns = 0;

	if (profile->mail != 0)
		length += 6;
	if (profile->name != 0)
		length += 3;
	if (profile->phone != 0)
		length += 5;
	if (profile->web != 0)
		length += 5;
	/*
	if (profile->sns_count > 0)
		length += 6 + profile->sns_count - 1;
	for (uint8_t i = 0; i < profile->sns_count; i++)
	{
		len_sns += strlen(profile->sns[i].name) + strlen(profile->sns[i].id) + 3;
	}
	*/

	char *text = (char *)malloc(length + 10);
	uint8_t index = 0;
	memcpy(text, "MECARD:", index += 7);
	if (len_mail > 0)
	{
		memcpy(&text[index], "EMAIL:", index += 6);
		memcpy(&text[index], profile->mail, index += len_mail);
		text[index++] = ';';
	}
	if (len_name > 0)
	{
		memcpy(&text[index], "N:", index += 2);
		memcpy(&text[index], profile->name, index += len_name);
		text[index++] = ';';
	}
	if (len_phone > 0)
	{
		memcpy(&text[index], "TEL:", index += 4);
		memcpy(&text[index], profile->phone, index += len_phone);
		text[index++] = ';';
	}
	if (len_web > 0)
	{
		memcpy(&text[index], "URL:", index += 4);
		memcpy(&text[index], profile->web, index += len_web);
		text[index++] = ';';
	}
	/*
	if (profile->sns_count > 0)
	{
		memcpy(&text[index], "NOTE:", index += 5);
		for (uint8_t i = 0; i < profile->sns_count; i++)
		{
			memcpy(&text[index], profile->sns[i].name, index += strlen(profile->sns[i].name));
			text[index++] = '\\';
			text[index++] = ':';
			memcpy(&text[index], profile->sns[i].id, index += strlen(profile->sns[i].id));
			text[index++] = '\n';
		}
		text[index - 1] = ';';
	}
	*/
	text[index++] = ';';
	text[index++] = 0;

	return text;
}

//"MECARD:NOTE:TWITTER\:@ixsiid\nLINE\:https\://line.me//ti/p/PVW4ewVZtr;;"