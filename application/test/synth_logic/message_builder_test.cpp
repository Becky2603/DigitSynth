#include <cassert>
#include "MessageBuilder.hpp"

int main() {
    MessageBuilder builder;

    //index 0: filter cutoff — single CC 74
    auto msgs = builder.buildMessages(0, 100);
    assert(msgs.size() == 1);
    assert(msgs[0].status == 0xB0);
    assert(msgs[0].data_1 == 74);
    assert(msgs[0].data_2 == 100);

    //index 1: LFO rate — NRPN sequence, LSB 6
    msgs = builder.buildMessages(1, 64);
    assert(msgs.size() == 4);
    assert(msgs[0].status == 0xB0);
    assert(msgs[0].data_1 == 99);
    assert(msgs[0].data_2 == 0);
    assert(msgs[1].status == 0xB0);
    assert(msgs[1].data_1 == 98);
    assert(msgs[1].data_2 == 6);
    assert(msgs[2].status == 0xB0);
    assert(msgs[2].data_1 == 6);
    assert(msgs[2].data_2 == 64);
    assert(msgs[3].status == 0xB0);
    assert(msgs[3].data_1 == 38);
    assert(msgs[3].data_2 == 0);

    // index 2: LFO filter depth — NRPN sequence, LSB 18
    msgs = builder.buildMessages(2, 80);
    assert(msgs.size() == 4);
    assert(msgs[0].status == 0xB0);
    assert(msgs[0].data_1 == 99);
    assert(msgs[0].data_2 == 0);
    assert(msgs[1].status == 0xB0);
    assert(msgs[1].data_1 == 98);
    assert(msgs[1].data_2 == 18);
    assert(msgs[2].status == 0xB0);
    assert(msgs[2].data_1 == 6);
    assert(msgs[2].data_2 == 80);
    assert(msgs[3].status == 0xB0);
    assert(msgs[3].data_1 == 38);
    assert(msgs[3].data_2 == 0);

    // index 3: pitch bend
    msgs = builder.buildMessages(3, 90);
    assert(msgs.size() == 1);
    assert(msgs[0].status == 0xE0);
    assert(msgs[0].data_1 == 0);
    assert(msgs[0].data_2 == 90);

    // invalid index: empty vector
    msgs = builder.buildMessages(99, 50);
    assert(msgs.size() == 0);

    return 0;
}
