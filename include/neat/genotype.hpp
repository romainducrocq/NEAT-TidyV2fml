#ifndef _NEAT_GENOTYPE_HPP
#define _NEAT_GENOTYPE_HPP

#include <cmath>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <unordered_map>

#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>
#include <memory>

#include <fstream>
#include <nlohmann/json.hpp>

#include "env/conf.hpp"

namespace Genotype
{
    struct Activation
    {
        static inline float id(float z)
        {
            return z;
        }

        static inline float sigmoid(float z)
        {
            return 2.f / (1.f + std::exp(-4.9f * z)) - 1.f;
        }

        static inline float tanh(float z)
        {
            return (std::exp(z) - std::exp(-z)) / (std::exp(z) + std::exp(-z));
        }

        static inline float relu(float z)
        {
            return z > 0.f ? z : 0.f;
        }

        static inline float elu(float z)
        {
            return z > 0.f ? z : 1.f * (std::exp(z) - 1.f);
        }

        static inline float selu(float z)
        {
            return 1.0507f * (z > 0.f ? z : 1.67326f * (std::exp(z) - 1.f));
        }

        static inline void softmax(std::vector<float>& zs)
        {
            float sum = 0.f;

            for(const auto& zj : zs) {
                sum += std::exp(zj);
            }

            for(auto& zi : zs) {
                zi = std::exp(zi) / sum;
            }
        }
    };

    struct Gene
    {
        size_t into = 0;
        size_t out = 0;
        size_t innovation = 0;

        float weight = 0.f;
        bool enabled = true;

        Gene() {}
        Gene(size_t into, size_t out, size_t innovation, float weight, bool enabled)
            : into(into), out(out), innovation(innovation), weight(weight), enabled(enabled) {}
    };

    struct Neuron
    {
        std::vector<std::shared_ptr<Genotype::Gene>> incoming;
        float value = 0.f;
    };

    class Genome
    {
        private:
            size_t inputs = CONF::INPUTS + 1;
            size_t outputs = CONF::OUTPUTS;

            size_t lim_hidden = CONF::LIM_HIDDEN;

            size_t max_neuron = 0;

            float fitness = 0.f;
            size_t global_rank = 0;

            float mutation_rate_weight = CONF::MUTATE_WEIGHT_RATE;
            std::unordered_map<std::string, float> mutation_rates = {
                {"gene", CONF::MUTATE_GENE_RATE},
                {"link", CONF::MUTATE_LINK_RATE},
                {"bias", CONF::MUTATE_BIAS_RATE},
                {"neuron", CONF::MUTATE_NEURON_RATE},
                {"enable", CONF::MUTATE_ENABLE_RATE},
                {"disable", CONF::MUTATE_DISABLE_RATE},
                {"offset", CONF::MUTATE_OFFSET_SIZE}
            };
            float mutate_rate_decay = CONF::MUTATE_RATE_DECAY;

            float d_disjoint = CONF::DELTA_DISJOINT;
            float d_weight = CONF::DELTA_WEIGHTS;
            float d_threshold = CONF::DELTA_THRESHOLD;

            std::vector<std::shared_ptr<Genotype::Gene>> genes;
            std::map<size_t, Genotype::Neuron> neurons;

            std::set<size_t> innovation_set;
            std::unordered_map<size_t, std::shared_ptr<Genotype::Gene>> innovation_gene_map;

            float(*activation_func)(float) = Genotype::Activation::sigmoid;

        private:
            size_t inc_innovation(size_t& innovation) const;
            bool contains_link(size_t into, size_t out) const;

            void rand_neurons(size_t& into, size_t& out) const;

            void mutate_point();
            void mutate_link(bool bias, size_t& innovation);
            void mutate_node(size_t& innovation);
            void mutate_enable(bool enable);

            float delta_disjoints(const Genotype::Genome& other) const;
            float delta_weights(const Genotype::Genome& other) const;

        public:
            Genome();
            Genome(const Genotype::Genome& other) = delete;
            Genome(Genotype::Genome&& other) noexcept = default;
            Genotype::Genome& operator=(const Genotype::Genome& other) = delete;
            Genotype::Genome& operator=(Genotype::Genome&& other) noexcept = default;

            void copy_genome(const Genotype::Genome& other);

            void ctor_network();
            void eval_network(const std::vector<float>& obs, std::vector<float>& act);

            void mutate(size_t& innovation);

            bool same_specie(const Genotype::Genome& other) const;

            void crossover(const Genotype::Genome& other, Genotype::Genome& child) const;

            void simple_genome(size_t& innovation);

            void save(const std::string& file) const;
            void load(const std::string& file);

            float get_fitness() const;
            size_t get_global_rank() const;

            void set_fitness(float fitness);
            void set_global_rank(size_t global_rank);
    };
}

#endif
