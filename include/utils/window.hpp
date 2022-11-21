#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <SFML/Graphics.hpp>

#include "env/conf.hpp"

#include "sfev/sfevmngr.hpp"

class Window
{
    private:
        std::string name = CONF::NAME;
        size_t win_w = CONF::WIN_W;
        size_t win_h = CONF::WIN_H;
        size_t framerate = CONF::FRAMERATE;
        size_t antialias = CONF::ANTIALIAS;
        size_t bg_col[3] = { CONF::BG_COL[0],
                             CONF::BG_COL[1],
                             CONF::BG_COL[2] };

    protected:
        sf::RenderWindow window;
        sfev::EventManager ev_manager;

    private:
        void app_setup();
        void app_loop();

    protected:
        void app_run();

        virtual void ev_setup() = 0;
        virtual void draw_setup() = 0;
        virtual void draw_loop() = 0;

        virtual void setup() = 0;
        virtual bool loop() = 0;

    public:
        Window();
};

#endif