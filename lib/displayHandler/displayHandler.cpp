#include <displayHandler.h>

/*
<---X axis--->
---------------
|             |  ^
|             |  | Y axis
|             |  
---------------
*/

/**************************************************************************/
/*!
  @brief    Set up the TFT display
  @param    touchRotation           uint8_t:  The rotation  of the touchscreen (x,Y position)
  @param    screenRotation          uint8_t:  The rotation  of the  text on the display (x,Y position)
  @param    textSize                uint8_t:  The default text size
  @param    textColor               uint16_t: The default color of the text
*/
/**************************************************************************/
void displayHandler::init(uint8_t touchRotation, uint8_t screenRotation, uint8_t textSize, uint16_t textColor){
    tft.begin();
    ts.begin();
    ts.setRotation(touchRotation);
    tft.setRotation(screenRotation);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(textSize);
    tft.setTextColor(textColor);
    displayHandler::textColor = textColor;
    displayHandler::textSize = textSize;
}

/**************************************************************************/
/*!
  @brief    Show the init screen when setting the AP (first setup)
*/
/**************************************************************************/
void displayHandler::showInitScreenAp(){
  tft.fillScreen(ILI9341_WHITE);
  tft.drawRect(X_OFFSET, Y_OFFSET, WIDTH, HEIGHT, ILI9341_BLACK);
  uint16_t i = 10;
  for (i = 10; i < 290; i = i + 10){
    tft.fillRect(X_OFFSET+1, Y_OFFSET+1, i-2, HEIGHT-2, ILI9341_GREEN);
    delay(10);
  }
  tft.fillRect(X_OFFSET, Y_OFFSET, i, HEIGHT, ILI9341_WHITE);
  placeTextInCenter("Setup is done", (DISPLAY_X_MAX / 2), Y_OFFSET, 3, ILI9341_BLUE);
  tft.drawLine(10, (Y_OFFSET + (HEIGHT / 2)), (DISPLAY_X_MAX - 10),(Y_OFFSET + (HEIGHT / 2)), ILI9341_BLACK);
}

/**************************************************************************/
/*!
  @brief    Place text in the center of a X an Y coordinate
  @param    textBuf                 const String: pointer to a String
  @param    x                       uint16_t:     The X coordinate
  @param    y                       uint16_t:     The Y coordinate
  @param    textSize                uint8_t:      The text size
  @param    textColor               uint16_t:     The color of the text
  @return   size                    uint16_t[0]:  The width of the text
                                    uint16_t[1]:  The height of the text
*/
/**************************************************************************/
uint16_t * displayHandler::placeTextInCenter(const String &textBuf, uint16_t x, uint16_t y, uint8_t textSize, uint16_t textColor){
    int16_t x1, y1;
    static uint16_t size[2] = {0, 0};
    tft.setTextSize(textSize);
    tft.setTextColor(textColor);
    tft.getTextBounds(textBuf, 0, 0, &x1, &y1, &size[0], &size[1]); //calc width of new string
    tft.setCursor(x - size[0] / 2, y - size[1] / 2);
    tft.print(textBuf);
    return size;
}

/**************************************************************************/
/*!
  @brief    Place a rectangular in the center of a X an Y coordinate
  @param    x                       uint16_t: X coordinate of the center of the rectangle
  @param    Y                       uint16_t: Y coordinate of the center of the rectangle
  @param    w                       uint16_t: Width of the rectangle
  @param    h                       uint16_t: Height of the rectangle
  @param    color                   uint16_t: Color of the rectangle
*/
/**************************************************************************/
void displayHandler::placeBoxtInCenter(uint16_t x,  uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    tft.drawRect((x - (w / 2)), (y - (h / 2)), w, h , color);
}

/**************************************************************************/
/*!
  @brief    Restore text size to default state
*/
/**************************************************************************/
void displayHandler::setTextSizeToDefault(){
    tft.setTextSize(displayHandler::textSize);
}

/**************************************************************************/
/*!
  @brief    Restore text color to default state
*/
/**************************************************************************/
void displayHandler::setTextColorToDefault(){
    tft.setTextColor(displayHandler::textColor);
}

/**************************************************************************/
/*!
  @brief    Restore both text size and text color to default state
*/
/**************************************************************************/
void displayHandler::setToDefault(){
    tft.setTextSize(displayHandler::textSize);
    tft.setTextColor(displayHandler::textColor);
}

