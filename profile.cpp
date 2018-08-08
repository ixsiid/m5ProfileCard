#include "profile.hpp"
#include <utility/qrcode.h>

#define PROFILE_QR_SCALE 1
#define PROFILE_QR_ECC ECC_HIGH

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

/*
class Profile {
	
		static void create(const char * name, const char * title);
		static void setIcon();
		static void setQR(const char * mail, const char * url, const char * phone);
		static void draw();
};
*/

static void Profile::create(const char *name, const char *title)
{
	this->created = false;

	uint8_t length = strlen(name);
	if (length > PROFILE_BUFFER_SIZE_NAME)
		length = PROFILE_BUFFER_SIZE_NAME;
	memcpy(this->name, name, length);
	this->name[length] = 0;

	uint8_t length = strlen(title);
	if (length > PROFILE_BUFFER_SIZE_TITLE)
		length = PROFILE_BUFFER_SIZE_TITLE;
	memcpy(this->title, title, length);
	this->title[length] = 0;
}

static void Profile::setIcon()
{
	// set icon
}

static void Profile::setQR(const char *mail, const char *url, const char *phone)
{
	this->created = true;

	uint8_t length = 8;

	uint8_t len_name = strlen(this->name);
	uint8_t len_mail = strlen(mail);
	uint8_t len_url = strlen(url);
	uint8_t len_phone = strlen(phone);

	if (this->name != 0)
		length += 3;
	if (len_mail > 0)
		length += 6;
	if (len_url != 0)
		length += 5;
	if (len_phone != 0)
		length += 5;

	char text[length + 10];
	uint8_t index = 0;
	memcpy(text, "MECARD:", index += 7);
	if (len_name > 0)
	{
		memcpy(&text[index], "N:", index += 2);
		memcpy(&text[index], profile->name, index += len_name);
		text[index++] = ';';
	}
	if (len_mail > 0)
	{
		memcpy(&text[index], "EMAIL:", index += 6);
		memcpy(&text[index], profile->mail, index += len_mail);
		text[index++] = ';';
	}
	if (len_url > 0)
	{
		memcpy(&text[index], "URL:", index += 4);
		memcpy(&text[index], profile->url, index += len_url);
		text[index++] = ';';
	}
	if (len_phone > 0)
	{
		memcpy(&text[index], "TEL:", index += 4);
		memcpy(&text[index], profile->phone, index += len_phone);
		text[index++] = ';';
	}

	text[index++] = ';';
	text[index++] = 0;

	QRCode code;
	qrcode_initText(&code, profileQRStream, PROFILE_QR_VERSION, PROFILE_QR_ECC, text);
}

static void Profile::drawQR(uint16_t offsetX, uint16_t offsetY)
{
	uint8_t dx = 0;
	uint16_t index = 0;
	int8_t bitPosition = 7;
	for (uint16_t i = 0; i < PROFILE_QR_PIXEL_NUM; i++)
	{
		uint8_t h = this->profileQRStream[index] & (1 << bitPosition);
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

const uint16_t back = 65535;
const uint16_t fore = 0;
static void draw()
{
	if (!this->created)
		this->setQR("", "", "");

	M5.Lcd.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, back);

	M5.Lcd.setTextColor(fore);
	M5.Lcd.setCursor(5, 3);
	M5.Lcd.setTextSize(4);
	M5.Lcd.print(this->title);
	M5.Lcd.setCursor(5, 64);
	M5.Lcd.setTextSize(3);
	M5.Lcd.print(this->name);

	this->drawQR(DISPLAY_WIDTH - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 10, DISPLAY_HEIGHT - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 10);

	M5.Lcd.fillCircle(80, DISPLAY_HEIGHT - 80, 64, fore);
}

//"MECARD:NOTE:TWITTER\:@ixsiid\nLINE\:https\://line.me//ti/p/PVW4ewVZtr;;"