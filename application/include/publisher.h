#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include "subscriber.h"

template<typename T> class Publisher {
public:
    virtual bool registerSubscriber(Subscriber<T> subscriber);
};


#endif
