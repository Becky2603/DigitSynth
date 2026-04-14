#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "types.h"


int main() {
    
    gpio::setupGpio();
    
    FlexSensor fs;
    fs.begin();
    
    getchar();
    
    gpio::teardownGpio();
    
    return 0;
}
