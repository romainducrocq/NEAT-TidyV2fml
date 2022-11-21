#ifndef _PLAY_HPP
#define _PLAY_HPP

#include <iostream>
#include <cstring>
#include <sstream>

#include "env/conf.hpp"
#include "env/env.hpp"
#include "env/view.hpp"

#include "utils/window.hpp"
#include "utils/timer.hpp"

namespace App
{
    class Play : public Window
    {
        private:
            typedef Window Super;
            MyEnv::Env env;

        private:
            void init();

            void reset();
            void step();

            bool play();

        private:
            void run();

            void ev_setup() override;
            void draw_setup() override;
            void draw_loop() override;

            void setup() override;
            bool loop() override;

            Play();

        public:
            Play(const Play &other) = delete;

            Play operator=(const Play &other) = delete;

            static Play &PLAY()
            {
                static Play singleton;
                return singleton;
            }
    };
}

#endif