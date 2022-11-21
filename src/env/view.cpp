#include "env/view.hpp"

/*** DEF EVENT SETUP HERE */
void View::EventHandler::ev_setup(sfev::EventManager& ev_manager, sf::RenderWindow& window)
{
    ev_manager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv){
        window.close();
    });

    ev_manager.addKeyReleasedCallback(sf::Keyboard::Escape, [&](sfev::CstEv){
        window.close();
    });

    ev_manager.addKeyReleasedCallback(sf::Keyboard::Space, [&](sfev::CstEv){
        this->ev_state.obs_view = ! this->ev_state.obs_view;
    });

    ev_manager.addKeyPressedCallback(sf::Keyboard::Up, [&](sfev::CstEv){
        ev_state.action = CONF::JUMP;
    });

    ev_manager.addKeyReleasedCallback(sf::Keyboard::Up, [&](sfev::CstEv){
        ev_state.action = CONF::NOOP;
    });
}

/*** DEF ACTION HANDLER HERE */
void View::EventHandler::get_action(std::vector<float>& act)
{
    switch(this->ev_state.action){
        case CONF::JUMP:
            act[0] = 1.f;
            break;
        case CONF::NOOP:
        default:
            act[0] = -1.f;
            break;
    }
}

/*** DEF DRAW SETUP HERE */
void View::Renderer::draw_setup(const MyEnv::Model& /*m*/, sf::RenderTarget& /*window*/)
{
    this->load_assets();
}

/*** DEF DRAW LOOP HERE */
void View::Renderer::draw_loop(const MyEnv::Model& m, sf::RenderTarget& window)
{
    this->render_background(window);
    this->render_bird(m.bird, window);
    this->render_pipes(m.pipes, window);
    this->render_foreground(window);
    this->render_score(m.bird, window);
    if(View::EventHandler::EVENTHANDLER().get_ev_state().obs_view &&
       (m.mode == CONF::Mode::TRAIN || m.mode == CONF::Mode::EVAL)){
        this->render_obs_vertices(m.bird, window);
    }
}

/*** DEF DRAW FUNCS HERE */

void View::Renderer::load_sprite(sf::Texture& texture, sf::Sprite& sprite, const std::string& dir, const std::string& file,
                                 float position_x, float position_y, float scale_x, float scale_y)
{
    texture.loadFromFile(dir + file);
    sprite.setTexture(texture);
    sprite.setPosition(position_x, position_y);
    sprite.setScale(scale_x, scale_y);
}

void View::Renderer::draw_sprite(sf::Sprite& sprite, float position_x, float position_y, float rotation,
                                 sf::RenderTarget& window)
{
    sprite.setPosition(position_x, position_y);
    sprite.setRotation(rotation);
    window.draw(sprite);
}

void View::Renderer::draw_text(sf::Text& text, sf::Font& font, const std::string& str, size_t size, float position_x,
                               float position_y, sf::Color color, sf::RenderTarget& window)
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position_x, position_y);
    window.draw(text);
}

void View::Renderer::draw_line(sf::Vertex (&line)[2], float position_x1, float position_y1, float position_x2,
                               float position_y2, sf::Color color, sf::RenderTarget& window)
{
    line[0].position.x = position_x1;
    line[0].position.y = position_y1;
    line[0].color = color;

    line[1].position.x = position_x2;
    line[1].position.y = position_y2;
    line[1].color = color;

    window.draw(line, 2, sf::Lines);
}

void View::Renderer::load_assets()
{
    View::Renderer::load_sprite(this->background_texture, this->background_sprite,
                                this->res + "img/", "background.png", 0.f, 0.f, 0.5f, 0.5f);
    View::Renderer::load_sprite(this->foreground_texture, this->foreground_sprite,
                                this->res + "img/", "foreground.png", 0.f, 0.f, 0.5f, 0.5f);
    View::Renderer::load_sprite(this->pipe_head_up_texture, this->pipe_head_up_sprite,
                                this->res + "img/", "pipe_head_up.png", 0.f, 0.f, 0.5f, 0.5f);
    View::Renderer::load_sprite(this->pipe_head_down_texture, this->pipe_head_down_sprite,
                                this->res + "img/", "pipe_head_down.png", 0.f, 0.f, 0.5f, 0.5f);
    View::Renderer::load_sprite(this->pipe_body_texture, this->pipe_body_sprite,
                                this->res + "img/", "pipe_body_full.png", 0.f, 0.f, 0.5f, 0.5f);
    View::Renderer::load_sprite(this->bird_texture, this->bird_sprite,
                                this->res + "img/", "seamonkey.png", 0.f, 0.f, 0.5f, 0.5f);
    this->bird_sprite.setOrigin(sf::Vector2f(this->bird_sprite.getLocalBounds().width * 4.f/3.f,
                                             this->bird_sprite.getLocalBounds().height) / 2.f);

    this->roboto_font.loadFromFile(this->res + "font/" + "Roboto-Regular.ttf");
}

void View::Renderer::render_background(sf::RenderTarget& window)
{
    View::Renderer::draw_sprite(this->background_sprite, 0.f, 0.f, 0.f, window);
}

void View::Renderer::render_foreground(sf::RenderTarget& window)
{
    View::Renderer::draw_sprite(this->foreground_sprite, 0.f, 0.f, 0.f, window);
}

void View::Renderer::render_bird(const Bird& bird, sf::RenderTarget& window)
{
    View::Renderer::draw_sprite(this->bird_sprite, bird.get_x(), bird.get_y(), math::rad_to_deg(bird.get_theta()), window);
}

void View::Renderer::render_pipes(const Pipes& pipes, sf::RenderTarget& window)
{
    for(const auto& pipe : pipes.get_pipes()) {
        View::Renderer::draw_sprite(this->pipe_body_sprite, pipe.get_rects()[0][0].x,
                         -(this->pipe_body_sprite.getLocalBounds().height / 2.f) + pipe.get_rects()[0][1].y, 0.f, window);
        View::Renderer::draw_sprite(this->pipe_body_sprite, pipe.get_rects()[1][0].x, pipe.get_rects()[1][0].y, 0.f, window);

        View::Renderer::draw_sprite(this->pipe_head_up_sprite, pipe.get_rects()[0][0].x, pipe.get_rects()[0][1].y - pipe.get_h_head(), 0.f, window);
        View::Renderer::draw_sprite(this->pipe_head_down_sprite, pipe.get_rects()[1][0].x, pipe.get_rects()[1][0].y, 0.f, window);
    }
}

void View::Renderer::render_score(const Bird& bird, sf::RenderTarget& window)
{
    View::Renderer::draw_text(this->text, this->roboto_font, "Score : " + std::to_string(bird.get_score()),
                              24, 10.f, 10.f, sf::Color::Black, window);
}

void View::Renderer::render_obs_vertices(const Agent& agent, sf::RenderTarget& window)
{
    View::Renderer::draw_line(this->line, agent.get_obs_x_vertex()[0].x, agent.get_obs_x_vertex()[0].y,
                              agent.get_obs_x_vertex()[1].x, agent.get_obs_x_vertex()[1].y, sf::Color::Red, window);
    View::Renderer::draw_line(this->line, agent.get_obs_y_vertex()[0].x, agent.get_obs_y_vertex()[0].y,
                              agent.get_obs_y_vertex()[1].x, agent.get_obs_y_vertex()[1].y, sf::Color::Red, window);
}
