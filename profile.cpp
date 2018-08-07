#include "profile.hpp"

char *createProfileText(Profile *profile)
{
	uint8_t length = 8;

	uint8_t len_mail = strlen(profile->mail);
	uint8_t len_name = strlen(profile->name);
	uint8_t len_phone = strlen(profile->phone);
	uint8_t len_web = strlen(profile->web);
	uint8_t len_sns = 0;
	if (profile->mail != 0)
		length += 6;
	if (profile->name != 0)
		length += 3;
	if (profile->phone != 0)
		length += 5;
	if (profile->web != 0)
		length += 5;
	if (profile->sns_count > 0)
		length += 6 + profile->sns_count - 1;
	for (uint8_t i = 0; i < profile->sns_count; i++)
	{
		len_sns += strlen(profile->sns[i].name) + strlen(profile->sns[i].id) + 3;
	}

	char *text = (char *)malloc(length);
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
	text[index++] = ';';
	text[index++] = 0;

	return text;
}

//"MECARD:NOTE:TWITTER\:@ixsiid\nLINE\:https\://line.me//ti/p/PVW4ewVZtr;;"