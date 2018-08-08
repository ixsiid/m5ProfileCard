#include "profile.hpp"
#include <utility/qrcode.h>

#define PROFILE_QR_SCALE 1
#define PROFILE_QR_ECC ECC_LOW

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define PROFILE_GITHUB_SCALE 5

char Profile::Name[PROFILE_BUFFER_SIZE_NAME] = "";
char Profile::Title[PROFILE_BUFFER_SIZE_TITLE] = "";
char Profile::Github[PROFILE_BUFFER_SIZE_GITHUB] = "";
bool Profile::Created = false;
uint8_t Profile::QRBytes[PROFILE_QR_STREAM_SIZE] = "";
int32_t Profile::Identicon = 0;
IconCallback Profile::Icon = NULL;

ProfileColor Profile::Color = {
	65535, // back
	0,     // qr
	0,     // title
	0,     // name
	0      // github
};

void Profile::create(const char *name, const char *title)
{
	Created = false;

	uint8_t length = strlen(name);
	if (length > PROFILE_BUFFER_SIZE_NAME)
		length = PROFILE_BUFFER_SIZE_NAME;
	memcpy(Name, name, length);
	Name[length] = 0;

	length = strlen(title);
	if (length > PROFILE_BUFFER_SIZE_TITLE)
		length = PROFILE_BUFFER_SIZE_TITLE;
	memcpy(Title, title, length);
	Title[length] = 0;
}

void Profile::setIconCallback(IconCallback iconCallback)
{
	Icon = iconCallback;
}

void Profile::setGithub(const char *name, uint16_t color, int32_t identicon)
{
	uint8_t length = strlen(name);
	memcpy(Github, name, length);
	Color.github = color;
	Identicon = identicon | 1;
}

void Profile::setQR(const char *mail, const char *url, const char *phone, const char *note)
{
	Created = true;

	uint8_t length = 8;

	uint8_t len_name = strlen(Name);
	uint8_t len_mail = strlen(mail);
	uint8_t len_url = strlen(url);
	uint8_t len_phone = strlen(phone);
	uint8_t len_note = strlen(note);

	if (len_name > 0)
		length += 3;
	if (len_mail > 0)
		length += 6;
	if (len_url > 0)
		length += 5;
	if (len_phone > 0)
		length += 5;
	if (len_note > 0)
		length += 6;

	char text[length + 10];
	uint8_t index = 0;
	memcpy(text, "MECARD:", index += 7);
	if (len_name > 0)
	{
		memcpy(&text[index], "N:", 2);
		index += 2;
		memcpy(&text[index], Name, len_name);
		index += len_name;
		text[index++] = ';';
	}
	if (len_mail > 0)
	{
		memcpy(&text[index], "EMAIL:", 6);
		index += 6;
		memcpy(&text[index], mail, len_mail);
		index += len_mail;
		text[index++] = ';';
	}
	if (len_url > 0)
	{
		memcpy(&text[index], "URL:", 4);
		index += 4;
		memcpy(&text[index], url, len_url);
		index += len_url;
		text[index++] = ';';
	}
	if (len_phone > 0)
	{
		memcpy(&text[index], "TEL:", 4);
		index += 4;
		memcpy(&text[index], phone, len_phone);
		index += len_phone;
		text[index++] = ';';
	}
	if (len_note > 0)
	{
		memcpy(&text[index], "NOTE:", 5);
		index += 5;
		memcpy(&text[index], note, len_note);
		index += len_note;
		text[index++] = ';';
	}

	text[index++] = ';';
	text[index++] = 0;

	QRCode code;
	qrcode_initText(&code, QRBytes, PROFILE_QR_VERSION, PROFILE_QR_ECC, text);
}

void Profile::drawQR(uint16_t x, uint16_t y)
{
	uint8_t dx = 0;
	uint16_t index = 0;
	int8_t bitPosition = 7;
	for (uint16_t i = 0; i < PROFILE_QR_PIXEL_NUM; i++)
	{
		uint8_t h = QRBytes[index] & (1 << bitPosition);
		if (--bitPosition < 0)
		{
			bitPosition = 7;
			index++;
		}
		if (h > 0)
			M5.Lcd.fillRect(
				x + dx * PROFILE_QR_SCALE,
				y,
				PROFILE_QR_SCALE, PROFILE_QR_SCALE,
				Color.qr);
		if (++dx >= PROFILE_QR_SIZE)
		{
			dx = 0;
			y += PROFILE_QR_SCALE;
		}
	}
}

void Profile::drawGithub(uint16_t x, uint16_t y)
{
	int32_t identicon = Identicon;
	for (uint8_t dy = 0; dy < 5; dy++)
	{
		for (uint8_t dx = 0; dx < 5; dx++)
		{
			if (identicon < 0)
				M5.Lcd.fillRect(
					x + dx * PROFILE_GITHUB_SCALE,
					y + dy * PROFILE_GITHUB_SCALE,
					PROFILE_GITHUB_SCALE,
					PROFILE_GITHUB_SCALE,
					Color.github);
			
			identicon <<= 1;
		}
	}
}

void Profile::draw()
{
	if (!Created)
		Profile::setQR("", "", "", "");

	M5.Lcd.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, Color.back);

	M5.Lcd.setTextColor(Color.title);
	M5.Lcd.setCursor(5, 3);
	M5.Lcd.setTextSize(4);
	M5.Lcd.print(Title);

	M5.Lcd.setTextColor(Color.name);
	M5.Lcd.setCursor(5, 64);
	M5.Lcd.setTextSize(3);
	M5.Lcd.print(Name);

	Profile::drawQR(DISPLAY_WIDTH - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 10,
					DISPLAY_HEIGHT - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 10);

	if ((Identicon & 1) > 0)
	{
		Profile::drawGithub(
			DISPLAY_WIDTH - PROFILE_QR_SIZE * PROFILE_QR_SCALE - 20 - 5 * PROFILE_GITHUB_SCALE,
			DISPLAY_HEIGHT - 10 - 5 * PROFILE_GITHUB_SCALE);
	}

	//M5.Lcd.fillCircle(80, DISPLAY_HEIGHT - 80, 64, Color.qr);
	if (Icon != NULL) {
		Icon(&M5.Lcd, 80, DISPLAY_HEIGHT - 80, 128);
	}
}
