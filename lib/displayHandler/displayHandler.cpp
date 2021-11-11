#include <displayHandler.h>
#include <wifiHandler.h>
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
  @brief    Show a simple progress bar on the display
*/
/**************************************************************************/
void displayHandler::boot(){
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(X_OFFSET, Y_OFFSET, WIDTH, HEIGHT, ILI9341_BLACK);
  placeTextInCenter("Loading...", X_CENTER, Y_CENTER, 3, ILI9341_BLUE);
  uint16_t i = 10;
  for (i = 10; i < 290; i = i + 10){
    tft.fillRect(X_OFFSET+1, Y_OFFSET+1, i-2, HEIGHT-2, ILI9341_GREEN);
    delay(120);
  }
}

/**************************************************************************/
/*!
  @brief    Show the init screen when the AP is setup (first setup)
*/
/**************************************************************************/
void displayHandler::showInitScreenAp(){
  tft.fillScreen(ILI9341_BLACK);
  // tft.fillRect(X_OFFSET, Y_OFFSET, 290, Y_CENTER + 20, ILI9341_WHITE);
  placeTextInCenter("Setup the device", (X_CENTER), Y_OFFSET, 3, ILI9341_BLUE);
  tft.drawLine(10, (Y_OFFSET + (HEIGHT / 2)), (DISPLAY_X_MAX - 10),(Y_OFFSET + (HEIGHT / 2)), ILI9341_WHITE);
  placeTextInCenter("Connect to wifi point:", X_CENTER, 80, 2, ILI9341_WHITE);
  placeTextInCenter(WIFI_AP_STANDARD_SSID, X_CENTER, 105, 2, ILI9341_BLUE);
  String text = "Using password:" + String(WIFI_AP_STANDARD_PSW);
  placeTextInCenter(text, X_CENTER, 130, 2, ILI9341_WHITE);
  text = "Go to:" + String(WIFI_AP_STANDARD_IP[0]) + "." 
                  + String(WIFI_AP_STANDARD_IP[1]) + "." 
                  + String(WIFI_AP_STANDARD_IP[2]) + "." 
                  + String(WIFI_AP_STANDARD_IP[3]);
  placeTextInCenter(text, X_CENTER, 155, 2, ILI9341_WHITE);
  placeTextInCenter("Here enter the data to", X_CENTER, 180, 2, ILI9341_WHITE);
  placeTextInCenter("Connect to your own wifi", X_CENTER, 205, 2, ILI9341_WHITE);
}

/**************************************************************************/
/*!
  @brief    Place text in the center of a X an Y coordinate
  @param    textBuf                 const String: pointer to a String
  @param    x                       int16_t:      The X coordinate
  @param    y                       int16_t:      The Y coordinate
  @param    textSize                uint8_t:      The text size
  @param    textColor               uint16_t:     The color of the text
  @return   size                    uint16_t[0]:  The width of the text
                                    uint16_t[1]:  The height of the text
*/
/**************************************************************************/
uint16_t * displayHandler::placeTextInCenter(const String &textBuf, int16_t x, int16_t y, uint8_t textSize, uint16_t textColor){
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
  @param    x                       int16_t:  X coordinate of the center of the rectangle
  @param    Y                       int16_t:  Y coordinate of the center of the rectangle
  @param    w                       int16_t:  Width of the rectangle
  @param    h                       int16_t:  Height of the rectangle
  @param    color                   uint16_t: Color of the rectangle
  @param    fill                    uint8_8:  fill if true, else not
*/
/**************************************************************************/
void displayHandler::placeBoxtInCenter(int16_t x,  int16_t y, int16_t w, int16_t h, uint16_t color, uint8_t fill){
  if (fill){
    tft.fillRect((x - (w / 2)), (y - (h / 2)), w, h , color);
  } else {
    tft.drawRect((x - (w / 2)), (y - (h / 2)), w, h , color);
  }
}

/**************************************************************************/
/*!
  @brief    Place a triangle and save a box that represents the touch area
  @param    x0                      int16_t: Vertex #0 y coordinate
  @param    Y0                      int16_t: Vertex #1 x coordinate
  @param    x1                      int16_t: Vertex #1 x coordinate
  @param    y1                      int16_t: Vertex #1 y coordinate
  @param    X2                      int16_t: Vertex #2 x coordinate
  @param    y2                      int16_t: Vertex #2 y coordinate
  @param    sizeButtonStruct        sizeButtonStruct: Pointer to struct to save data
  @param    color                   uint16_t: Color of the rectangle
*/
/**************************************************************************/
void displayHandler::drawTriangleButton(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, sizeButtonStruct *sizeButtonStruct, uint16_t  color){
  tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
  uint16_t width, height;
  height = y1 - y2;
  if (x0 < x1 ){
    width = x1 - x0; 
    sizeButtonStruct->X = x0 - TS_BOX_OFFSET;
    sizeButtonStruct->Y = y0 - ((height / 2) + TS_BOX_OFFSET);
    sizeButtonStruct->W = width + (TS_BOX_OFFSET * 2);
    sizeButtonStruct->H = height + (TS_BOX_OFFSET * 2);
    // offset the x0 point from the right center to top left
  } else {
    width = x0 - x1;
    sizeButtonStruct->X = x0 - (TS_BOX_OFFSET + width);
    sizeButtonStruct->Y = y0 - (height / 2) - TS_BOX_OFFSET;
    sizeButtonStruct->W = (width +  (TS_BOX_OFFSET * 2));
    sizeButtonStruct->H = height + (TS_BOX_OFFSET * 2);
  }
}
 
/**************************************************************************/
/*!
  @brief    Place a rectangle and save a box that represents the touch area
  @param    x0                      int16_t: Vertex #0 y coordinate
  @param    Y0                      int16_t: Vertex #1 x coordinate
  @param    w                       int16_t: Width of the rectangle
  @param    h                       int16_t: Height of the rectangle
  @param    sizeButtonStruct        sizeButtonStruct: Pointer to struct to save data
  @param    color                   uint16_t: Color of the rectangle
*/
/**************************************************************************/
void displayHandler::drawRectButton(int16_t x, int16_t y, int16_t w, int16_t h, sizeButtonStruct *sizeButtonStruct, uint16_t  color){
  tft.fillRect(x, y, w, h, color);
  sizeButtonStruct->X = x + TS_BOX_OFFSET;
  sizeButtonStruct->Y = y + TS_BOX_OFFSET;
  sizeButtonStruct->W = w - (TS_BOX_OFFSET * 2);
  sizeButtonStruct->H = h - (TS_BOX_OFFSET * 2);
  tft.fillRect(sizeButtonStruct->X, sizeButtonStruct->Y, sizeButtonStruct->W, sizeButtonStruct->H, color);
}

/**************************************************************************/
/*!
  @brief    Place a rectangle and save a box that represents the touch area
  @param    x0                      int16_t: Vertex #0 y coordinate
  @param    Y0                      int16_t: Vertex #1 x coordinate
  @param    w                       int16_t: Width of the rectangle
  @param    h                       int16_t: Height of the rectangle
  @param    sizeButtonStruct        sizeButtonStruct: Pointer to struct to save data
  @param    color                   uint16_t: Color of the rectangle
*/
/**************************************************************************/
void displayHandler::drawGraph(dataGraphStruct *dataGraphStruct, uint16_t x, uint16_t y, uint16_t w, uint16 h, uint8_t offset){

  dataGraphStruct->maxMin[0] = dataGraphStruct->rawXAxis[0];
  for (int i = 0; i < DATAPOINTS_GRAPH; i++) {
    if (dataGraphStruct->maxMin[0] < dataGraphStruct->rawXAxis[i])
      dataGraphStruct->maxMin[0] = dataGraphStruct->rawXAxis[i];
  }
  dataGraphStruct->maxMin[1] = dataGraphStruct->rawXAxis[0];
  for (int i = 0; i < DATAPOINTS_GRAPH; i++) {
    if (dataGraphStruct->maxMin[1] > dataGraphStruct->rawXAxis[i])
      dataGraphStruct->maxMin[1] = dataGraphStruct->rawXAxis[i];
  }

  dataGraphStruct->procent = percent(dataGraphStruct->rawXAxis[7], dataGraphStruct->rawXAxis[0]);

  for (int i = 0; i < DATAPOINTS_GRAPH; i++) {
    dataGraphStruct->Yaxis[i] = mapDouble(dataGraphStruct->rawXAxis[i], dataGraphStruct->maxMin[1], dataGraphStruct->maxMin[0], offset, h - offset);
  }

  uint8_t stepSize = w / (DATAPOINTS_GRAPH - 1);
  for (int i = 0; i < DATAPOINTS_GRAPH; i++) {
    if (i == 0){
      dataGraphStruct->Xaxis[i] = x;
    } else {
      dataGraphStruct->Xaxis[i] = (x + (i * stepSize));
    }
  }

  for (int i = 0; i < DATAPOINTS_GRAPH - 1; i++) {
    if (dataGraphStruct->Yaxis[i] < dataGraphStruct->Yaxis[(i + 1)]){
      tft.drawLine(dataGraphStruct->Xaxis[i], y - dataGraphStruct->Yaxis[i],dataGraphStruct->Xaxis[(i + 1)], y - dataGraphStruct->Yaxis[(i + 1)], ILI9341_GREEN);
    } else {
      tft.drawLine(dataGraphStruct->Xaxis[i], y - dataGraphStruct->Yaxis[i],dataGraphStruct->Xaxis[(i + 1)], y - dataGraphStruct->Yaxis[(i + 1)], ILI9341_RED);
    }
  }
  tft.drawLine(x,y,x,y-h,ILI9341_WHITE);
  tft.drawLine(x,y,x+w,y,ILI9341_WHITE);
}

void displayHandler::clear(int color){
  tft.fillScreen(color);
}

uint8_t displayHandler::checkIfPressed(sizeButtonStruct *sizeButtonStruct, uint16_t x, uint16_t y){
  int16_t x1,y1;
  x1 = sizeButtonStruct->X + sizeButtonStruct->W;
  y1 = sizeButtonStruct->Y + sizeButtonStruct->H;
  if ((sizeButtonStruct->X <= x && x <= x1) && (sizeButtonStruct->Y <= y && y <= y1)){
    return 1;
  }
  return 0;
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

/**************************************************************************/
/*!
  @brief    Remade version of the map function to return a double instead of a int
*/
/**************************************************************************/
double displayHandler::mapDouble(double value, double in_min, double in_max, double out_min, double out_max){
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double displayHandler::percent(double a, double b){
    double result = 0;
    return result = ((b - a) * 100) / a;
}