#include "env/env.hpp"

/*** DEF INIT FUNC HERE */
void MyEnv::Env::init_func()
{
    this->m.mode = this->Super::mode;

    this->Super::minmax_map.insert({
        "dist_x",
        Super::Scale::MinMax(0.f, (CONST::WIDTH / 2.f) + this->m.pipes.get_next_pipe().get_width())
    });
    this->Super::minmax_map.insert({
        "dist_y",
        Super::Scale::MinMax(0.f, std::abs(this->m.bird.get_y_lim()[0]
            - ((this->m.pipes.get_next_pipe().get_gap() / 2.f) + this->m.pipes.get_next_pipe().get_h_head() + 1.f)))
    });
    this->Super::minmax_map.insert({"speed", Super::Scale::MinMax(-46.8f, 7.2f)});
}

/*** DEF OBS FUNC HERE */
void MyEnv::Env::obs_func()
{
    this->m.bird.obs_x_vertex(this->m.pipes.get_next_pipe().get_rects()[0][1].x, this->Super::minmax_map.at("dist_x").max);
    this->m.bird.obs_y_vertex(this->m.pipes.get_next_pipe().get_y());

    this->mdp.obs[0] = this->Super::minmax_map.at("dist_x").minmax(this->m.bird.get_obs_dist_x());
    this->mdp.obs[1] = ((this->Super::minmax_map.at("dist_y").minmax(this->m.bird.get_obs_dist_y()) *
            (this->m.bird.get_y() >= this->m.pipes.get_next_pipe().get_y() ? 1.f : -1.f)) + 1.f) / 2.f;

    this->mdp.obs[2] = this->Super::minmax_map.at("speed").minmax(this->m.bird.get_speed());
}

/*** DEF ACT FUNC HERE */
void MyEnv::Env::act_func()
{
    if(Super::Output::activate(this->mdp.act[0])){
        this->m.bird.move(CONF::JUMP);
    }else{
        this->m.bird.move(CONF::NOOP);
    }
}

/*** DEF DONE FUNC HERE */
void MyEnv::Env::done_func()
{
    this->mdp.done = this->m.bird.is_collision(this->m.pipes.get_next_pipe().get_rects());
}

/*** DEF FITNESS FUNC HERE */
void MyEnv::Env::fitness_func()
{
    switch(this->Super::mode){
        case CONF::Mode::TRAIN:
        case CONF::Mode::EVAL:
            this->Super::mdp.fitness +=
                    std::pow(1.f - this->Super::minmax_map.at("dist_y").minmax(this->m.bird.get_obs_dist_y()), 2);
            break;

        default:
            break;
    }
}

/*** DEF INFO FUNC HERE */
void MyEnv::Env::info_func()
{
    if(this->m.bird.get_score() > this->m.max_score){
        this->m.max_score = this->m.bird.get_score();
    }

    switch(this->Super::mode){

        case CONF::Mode::TRAIN:
            std::cout << "GENERATION  : " << this->Super::generation << " / " << this->Super::max_generation_train << "\n";
            std::cout << "MAX FITNESS : " << this->Super::max_fitness << "\n";
            std::cout << "MVG AVG     : " << this->Super::mvg_avg.get() << "\n";
            std::cout << "MAX SCORE   : " << this->m.max_score << "\n";
            std::cout << "\n";
            break;

        case CONF::Mode::EVAL:
            std::cout << "EPOCH       : " << this->Super::epoch << " / " << this->Super::max_epoch_eval << "\n";
            std::cout << "FITNESS     : " << this->Super::mdp.fitness << "\n";
            std::cout << "MVG AVG     : " << this->Super::mvg_avg.get() << "\n";
            std::cout << "SCORE       : " << this->m.bird.get_score() << "\n";
            std::cout << "MAX SCORE   : " << this->m.max_score << "\n";
            std::cout << "\n";
            break;

        case CONF::Mode::PLAY:
            std::cout << "EPOCH     : " << this->Super::epoch << " / " << this->Super::max_epoch_eval << "\n";
            std::cout << "SCORE     : " << this->m.bird.get_score() << "\n";
            std::cout << "MAX SCORE : " << this->m.max_score << "\n";
            std::cout << "\n";
            break;

        default:
            break;
    }
}

/*** DEF NOOP FUNC HERE */
void MyEnv::Env::noop_func()
{
}

/*** DEF RESET FUNC HERE */
void MyEnv::Env::reset_func()
{
    this->m.bird = Agent();
    this->m.pipes = Pipes();
}

/*** DEF STEP FUNC HERE */
void MyEnv::Env::step_func()
{
    this->m.pipes.add_pipe();
    this->m.pipes.move_pipes();
    this->m.pipes.remove_pipe();
    this->m.pipes.next_pipe(this->m.bird.get_back_x());
    if(this->m.pipes.passed_pipe(this->m.bird.get_back_x())){
        this->m.bird.reward();
    }
}

/*** DEF RESET RENDER FUNC HERE */
void MyEnv::Env::reset_render_func()
{
}

/*** DEF STEP RENDER FUNC HERE */
void MyEnv::Env::step_render_func()
{
    this->m.bird.rotate_theta();
}