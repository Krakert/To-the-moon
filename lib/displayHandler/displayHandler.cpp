#include <displayHandler.h>

displayHandler::displayHandler(uint8_t tftCs, uint8_t tftDc, uint8_t tftRst, uint8_t tsCs, uint8_t displayRotation, uint8_t touchRotation){
    tft = Adafruit_ILI9341(tftCs, tftDc, tftRst);
    XPT2046_Touchscreen ts(TS_CS);
    ts.begin();
    ts.setRotation(touchRotation);
    tft.begin();
    tft.setRotation(displayRotation);
}