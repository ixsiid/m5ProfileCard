#ifndef M5STACK_H
#define M5STACK_H
#include <M5Stack.h>
#endif

#define PROFILE_QR_VERSION 8
#define PROFILE_QR_SIZE (4 * PROFILE_QR_VERSION + 17)
#define PROFILE_QR_PIXEL_NUM (PROFILE_QR_SIZE * PROFILE_QR_SIZE)
#define PROFILE_QR_STREAM_SIZE ((PROFILE_QR_PIXEL_NUM + 7) / 8)

#define PROFILE_BUFFER_SIZE_NAME 10
#define PROFILE_BUFFER_SIZE_TITLE 20
class Profile {
	public:
		static void create(const char * name, const char * title);
		static void setIcon();
		static void setQR(const char * mail, const char * url, const char * phone);
		static void draw();

	private:
		Profile();

		static char name[PROFILE_BUFFER_SIZE_NAME];
		static char title[PROFILE_BUFFER_SIZE_TITLE];
		static bool created;
		static uint8_t profileQRStream[PROFILE_QR_STREAM_SIZE];
		static void Profile::drawQR(uint16_t x, uint16_t y);
};
