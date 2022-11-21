#include "env/env/bird.hpp"

Bird::Bird()
{
    this->y_lim[0] = CONST::HEIGHT - (this->r + CONST::FLOOR_Y);
    this->y_lim[1] = this->r;
    this->back_x = this->pos_x - this->r;
}

void Bird::move(size_t action)
{
    if(action == CONF::Action::JUMP) {
        this->speed -= this->jump;
    }

    this->speed += this->fall;
    this->speed *= this->d_a;
    this->pos_y = math::clip(this->r, this->y_lim[0], this->pos_y + this->speed);
}

bool Bird::is_collision(const std::array<std::array<math::Vector2f, 2>, 2>& rects)
{
    for(size_t i = 0; i < this->n_collision_points; i++){
        math::point_on_circle((math::PI * 2.f * i) / this->n_collision_points, this->r, this->pos_x, this->pos_y, this->collision_point);
        for(size_t j = 0; j < 2; j++) {
            if(this->collision_point.x >= rects[j][0].x && this->collision_point.x <= rects[j][1].x
            && this->collision_point.y >= rects[j][0].y && this->collision_point.y <= rects[j][1].y) {
                return true;
            }
        }
    }
    return false;
}

void Bird::rotate_theta()
{
    this->theta = math::linterp(this->pos_y - this->y_prev, -this->jump, this->jump, -math::PI / 4.f, math::PI / 4.f);
    this->y_prev = this->pos_y;
}

void Bird::reward()
{
    this->score += 1.f;
}

float Bird::get_x() const { return this->pos_x; }

float Bird::get_y() const { return this->pos_y; }

float Bird::get_r() const { return this->r; }

float Bird::get_theta() const { return this->theta; }

float Bird::get_back_x() const { return this->back_x; }

float Bird::get_speed() const { return this->speed; }

size_t Bird::get_score() const { return this->score; }

const std::array<float, 2>& Bird::get_y_lim() const { return this->y_lim; }


Agent::Agent()
{
    this->x_vertex[0].x = this->pos_x - this->r;

    this->y_vertex[0].x = this->pos_x - this->r;
    this->y_vertex[1].x = this->pos_x - this->r;
}

void Agent::obs_x_vertex(float pipe_back_x, float max_dist_x)
{
    this->x_vertex[0].y = this->pos_y;
    this->x_vertex[1].x = math::clip(this->pos_x - this->r, this->pos_x - this->r + max_dist_x, pipe_back_x);
    this->x_vertex[1].y = this->pos_y;
}

void Agent::obs_y_vertex(float pipe_y)
{
    this->y_vertex[0].y = this->pos_y;
    this->y_vertex[1].y = pipe_y;
}

float Agent::get_obs_dist_x() const
{
    return math::dist(this->x_vertex[0], this->x_vertex[1]);
}

float Agent::get_obs_dist_y() const
{
    return math::dist(this->y_vertex[0], this->y_vertex[1]);
}

const std::array<math::Vector2f, 2>& Agent::get_obs_x_vertex() const { return this->x_vertex; }
const std::array<math::Vector2f, 2>& Agent::get_obs_y_vertex() const { return this->y_vertex; }