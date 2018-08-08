#include "std_arduino.hpp"
#include "profile.hpp"

void profileIcon(M5Display *d, int cx, int cy, int size)
{
	uint16_t back = 0b0100101011100011;
	uint16_t fore = 0b1111000110110110;
	
	uint16_t r = 16;
	uint16_t t = r * 19 / 11 + 1;
	uint16_t p = r * 5 / 3;

	d->fillCircle(cx, cy, size / 2, back);
	
	d->fillCircle(cx - t, cy - t / 2 - 1, r, fore);
	d->fillCircle(cx + t, cy - t / 2 - 1, r, fore);
	d->fillCircle(cx, cy + 2 * r, r, fore);

	d->fillCircle(cx, cy - 2 * r, p, back);
	d->fillCircle(cx, cy - 2 * r, r, fore);
	d->fillCircle(cx - t, cy + t / 2, p, back);
	d->fillCircle(cx - t, cy + t / 2, r, fore);
	d->fillCircle(cx + t, cy + t / 2, p, back);
	d->fillCircle(cx + t, cy + t / 2, r, fore);

	d->fillCircle(cx, cy, p, back);
	d->fillCircle(cx, cy, r, fore);
}

void setup()
{
	// put your setup code here, to run once:

	M5.begin();

	// LCD display
	M5.Lcd.print("Hello, World!!");

	M5.setWakeupButton(BUTTON_A_PIN);
	M5.setWakeupButton(BUTTON_B_PIN);
	M5.setWakeupButton(BUTTON_C_PIN);

	Profile::create("ixsiid", "HALZION.NET");
	Profile::setQR("ixsiid@halzion.net", "", "", "TWITTER:@ixsiid\nGithub:ixsiid");
	Profile::setGithub("ixsiid", 0b1101001111111001, 0b10001110110101000100100010000000);
	Profile::setIconCallback(profileIcon);
	Profile::draw();
}

bool turn = true;
void loop()
{
	// put your main code here, to run repeatedly:

	if (M5.BtnA.wasPressed())
	{
		M5.Lcd.writecommand(ILI9341_DISPOFF);
		M5.Lcd.setBrightness(0);
		turn = false;
	}
	else if (M5.BtnB.wasPressed())
	{
		M5.Lcd.writecommand(ILI9341_DISPON);
		M5.Lcd.setBrightness(100);
		turn = true;
	}
	else if (M5.BtnC.wasPressed())
	{
		M5.Lcd.writecommand((turn = !turn) ? ILI9341_DISPON : ILI9341_DISPOFF);
		M5.Lcd.setBrightness(turn ? 100 : 0);
	}

	M5.update();
}
