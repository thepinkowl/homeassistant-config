//
// Created by Roger Miret Gine on 2019-07-26.
//

#ifndef ESPHOME_VIVAX_IR_CLIMATE_H
#define ESPHOME_VIVAX_IR_CLIMATE_H

#include "esphome.h"
#include <esphome/components/climate/climate.h>
#include <esphome/components/climate/climate_mode.h>
#include <esphome/components/climate/climate_traits.h>
#include <esphome/components/remote_transmitter/remote_transmitter.h>
#include <esphome/components/remote_base/remote_base.h>
#include <SamsungAction.h>

class VivaxInfraredClimate : public Component, public Climate {
public:
    remote_transmitter::RemoteTransmitterComponent *remote_transmitter_remotetransmittercomponent;

    VivaxInfraredClimate(remote_transmitter::RemoteTransmitterComponent *rt) {
        remote_transmitter_remotetransmittercomponent = rt;
    }

    void setup() override {
        // This will be called by App.setup()
    }

    ClimateTraits traits() override {
        auto traits = ClimateTraits();
        traits.set_supports_current_temperature(false);
        traits.set_supports_two_point_target_temperature(false);
        traits.set_supports_auto_mode(false);
        traits.set_supports_cool_mode(true);
        traits.set_supports_heat_mode(false);
        traits.set_supports_away(false);
        traits.set_visual_min_temperature(17.0f);
        traits.set_visual_max_temperature(30.0f);
        traits.set_visual_temperature_step(1.0f);
        return traits;
    }

    void control(const ClimateCall &call) override {
        SamsungAction action = SamsungAction();

        uint32_t command = 0xA10840FF;
        //                     |  |
        //                     |  --- temperature
        //                     ------ off = 0 // on = 8

        if (call.get_mode().has_value()) {
            // User requested mode change
            ClimateMode mode = *call.get_mode();
            // Send mode to hardware
            // ...
            ESP_LOGD("climate", "Climate MODE");
            ESP_LOGD("climate", "%d", (uint16_t) mode);

            float temp = *call.get_target_temperature();
            ESP_LOGD("climate = debug", "Temp was set to %f", temp);


            if (mode == CLIMATE_MODE_COOL) {
                command += 0x800000;
            }

            // Publish updated state
            this->mode = mode;
            this->publish_state();
        }
        if (call.get_target_temperature().has_value()) {
            // User requested target temperature change
            float temp = *call.get_target_temperature();
            command += 0x800000;

            // Send target temp to climate
            if (temp >= 17.0f && temp <= 30.0f) {
                temp -= 17.0f;
                command += (int)temp * 0x100;
                ESP_LOGD("climate", "Temp set to %f", temp);
                ESP_LOGD("climate - debug", "Command %X", command);
            }
        }
        action.init(remote_transmitter_remotetransmittercomponent, command);
        action.play();

    }
};

#endif //ESPHOME_VIVAX_IR_CLIMATE_H
