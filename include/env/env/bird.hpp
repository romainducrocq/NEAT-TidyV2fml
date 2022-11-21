#ifndef _ENV_BIRD_HPP
#define _ENV_BIRD_HPP

#include <array>

#include "env/env/utils.hpp"

namespace View { class Renderer; }

class Bird
{
    protected:
        friend class View::Renderer;

    protected:
        float pos_x = CONST::WIDTH / 4.f;
        float r = 20.f;
        size_t n_collision_points = 10;

        float fall = 0.8f;
        float jump = 6.f; // TODO: tune params
        float d_a = 0.9f;

        float pos_y = CONST::HEIGHT / 2.f;
        float speed = 0.f;

        float score = 0.f;

        float theta = 0.f;
        float y_prev = 0.f;

        float back_x;
        std::array<float, 2> y_lim;

        math::Vector2f collision_point = math::Vector2f();

    public:
        Bird();

        void move(size_t action);
        bool is_collision(const std::array<std::array<math::Vector2f, 2>, 2>& rects);
        void rotate_theta();
        void reward();

        float get_x() const;
        float get_y() const;
        float get_r() const;
        float get_theta() const;
        float get_back_x() const;
        float get_speed() const;
        size_t get_score() const;
        const std::array<float, 2>& get_y_lim() const;
};

class Agent : public Bird
{
    private:
        typedef Bird Super;

    private:
        std::array<math::Vector2f, 2> x_vertex = { math::Vector2f(), math::Vector2f() };
        std::array<math::Vector2f, 2> y_vertex = { math::Vector2f(), math::Vector2f() };

    public:
        Agent();

        void obs_x_vertex(float pipe_back_x, float max_dist_x);
        void obs_y_vertex(float pipe_y);

        float get_obs_dist_x() const;
        float get_obs_dist_y() const;

        const std::array<math::Vector2f, 2>& get_obs_x_vertex() const;
        const std::array<math::Vector2f, 2>& get_obs_y_vertex() const;
};

#endif