#ifndef _ENV_VIEW_HPP
#define _ENV_VIEW_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "env/conf.hpp"

#include "env/env.hpp"

#include "sfev/sfevmngr.hpp"
#include "sfev/sfmap.hpp"

#include "utils/logger.hpp"

/*** ADD INCLUDE HERE */


namespace View
{
    class EventHandler
    {
        private:
            /*** DEC EVENT STATE HERE */
            struct EventState
            {
            };

        private:
            EventState ev_state;

        private:
            EventHandler() = default;

        public:
            void ev_setup(sfev::EventManager& ev_manager, sf::RenderWindow& window);
            void get_action(std::vector<float>& act);

            inline const EventState& get_ev_state() const
            {
                return this->ev_state;
            }

        public:
            EventHandler(const EventHandler& other) = delete;
            EventHandler operator=(const EventHandler& other) = delete;

            static EventHandler& EVENTHANDLER()
            {
                static EventHandler singleton;
                return singleton;
            }
    };

    class Renderer
    {
        private:
            Renderer() = default;

        private:
            std::string res = "../../res/";

        private:
            /*** DEC DRAW VARS HERE */


        private:
            /*** DEC DRAW FUNCS HERE */


        public:
            void draw_setup(const MyEnv::Model& m, sf::RenderTarget& window);
            void draw_loop(const MyEnv::Model& m, sf::RenderTarget& window);

        public:
            Renderer(const Renderer& other) = delete;
            Renderer operator=(const Renderer& other) = delete;

            static Renderer& RENDERER()
            {
                static Renderer singleton;
                return singleton;
            }
    };
}

#endif