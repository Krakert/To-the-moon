#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

/* PARAMETERS STANDARD TFT AND TS*/ 
#define TFT_CS              D0 
#define TFT_DC              D8  
#define TS_CS               D3 
#define TFT_RST             -1

#define DISPLAY_ROTATION    3
#define TOUCH_ROTATION      1

class displayHandler {
    public:
        displayHandler(uint8_t tftCs = TFT_CS, 
                       uint8_t tftDc = TFT_DC, 
                       uint8_t tftRst = TFT_RST, 
                       uint8_t tsCs = TS_CS,
                       uint8_t displayRotation = DISPLAY_ROTATION,
                       uint8_t touchRotation = TOUCH_ROTATION);
        void showInitScreen();
    private:    
        Adafruit_ILI9341 tft;
        XPT2046_Touchscreen ts;
};

#endif // DISPLAYHANDLER_H
