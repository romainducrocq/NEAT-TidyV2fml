#ifndef _ENV_VIEW_HPP
#define _ENV_VIEW_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "env/conf.hpp"

#include "env/env.hpp"

#include "sfev/sfevmngr.hpp"

#include "utils/logger.hpp"

/*** ADD INCLUDE HERE */
#include "env/env/bird.hpp"
#include "env/env/pipe.hpp"
#include "env/env/utils.hpp"

namespace View
{
    class EventHandler
    {
        private:
            /*** DEC EVENT STATE HERE */
            struct EventState
            {
                bool obs_view = false;
                size_t action = CONF::NOOP;
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

            sf::Texture background_texture;
            sf::Sprite background_sprite;

            sf::Texture foreground_texture;
            sf::Sprite foreground_sprite;

            sf::Texture pipe_head_up_texture;
            sf::Sprite pipe_head_up_sprite;

            sf::Texture pipe_head_down_texture;
            sf::Sprite pipe_head_down_sprite;

            sf::Texture pipe_body_texture;
            sf::Sprite pipe_body_sprite;

            sf::Texture bird_texture;
            sf::Sprite bird_sprite;

            sf::Font roboto_font;
            sf::Text text;

            sf::Vertex line[2] = { sf::Vertex(), sf::Vertex() };

        private:
            /*** DEC DRAW FUNCS HERE */

            static void load_sprite(sf::Texture& texture, sf::Sprite& sprite, const std::string& dir, const std::string& file,
                                    float position_x, float position_y, float scale_x, float scale_y);

            static void draw_sprite(sf::Sprite& sprite, float position_x, float position_y, float rotation,
                                    sf::RenderTarget& window);

            static void draw_text(sf::Text& text, sf::Font& font, const std::string& str, size_t size, float position_x,
                                  float position_y, sf::Color color, sf::RenderTarget& window);

            static void draw_line(sf::Vertex (&line)[2], float position_x1, float position_y1, float position_x2,
                                  float position_y2, sf::Color color, sf::RenderTarget& window);

            void load_assets();

            void render_background(sf::RenderTarget& window);
            void render_foreground(sf::RenderTarget& window);
            void render_bird(const Bird& bird, sf::RenderTarget& window);
            void render_pipes(const Pipes& pipes, sf::RenderTarget& window);
            void render_score(const Bird& bird, sf::RenderTarget& window);
            void render_obs_vertices(const Agent& agent, sf::RenderTarget& window);

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