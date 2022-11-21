#include "env/env/pipe.hpp"

Pipe::Pipe()
{
    this->pos_x = CONST::WIDTH + this->width;

    this->pos_y = (
            (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX))
            * (CONST::HEIGHT - this->gap - (2.f * this->h_head) - CONST::FLOOR_Y - 2.f)
            ) + (this->gap / 2.f) + this->h_head + 1.f;

    this->rects[0][0] = math::Vector2f(this->pos_x, 0.f);
    this->rects[0][1] = math::Vector2f(this->pos_x + this->width, this->pos_y - (this->gap / 2.f));

    this->rects[1][0] = math::Vector2f(this->pos_x, this->pos_y + (this->gap / 2.f));
    this->rects[1][1] = math::Vector2f(this->pos_x + this->width, CONST::HEIGHT);
}

void Pipe::move()
{
    this->pos_x -= this->speed;
    for(size_t i = 0; i < 2; i++) {
        for(size_t j = 0; j < 2; j++) {
            this->rects[i][j].x -= this->speed;
        }
    }
}

bool Pipe::passed(float x) const
{
    return (this->pos_x + this->width < x);
}

bool Pipe::middle_of_screen() const
{
    return (this->pos_x - (this->width / 2.f) == CONST::WIDTH / 2.f);
}

bool Pipe::out_of_screen() const
{
    return (this->pos_x + this->width < 0.f);
}

void Pipe::set_is_passed() { this->is_passed = true; }

float Pipe::get_y() const { return this->pos_y; }

float Pipe::get_width() const { return this->width; }

bool Pipe::get_is_passed() const { return this->is_passed; }

float Pipe::get_gap() const { return this->gap; }

float Pipe::get_h_head() const { return this->h_head; }

const std::array<std::array<math::Vector2f, 2>, 2>& Pipe::get_rects() const { return this->rects; }


Pipes::Pipes()
{
    this->pipes.emplace_back();
}

void Pipes::add_pipe()
{
    for(const auto& pipe : this->pipes) {
        if(pipe.middle_of_screen()) {
            this->pipes.emplace_back();
            break;
        }
    }
}

void Pipes::remove_pipe()
{
    if(this->pipes.front().out_of_screen()){
        this->pipes.pop_front();
    }
}

void Pipes::move_pipes()
{
    for(auto& pipe : this->pipes) {
        pipe.move();
    }
}

void Pipes::next_pipe(const float x)
{
    size_t i = 0;
    for (auto &pipe: this->pipes) {
        if(! pipe.passed(x)) {
            this->next_pipe_i = i;
            return;
        }
        i++;
    }
}

bool Pipes::passed_pipe(const float x)
{
    for(auto& pipe : this->pipes) {
        if(pipe.passed(x) && (! pipe.get_is_passed())){
            pipe.set_is_passed();
            return true;
        }
    }
    return false;
}

float Pipes::get_floor_y() const { return CONST::FLOOR_Y; }

const Pipe& Pipes::get_next_pipe() const { return this->pipes[next_pipe_i]; }

const std::deque<Pipe>& Pipes::get_pipes() const { return this->pipes; }