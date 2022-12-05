#ifndef _NEAT_POPULATION_HPP
#define _NEAT_POPULATION_HPP

#include <cstdlib>
#include <cmath>

#include <array>
#include <vector>

#include <functional>
#include <utility>

#include "env/conf.hpp"

#include "neat/genotype.hpp"

namespace Population
{
    class Specie
    {
        private:
            size_t staleness = 0;

            float max_fitness = 0.f;
            float avg_fitness = 0.f;

            float crossover_prob = CONF::CROSSOVER_PROB;

            std::vector<Genotype::Genome> genomes;

        public:
            Specie();

            void calc_avg_fitness();

            void breed_child(Genotype::Genome& child, size_t& innovation) const;

            float get_avg_fitness() const;
            size_t get_staleness() const;
            float get_max_fitness() const;

            void set_staleness(size_t staleness);
            void set_max_fitness(float max_fitness);

            std::vector<Genotype::Genome>& self_genomes();
    };

    class Pool
    {
        private:
            struct Interp
            {
                static inline float linear(float x, float x1, float x2, float y1, float y2)
                {
                    return y1 + ((std::min(std::max(x, x1), x2) - x1) / (x2 - x1)) * (y2 - y1);
                }

                static inline float exponential(float x, float x1, float x2, float y1, float y2)
                {
                    return std::exp(std::log(y1) +
                        ((std::min(std::max(x, x1), x2) - x1) / (x2 - x1)) * (std::log(y2) - std::log(y1)));
                }
            };

        private:
            size_t curr_specie = 0;
            size_t curr_genome = 0;

            size_t generation = 0;
            size_t innovation = 0;

            float max_fitness = 0.f;
            float tot_avg_fitness = 0.f;

            size_t stale_species = CONF::STALE_SPECIES;
            size_t population_size = CONF::POPULATION_SIZE;

            size_t start_population_size = CONF::POPULATION_SIZE;
            size_t end_population_size = CONF::END_POPULATION_SIZE;
            size_t population_gens_inc = CONF::POPULATION_GENS_INC;
            size_t population_inc_freq = CONF::POPULATION_INC_FREQ;
            float(*interp_func)(float, float, float, float, float) = Population::Pool::Interp::linear;

            std::vector<Population::Specie> species;

        private:
            void calc_tot_avg_fitness();
            void calc_glob_rank();

            void cull_species(bool half);
            void pop_stale_species();
            void pop_weak_species();

            void add_to_species(Genotype::Genome& child);

            void new_generation();

        public:
            Pool();

            void init();

            void eval_curr_genome(const std::array<float, CONF::INPUTS>& obs, std::array<float, CONF::OUTPUTS>& act);

            bool fitness_pass();
            void next_genome(Genotype::Genome& best);
            void copy_best_genome(Genotype::Genome& best);

            size_t get_generation() const;
            float get_max_fitness() const;

            void set_max_fitness(float max_fitness);

            Genotype::Genome& self_curr_genome();
    };
}

#endif
