#include <vector>
#include <thread>

#include "types.h"

class AdcDriver {
public:
    AdcDriver();
    void registerCallback(AdcCallback);
    void beginContinuous();
    void cancel();

private:
    std::vector<AdcCallback> callbacks;
    std::thread continuousConversion;
    bool conversionActive = false;

    void notifySubscribers(AdcData);
};

enum Ads1256Commands {
    WAKEUP   = 0x00,    // n.b. wakeup can also be 0xff
    RDATA    = 0x01,
    RDATAC   = 0x03,
    SDATAC   = 0x0f,
    RREG     = 0x10,    // n.b. OR with register number 0000 rrrr
    WREG     = 0x50,    // n.b. OR with register number 0000 rrrr
    SELFCAL  = 0xf0,
    SELFOCAL = 0xf1,
    SELFGCAL = 0xf2,
    SYSOCAL  = 0xf3,
    SYSGCAL  = 0xf4,
    SYNC     = 0xfc,
    STANDBY  = 0xfd,
    RESET    = 0xfe,
};
