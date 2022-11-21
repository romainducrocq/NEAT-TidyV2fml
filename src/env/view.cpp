#include "env/view.hpp"

/*** DEF EVENT SETUP HERE */
void View::EventHandler::ev_setup(sfev::EventManager& /*ev_manager*/, sf::RenderWindow& /*window*/)
{
}

/*** DEF ACTION HANDLER HERE */
void View::EventHandler::get_action(std::vector<float>& /*act*/)
{
}

/*** DEF DRAW SETUP HERE */
void View::Renderer::draw_setup(const MyEnv::Model& /*m*/, sf::RenderTarget& /*window*/)
{
}

/*** DEF DRAW LOOP HERE */
void View::Renderer::draw_loop(const MyEnv::Model& /*m*/, sf::RenderTarget& /*window*/)
{
}

/*** DEF DRAW FUNCS HERE */

