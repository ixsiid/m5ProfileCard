#ifndef M5STACK_H
#define M5STACK_H
#include <M5Stack.h>
#endif

#ifndef PROFILE_H
#define PROFILE_H

#define PROFILE_QR_VERSION 10
#define PROFILE_QR_SIZE (4 * PROFILE_QR_VERSION + 17)
#define PROFILE_QR_PIXEL_NUM (PROFILE_QR_SIZE * PROFILE_QR_SIZE)
#define PROFILE_QR_STREAM_SIZE ((PROFILE_QR_PIXEL_NUM + 7) / 8)

typedef struct ProfileColor {
	uint16_t back;
	uint16_t qr;
	uint16_t title;
	uint16_t name;
	uint16_t github;
} ProfileColor;

typedef void (* IconCallback)(M5Display * display, int cx, int cy, int size);

#define PROFILE_BUFFER_SIZE_NAME 10
#define PROFILE_BUFFER_SIZE_TITLE 20
#define PROFILE_BUFFER_SIZE_GITHUB 10
class Profile {
	public:
		static void create(const char * name, const char * title);
		static void setIconCallback(IconCallback callback);
		static void setGithub(const char * name, uint16_t color, int32_t identicon);
		static void setQR(const char * mail, const char * url, const char * phone, const char * note);
		static void draw();

	protected:
		static char Name[PROFILE_BUFFER_SIZE_NAME];
		static char Title[PROFILE_BUFFER_SIZE_TITLE];
		static bool Created;
		static uint8_t QRBytes[PROFILE_QR_STREAM_SIZE];
		static void drawQR(uint16_t x, uint16_t y);
		static IconCallback Icon;
		static ProfileColor Color;
		
		static void drawGithub(uint16_t x, uint16_t y);
		static char Github[PROFILE_BUFFER_SIZE_GITHUB];
		static int32_t Identicon;

	private:
		Profile();
};

#endif
