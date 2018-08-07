#include "qrdraw.hpp"

const uint16_t _version[40] = {
    17,   32,   53,   78,  106,   134,  154,  192,  230,  271,
   321,  367,  425,  458,  520,   586,  644,  718,  792,  858,
   929, 1003, 1091, 1171, 1273,  1367, 1465, 1528, 1628, 1732,
  1840, 1952, 2068, 2188, 2303,  2431, 2563, 2699, 2809, 2953
};
QRCode * createTextQR(const char * data) {
  uint16_t length = strlen(data);
  if (length > 2953) {
    length = 2953;
    // data[2953] = 0;
  }
  uint8_t qr_version = 0;
  while (length > _version[qr_version++]);

  QRCode * code = (QRCode *)malloc(sizeof(QRCode));
  uint8_t * qrcodeBytes = (uint8_t *)malloc(qrcode_getBufferSize(qr_version));
  qrcode_initText(code, qrcodeBytes, qr_version, ECC_LOW, data);
  return code;
}

void drawQRcode(QRCode * code, uint16_t offsetX, uint16_t offsetY) {
  const uint16_t back = 65535;
  const uint16_t fore = 0;
  const uint8_t padding = 5;
  const uint8_t scale = 3;

  M5.Lcd.fillRect(offsetX, offsetY, scale*(code->size + 2 * padding), scale*(code->size + 2 * padding), back);
  for (uint8_t y = 0; y < code->size; y++) {
    for (uint8_t x = 0; x < code->size; x++) {
      if (qrcode_getModule(code, x, y)) {
        
        M5.Lcd.fillRect(offsetX+scale*(x+padding), offsetY+scale*(y+padding), scale, scale, fore);
      }
    }
  }
}

QRCode * drawTextQRcode(const char * text, uint16_t x, uint16_t y) {
  QRCode * code = createTextQR(text);
  drawQRcode(code, x, y);
  return code;
}

