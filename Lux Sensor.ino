// This #include statement was automatically added by the Particle IDE.
#include <tsl2561.h>

// declare a TSL2561 object with I2C address 0x39 (ADDR pin floating)
TSL2561 tsl(TSL2561_ADDR);

// Light sensor settings vars
uint16_t integrationTime;
bool autoGainOn = true;

// define a frequency of readings every x ms
int sensePeriod = 1000;
int senseTime;
double illuminance;

void setup() {
    Particle.subscribe("hook-response/LUX T", myHandler, MY_DEVICES);
    tsl.begin();
    tsl.setPowerUp();
    // x1 gain, 101ms integration time
    tsl.setTiming(false,1,integrationTime);

    // set illuminance as a cloud variable
    Particle.variable("illuminance", illuminance);
    // initialise a timer
    senseTime = millis();
}

void loop() {
    
  if ((millis() - senseTime) > sensePeriod) {
    // get raw data
    uint16_t _broadband, _ir;
    if (tsl.getData(_broadband,_ir,autoGainOn)){
      // now get the illuminance value in lux
      double lux = tsl.getLux(integrationTime,_broadband,_ir,illuminance);
      String data = String(illuminance);
     // Trigger the integration
      Particle.publish("LUX", data, PRIVATE);
     
      delay(10000);
    }
  }
}
  void myHandler(const char *event, const char *data) {
  // Handle the integration response
}

