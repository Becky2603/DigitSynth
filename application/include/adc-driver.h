#include "callback-types.h"

#include <vector>


class AdcDriver {
public:
    AdcDriver();
    void registerCallback(AdcCallback);
    void begin();
    void cancel();

private:
    std::vector<AdcCallback> callbacks;
    bool active = false;

    void notifySubscribers(AdcData);
};
