//
// Created by Roger Miret Giné on 2019-07-26.
//

#ifndef ESPHOME_RGB_IR_LIGHT_H
#define ESPHOME_RGB_IR_LIGHT_H


#include "esphome.h"
#include <esphome/components/remote_transmitter/remote_transmitter.h>
#include <esphome/components/remote_base/remote_base.h>
#include <NECAction.h>

class RGBInfraredLightOutput : public Component, public LightOutput {
public:

    remote_transmitter::RemoteTransmitterComponent *remote_transmitter_remotetransmittercomponent;
    NECAction actions[15];

    RGBInfraredLightOutput(remote_transmitter::RemoteTransmitterComponent *rt) {
        remote_transmitter_remotetransmittercomponent = rt;
    }

    float Min(float a, float b) {
        return a <= b ? a : b;
    }

    float Max(float a, float b) {
        return a >= b ? a : b;
    }

    void setup() override {

        uint16_t buttons[16] = {
                0x906F, // 0  RED
                0xB04F, // 1  Orange+
                0xA857, // 2  Orange
                0x9867, // 3  Orange-
                0x8877, // 4  Yellow
                0x10EF, // 5  GREEN
                0x30CF, // 6  Green-
                0x28D7, // 7  Turquoise
                0x18E7, // 8  Cyan
                0x08F7, // 9  Blue-
                0x708F, // 10 BLUE
                0x50AF, // 11 Blue+
                0x6897, // 12 Lila
                0x58A7, // 13 Fuchsia
                0x48B7, // 14 Pink
                0xD02F, // 15 WHITE
        };
        for (int i = 0; i < 16; ++i) {
            actions[i].init(remote_transmitter_remotetransmittercomponent, buttons[i]);
        }
    }

    LightTraits get_traits() override {
        // return the traits this light supports
        auto traits = LightTraits();
        traits.set_supports_brightness(false);
        traits.set_supports_rgb(true);
        traits.set_supports_rgb_white_value(false);
        traits.set_supports_color_temperature(false);
        return traits;
    }

    void write_state(LightState *state) override {
        // This will be called by the light to get a new state to be written.
        float r, g, b;
        state->current_values_as_rgb(&r, &g, &b);


        ESP_LOGD("custom", "The value of red is: %f", r);
        ESP_LOGD("custom", "The value of green is: %f", g);
        ESP_LOGD("custom", "The value of blue is: %f", b);


        // Write red, green and blue to HW
        int H; // Max 360
        float S; // Max 1.0
        float L; // Max 1.0

        float min = Min(Min(r, g), b);
        float max = Max(Max(r, g), b);
        float delta = max - min;

        L = (max + min) / 2;

        if (delta == 0) {
            H = 0;
            S = 0.0f;
        } else {
            S = (L <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

            float hue;

            if (r == max) {
                hue = ((g - b) / 6) / delta;
            } else if (g == max) {
                hue = (1.0f / 3) + ((b - r) / 6) / delta;
            } else {
                hue = (2.0f / 3) + ((r - g) / 6) / delta;
            }

            if (hue < 0)
                hue += 1;
            if (hue > 1)
                hue -= 1;

            H = (int) (hue * 360);
        }

        ESP_LOGD("custom", "The value of H is: %d", H);
        ESP_LOGD("custom", "The value of S is: %f", S);
        ESP_LOGD("custom", "The value of L is: %f", L);

        if (S == 0 || L > 0.95) {
            actions[15].play();
        } else {
            if (H < 8 || H >= 350) {
                actions[0].play();
            } else if (H < 19) {
                actions[1].play();
            } else if (H < 32) {
                actions[2].play();
            } else if (H < 48) {
                actions[3].play();
            } else if (H < 78) {
                actions[4].play();
            } else if (H < 122) {
                actions[5].play();
            } else if (H < 154) {
                actions[6].play();
            } else if (H < 172) {
                actions[7].play();
            } else if (H < 188) {
                actions[8].play();
            } else if (H < 209) {
                actions[9].play();
            } else if (H < 230) {
                actions[10].play();
            } else if (H < 256) {
                actions[11].play();
            } else if (H < 287) {
                actions[12].play();
            } else if (H < 310) {
                actions[13].play();
            } else if (H < 350) {
                actions[14].play();
            }
        }
    }
};

#endif //ESPHOME_RGB_IR_LIGHT_H
