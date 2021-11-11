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
#define X_CENTER            DISPLAY_X_MAX / 2
#define Y_CENTER            DISPLAY_Y_MAX / 2   

/* DEFINE VALUES TOUCHSCREEN */
#define TS_MINX 400
#define TS_MINY 300
#define TS_MAXX 3800
#define TS_MAXY 3600
#define TS_BOX_OFFSET 5

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

typedef struct {
    double rawXAxis[8];
    uint16_t  Yaxis[8];
    uint16_t  Xaxis[8];
    double   maxMin[2];
    double     percent;
} dataGraphStruct;

typedef struct {
    uint16_t  X;
    uint16_t  Y;
    uint16_t  H;
    uint16_t  W;
} sizeButtonStruct;


class displayHandler {
    
    
    public:
        Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
        XPT2046_Touchscreen ts = XPT2046_Touchscreen(TS_CS);

        void boot();
        void showInitScreenAp(); 

        const uint16_t displayXSize = DISPLAY_X_MAX;
        const uint16_t displayYSize = DISPLAY_Y_MAX;
        void init(uint8_t touchRotation = TOUCH_ROTATION, 
                  uint8_t screenRotation = DISPLAY_ROTATION, 
                  uint8_t textSize = STANDARD_TEXT_SIZE, 
                  uint16_t textColor = STANDARD_TEXT_COLOR);

        uint16_t * placeTextInCenter(const String &textBuf, 
                                     int16_t x, 
                                     int16_t y, 
                                     uint8_t textSize = STANDARD_TEXT_SIZE, 
                                     uint16_t textColor = STANDARD_TEXT_COLOR);

        void placeBoxtInCenter(int16_t x, 
                               int16_t y, 
                               int16_t w, 
                               int16_t h,
                               uint16_t color = STANDARD_TEXT_COLOR,
                               uint8_t fill = false);

        void drawTriangleButton(int16_t x0, int16_t y0, 
                                int16_t x1, int16_t y1, 
                                int16_t x2, int16_t y2, 
                                sizeButtonStruct *sizeButtonStruct,
                                uint16_t  color = ILI9341_BLACK);

        void drawRectButton(int16_t x, int16_t y, 
                            int16_t w, int16_t h, 
                            sizeButtonStruct *sizeButtonStruct,
                            uint16_t  color = ILI9341_BLACK);

        void drawGraph(dataGraphStruct *dataGraphStruct, 
                       uint16_t x, 
                       uint16_t y, 
                       uint16_t w, 
                       uint16 h, 
                       uint8_t offset);

        void clear(int color);
        void setToDefault();
        
        uint8_t checkIfPressed(sizeButtonStruct *sizeButtonStruct, uint16_t x, uint16_t y);

    private:
        uint8_t textSize = STANDARD_TEXT_SIZE;
        uint16_t textColor = STANDARD_TEXT_COLOR;
        void setTextSizeToDefault();
        void setTextColorToDefault();

        double mapDouble(double value, double in_min, double in_max, double out_min, double out_max);
        double percent(double a, double b);
};

#endif // DISPLAYHANDLER_H