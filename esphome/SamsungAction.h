//
// Created by Roger Miret Giné on 2019-07-26.
//

#ifndef ESPHOME_SAMSUNGACTION_H
#define ESPHOME_SAMSUNGACTION_H

#include <esphome/components/remote_transmitter/remote_transmitter.h>
#include <esphome/components/remote_base/remote_base.h>

class SamsungAction {
public:
    remote_base::SamsungAction<> *remote_base_action;

    void init(remote_transmitter::RemoteTransmitterComponent *remote, uint32_t data) {
        remote_base_action = new remote_base::SamsungAction<>();
        remote_base_action->set_parent(remote);
        remote_base_action->set_data(data);
    }

    void play() {
        remote_base_action->play();
    }
};

#endif //ESPHOME_SAMSUNGACTION_H
