#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

template<typename T>
class Subscriber {
public:
    virtual bool update(T data);
};

template class Subscriber<int>;

#endif
