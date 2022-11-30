#ifndef _NEAT_NEAT_HPP
#define _NEAT_NEAT_HPP

#include <limits>
#include <cstring>
#include <sstream>

#include <array>
#include <vector>
#include <unordered_map>
#include <utility>

#include <algorithm>

#include <mutex>

#include <pbplots/pbPlots.hpp>
#include <pbplots/supportLib.hpp>

#include "env/conf.hpp"

#include "neat/population.hpp"

#include "utils/mvgavg.hpp"

extern std::unordered_map<std::string, std::mutex> mutex_map;

class Neat
{
    private:
        struct Mdp
        {
            std::array<float, CONF::INPUTS> obs = { 0.f };
            std::array<float, CONF::OUTPUTS> act = { 0.f };
            bool done = false;
            float fitness = 0.f;
        };

    protected:
        struct Scale
        {
            static inline float out01(float z)
            {
                return (z + 1.f) / 2.f;
            }

            struct MinMax
            {
                float min = 0.f;
                float max = 1.f;

                inline MinMax(float min, float max)
                    : min(min), max(max)
                {
                    if(this->max - this->min == 0.f){
                        this->max += std::numeric_limits<float>::min();
                    }
                }

                inline float minmax(float z) const
                {
                    return (z - this->min) / (this->max - this->min);
                }
            };
        };

        struct Output
        {
            static inline bool activate(float z)
            {
                return z > std::numeric_limits<float>::min();
            }

            static inline size_t argmax(const std::vector<float>& out)
            {
                size_t argmax = 0;
                float max = -std::numeric_limits<float>::max();

                for(size_t i = 0; i < out.size(); i++){
                    if(out[i] > max){
                        max = out[i];
                        argmax = i;
                    }
                }

                return argmax;
            }
        };

    private:
        size_t inputs = CONF::INPUTS;

        size_t repeat = CONF::ACT_REPEAT;
        size_t max_noop = CONF::MAX_NOOP;

        size_t noops = 0;

        Population::Pool pool;

        Genotype::Genome best;

        std::string log_sav = CONF::LOG_SAV;
        std::string log_plt = CONF::LOG_PLT;
        size_t plt_freq = CONF::PLT_FREQ;
        size_t sav_freq = CONF::SAV_FREQ;
        std::pair<std::vector<double>, std::vector<double>> plt_data;

    protected:
        size_t mode = CONF::MODE;

        size_t outputs = CONF::OUTPUTS;

        size_t max_epoch_eval = CONF::EPOCHS_EVAL;
        size_t max_generation_train = CONF::GENERATIONS_TRAIN;

        size_t max_step = CONF::MAX_STEP;
        size_t mvg_avg_max = CONF::MVG_AVG;

        bool ini = true;
        size_t steps = 0;
        size_t epoch = 0;
        size_t generation = 0;
        float max_fitness = 0.f;
        bool is_noop = false;

        Neat::Mdp mdp;

        MovingAverage mvg_avg;
        std::stringstream ss_info;

        std::unordered_map<std::string, Neat::Scale::MinMax> minmax_map;

    protected:
        virtual void init_func() = 0;

        virtual void obs_func() = 0;
        virtual void act_func() = 0;
        virtual void done_func() = 0;
        virtual void fitness_func() = 0;
        virtual void info_func() = 0;

        virtual void noop_func() = 0;

        virtual void reset_func() = 0;
        virtual void step_func() = 0;

        virtual void reset_render_func() = 0;
        virtual void step_render_func() = 0;

    private:
        void obs();
        void act();
        void done();
        void fitness();
        void info();

        void noop();

        void reset();
        void step();

        void reset_render();
        void step_render();

        void to_plt();
        void plot();

    public:
        Neat();

        void init(size_t mode);

        bool train();
        bool eval();
};

#endif
