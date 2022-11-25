#include "play.hpp"

App::Play::Play()
{
}

void App::Play::run()
{
    Timer timer;

    std::cout << "\n";
    std::cout << "-------------------------------PLAY--------------------------------" << "\n";
    std::cout << "\n";

    this->Super::app_run();
}

void App::Play::setup()
{
    this->init();
}

bool App::Play::loop()
{
    return this->play();
}

void App::Play::ev_setup()
{
    View::EventHandler::EVENTHANDLER().ev_setup(this->Super::ev_manager, this->Super::window);
}

void App::Play::draw_setup()
{
    View::Renderer::RENDERER().draw_setup(this->env.get_m(), this->Super::window);
}

void App::Play::draw_loop()
{
    View::Renderer::RENDERER().draw_loop(this->env.get_m(), this->Super::window);
}

void App::Play::init()
{
    this->env.init_func();

    this->env.mvg_avg = MovingAverage(this->env.mvg_avg_max);

    this->env.mdp.act.clear();
    for(size_t i = 0; i < this->env.outputs; i++){
        this->env.mdp.act.push_back(0.f);
    }

    this->env.epoch = 0;

    this->reset();
}

void App::Play::reset()
{
    this->env.reset_func();

    this->env.reset_render_func();

    this->env.steps = 0;

    this->env.mdp.done = false;
    this->env.mdp.fitness = 0.f;

    this->env.epoch++;
}

void App::Play::step()
{
    this->env.step_func();

    this->env.step_render_func();

    View::EventHandler::EVENTHANDLER().get_action(this->env.mdp.act);

    this->env.act_func();

    this->env.steps++;

    this->env.done_func();

    this->env.fitness_func();

    if(this->env.mdp.done){
        this->env.mvg_avg.add(this->env.mdp.fitness);
    }
}

bool App::Play::play()
{
    this->step();

    if(this->env.mdp.done) {
        this->env.info_func();
        this->env.ss_info.str(std::string());

        this->reset();
    }

    if(this->env.max_epoch_eval && this->env.epoch > this->env.max_epoch_eval){

        return false;
    }

    return true;
}
