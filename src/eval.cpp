#include "eval.hpp"

App::Eval::Eval()
{
}

void App::Eval::run()
{
    Timer timer;

    std::cout << "\n";
    std::cout << "-------------------------------EVAL--------------------------------" << "\n";
    std::cout << "\n";

    this->Super::app_run();
}

void App::Eval::setup()
{
    this->env.init(CONF::Mode::EVAL);
}

bool App::Eval::loop()
{
    return this->env.eval();
}


void App::Eval::ev_setup()
{
    View::EventHandler::EVENTHANDLER().ev_setup(this->Super::ev_manager, this->Super::window);
}

void App::Eval::draw_setup()
{
    View::Renderer::RENDERER().draw_setup(this->env.get_m(), this->Super::window);
}

void App::Eval::draw_loop()
{
    View::Renderer::RENDERER().draw_loop(this->env.get_m(), this->Super::window);
}