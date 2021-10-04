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

/* DEFINE MAX SCREEN SIZE */
#define DISPLAY_X_MAX       320 
#define DISPLAY_Y_MAX       240 

/* SETUP STANDAARD COLOR AND SIZE */
#define STANDARD_TEXT_SIZE  2
#define STANDARD_TEXT_COLOR ILI9341_WHITE

/* DEFINE SIZE OF THE PROGRESS BAR */
#define X_OFFSET            20
#define Y_OFFSET            40
#define WIDTH               280
#define HEIGHT              50
#define BAR_CENTER_X        X_OFFSET + (WIDTH / 2)
#define BAR_CENTER_Y        Y_OFFSET + (HEIGHT / 2)


class displayHandler {
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
    XPT2046_Touchscreen ts = XPT2046_Touchscreen(TS_CS);
    public:
        void showInitScreenAp(); 

        const uint16_t displayXSize = DISPLAY_X_MAX;
        const uint16_t displayYSize = DISPLAY_Y_MAX;
        void init(uint8_t touchRotation = TOUCH_ROTATION, 
                  uint8_t screenRotation = DISPLAY_ROTATION, 
                  uint8_t textSize = STANDARD_TEXT_SIZE, 
                  uint16_t textColor = STANDARD_TEXT_COLOR);

        uint16_t * placeTextInCenter(const String &textBuf, 
                                     uint16_t x, 
                                     uint16_t y, 
                                     uint8_t textSize = STANDARD_TEXT_SIZE, 
                                     uint16_t textColor = STANDARD_TEXT_COLOR);

        void placeBoxtInCenter(uint16_t x, 
                               uint16_t y, 
                               uint16_t w, 
                               uint16_t h,
                               uint16_t color = STANDARD_TEXT_COLOR);

    private:
        uint8_t textSize = STANDARD_TEXT_SIZE;
        uint16_t textColor = STANDARD_TEXT_COLOR;
        void setTextSizeToDefault();
        void setTextColorToDefault();
        void setToDefault();
};

#endif // DISPLAYHANDLER_H