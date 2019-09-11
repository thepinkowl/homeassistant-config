//
// Created by Roger Miret Giné on 2019-07-26.
//

#ifndef ESPHOME_NECACTION_H
#define ESPHOME_NECACTION_H


#include <esphome/components/remote_transmitter/remote_transmitter.h>
#include <esphome/components/remote_base/remote_base.h>

class NECAction {
public:
    remote_base::NECAction<> *remote_base_necaction;

    void init(remote_transmitter::RemoteTransmitterComponent *remote, uint16_t command) {
        remote_base_necaction = new remote_base::NECAction<>();
        remote_base_necaction->set_parent(remote);
        remote_base_necaction->set_address(0xFF);
        remote_base_necaction->set_command(command);
    }

    void play() {
        remote_base_necaction->play();
    }
};

#endif //ESPHOME_NECACTION_H
