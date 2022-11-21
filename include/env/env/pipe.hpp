#ifndef _ENV_PIPE_HPP
#define _ENV_PIPE_HPP

#include <cstdlib>
#include <array>
#include <deque>

#include "env/env/utils.hpp"

namespace View { class Renderer; }

class Pipe
{
    private:
        friend class View::Renderer;

    private:
        float width = 60.f;
        float h_head = 45.f;
        float gap = 175.f;

        float speed = 2.f;

        float pos_x;
        float pos_y;

        std::array<std::array<math::Vector2f, 2>, 2> rects;

        bool is_passed = false;

    public:
        Pipe();

        void move();

        bool passed(float x) const;
        bool middle_of_screen() const;
        bool out_of_screen() const;

        void set_is_passed();

        float get_y() const;
        float get_width() const;
        bool get_is_passed() const;
        float get_gap() const;
        float get_h_head() const;
        const std::array<std::array<math::Vector2f, 2>, 2>& get_rects() const;
};

class Pipes
{
    private:
        std::deque<Pipe> pipes;
        size_t next_pipe_i = 0;

    public:
        Pipes();

        void add_pipe();
        void remove_pipe();
        void move_pipes();

        void next_pipe(float x);
        bool passed_pipe(float x);

        float get_floor_y() const;

        const Pipe& get_next_pipe() const;
        const std::deque<Pipe>& get_pipes() const;
};

#endif