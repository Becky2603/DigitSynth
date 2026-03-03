#include <iostream>
#include <vector>

#include "subscriber.h"
#include "publisher.h"

class SubscriberImplementation : public Subscriber<int> {
public:
    bool update(int data) {
        std::cout << data << std::endl;

        return false;
    }
};

class PublisherImplementation : public Publisher<int> {
public:
    bool registerSubscriber(Subscriber<int> subscriber) {
        this->subscribers.push_back(subscriber);
        return true;
    }

    PublisherImplementation(int value) {
        this->subscribers = std::vector<Subscriber<int>>();
        this->value = value;
    }

    void notifySubscribers() {
        for (auto &subscriber : this->subscribers) {
            subscriber.update(this->value);
        }
    }

private:
    std::vector<Subscriber<int>> subscribers;
    int value;
};

int test_single_subscriber() {
    auto publisher = PublisherImplementation(12);
    auto subscriber = SubscriberImplementation();

    publisher.registerSubscriber(subscriber);
    publisher.notifySubscribers();

    return 0;
}

int test_multiple_subscribers() {
    return 0;
}

int main() {
    test_single_subscriber();

    test_multiple_subscribers();

    return 0;
}
