#include "qrdraw.hpp"
#include "profile.hpp"

void setup()
{
	// put your setup code here, to run once:

	M5.begin();

	// LCD display
	M5.Lcd.print("Hello, World!!");

	M5.setWakeupButton(BUTTON_A_PIN);
	M5.setWakeupButton(BUTTON_B_PIN);
	M5.setWakeupButton(BUTTON_C_PIN);

/*
	Profile p;
	p.name = (char *)malloc(7);
	memcpy(p.name, "ixsiid", 7);
	p.mail = (char *)malloc(19);
	memcpy(p.mail, "ixsiid@halzion.net", 19);
	SNS sns[2];
	sns[0].name = (char *)malloc(8);
	memcpy(sns[0].name, "TWITTER", 8);
	sns[0].id = (char *)malloc(8);
	memcpy(sns[0].id, "@ixsiid", 8);
	p.sns = sns;
	p.sns_count = 1;

	char *text = createProfileText(&p);
	drawTextQRcode(text, 75, 3);
	*/

	drawTextQRcode("MECARD:N:IXSIID;;", 75, 3);
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
