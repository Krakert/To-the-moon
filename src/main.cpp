#include <displayHandler.h>
#include <wifi.h>
displayHandler display;

void setup(){
  Serial.begin(115200);
  display.init(1,3);
  display.boot();
  display.showInitScreenAp();
}

void loop(){

}
