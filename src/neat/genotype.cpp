#include "neat/genotype.hpp"

Genotype::Genome::Genome()
{
}

// copyGenome
void Genotype::Genome::copy_genome(const Genotype::Genome& other)
{
    *this = Genotype::Genome();

    this->max_neuron = other.max_neuron;

    for(const auto& mutation_rate : other.mutation_rates){
        this->mutation_rates[mutation_rate.first] = mutation_rate.second;
    }

    for(const auto& gene : other.genes){
        this->genes.push_back(std::make_shared<Genotype::Gene>(*gene));

        this->innovation_set.insert(this->genes.back()->innovation);
        this->innovation_gene_map.insert({this->genes.back()->innovation, this->genes.back()});
    }
}

// generateNetwork
void Genotype::Genome::ctor_network()
{
    this->neurons.clear();

    for(size_t i = 0; i < this->inputs; i++){
        this->neurons.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple());
    }

    for(size_t i = 0; i < this->outputs; i++){
        this->neurons.emplace(std::piecewise_construct, std::forward_as_tuple(this->lim_hidden + i),
                              std::forward_as_tuple());
    }

    std::sort(this->genes.begin(), this->genes.end(), [](const auto& a, const auto& b) {
        return a->out < b->out;
    });

    for(auto& gene : this->genes){
        if(gene->enabled){
            if(!this->neurons.count(gene->out)){
                this->neurons.emplace(std::piecewise_construct, std::forward_as_tuple(gene->out),
                                      std::forward_as_tuple());
            }

            this->neurons.at(gene->out).incoming.push_back(gene);

            if(!this->neurons.count(gene->into)){
                this->neurons.emplace(std::piecewise_construct, std::forward_as_tuple(gene->into),
                                      std::forward_as_tuple());
            }
        }
    }
}

// evaluateNetwork
void Genotype::Genome::eval_network(const std::vector<float>& obs, std::vector<float>& act)
{
    for(auto& neuron : this->neurons){
        neuron.second.value = 0.f;
    }

    for(size_t i = 0; i < obs.size(); i++){
        this->neurons.at(i).value = obs[i];
    }
    this->neurons.at(obs.size()).value = 1.f; // bias

    for(auto& neuron : this->neurons){
        if(! neuron.second.incoming.empty()) {
            float dot = 0.f;
            for(const auto& gene : neuron.second.incoming) {
                dot += gene->weight * this->neurons.at(gene->into).value;
            }

            neuron.second.value = this->activation_func(dot);
        }
    }

    // classification
    for(size_t i = 0; i < this->outputs; i++){
        // act[i] = this->neurons.at(i + this->lim_hidden).value > 0;
        act[i] = this->neurons.at(i + this->lim_hidden).value;
    }
}

// randomNeuron
void Genotype::Genome::rand_neurons(size_t& into, size_t& out) const
{
    std::set<size_t> neurons;

    for(size_t i = 0; i < this->inputs; i++){
        neurons.insert(i);
    }

    for(size_t i = 0; i < this->outputs; i++){
        neurons.insert(i + this->lim_hidden);
    }

    for(const auto& gene : this->genes){
        neurons.insert(gene->into);
        neurons.insert(gene->out);
    }

    into = *std::next(neurons.begin(), std::rand() % neurons.size());

    do{
        out = *std::next(neurons.begin(), std::rand() % neurons.size());
    }while(out < this->inputs);
}

// newInnovation
size_t Genotype::Genome::inc_innovation(size_t& innovation) const
{
    innovation++;
    return innovation;
}

// containsLink
bool Genotype::Genome::contains_link(size_t into, size_t out) const
{
    return std::find_if(this->genes.begin(), this->genes.end(), [&](const auto& gene){
        return gene->into == into && gene->out == out;
    }) != this->genes.end();
}

// pointMutate
void Genotype::Genome::mutate_point()
{
    for(auto& gene : this->genes){
        if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < this->mutation_rate_weight){
            gene->weight = ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 4.f) - 2.f;
        } else {
            gene->weight += ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX))
                            * (this->mutation_rates["offset"] * 2.f)) - this->mutation_rates["offset"];
        }
    }
}

// linkMutate
void Genotype::Genome::mutate_link(bool bias, size_t& innovation)
{
    size_t into, out;
    this->rand_neurons(into, out);

    if(into < this->inputs && out < this->inputs){
        return;
    }
    if(out < this->inputs){
        std::swap(into, out);
    }

    if(bias){
        into = this->inputs - 1;
    }

    if(this->contains_link(into, out)){
        return;
    }

    this->genes.push_back(std::make_shared<Genotype::Gene>(
        into,
        out,
        this->inc_innovation(innovation),
        ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 4.f) - 2.f,
        true
    ));
    this->innovation_set.insert(this->genes.back()->innovation);
    this->innovation_gene_map.insert({this->genes.back()->innovation, this->genes.back()});
}

// nodeMutate
void Genotype::Genome::mutate_node(size_t& innovation)
{
    if(this->genes.empty()){
        return;
    }

    this->max_neuron++;

    size_t gene = std::rand() % this->genes.size();

    if(! this->genes[gene]->enabled){
        return;
    }
    this->genes[gene]->enabled = false;

    this->genes.push_back(std::make_shared<Genotype::Gene>(
        this->genes[gene]->into,
        this->max_neuron,
        this->inc_innovation(innovation),
        1.f,
        true
    ));
    this->innovation_set.insert(this->genes.back()->innovation);
    this->innovation_gene_map.insert({this->genes.back()->innovation, this->genes.back()});

    this->genes.push_back(std::make_shared<Genotype::Gene>(
        this->max_neuron,
        this->genes[gene]->out,
        this->inc_innovation(innovation),
        this->genes[gene]->weight,
        true
    ));
    this->innovation_set.insert(this->genes.back()->innovation);
    this->innovation_gene_map.insert({this->genes.back()->innovation, this->genes.back()});
}

// enableDisableMutate
void Genotype::Genome::mutate_enable(bool enable)
{
    std::vector<size_t> candidates;

    for(size_t i = 0; i < this->genes.size(); i++){
        if(this->genes[i]->enabled != enable){
            candidates.push_back(i);
        }
    }

    if(candidates.empty()){
        return;
    }

    size_t gene = candidates[std::rand() % candidates.size()];
    this->genes[gene]->enabled = ! this->genes[gene]->enabled;
}

// mutate
void Genotype::Genome::mutate(size_t& innovation)
{
    for(auto& mutation_rate : this->mutation_rates){
        if(this->mutate_rate_decay) {
            mutation_rate.second *= this->mutate_rate_decay;
        } else if(std::rand() % 2) {
            mutation_rate.second *= 0.95f;
        } else {
            mutation_rate.second *= 1.05263f;
        }
    }

    if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < this->mutation_rates["gene"]){
        this->mutate_point();
    }

    for(float p = this->mutation_rates["link"]; p > 0.f; p -= 1.f){
        if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < p){
            this->mutate_link(false, innovation);
        }
    }

    for(float p = this->mutation_rates["bias"]; p > 0.f; p -= 1.f){
        if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < p){
            this->mutate_link(true, innovation);
        }
    }

    for(float p = this->mutation_rates["neuron"]; p > 0.f; p -= 1.f){
        if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < p){
            this->mutate_node(innovation);
        }
    }

    for(float p = this->mutation_rates["enable"]; p > 0.f; p -= 1.f){
        if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < p){
            this->mutate_enable(true);
        }
    }

    for(float p = this->mutation_rates["disable"]; p > 0.f; p -= 1.f){
        if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < p){
            this->mutate_enable(false);
        }
    }
}

// disjoint
float Genotype::Genome::delta_disjoints(const Genotype::Genome& other) const
{
    std::vector<size_t> disjoints;

    std::set_symmetric_difference(this->innovation_set.begin(), this->innovation_set.end(),
                                  other.innovation_set.begin(), other.innovation_set.end(),
                                  std::back_inserter(disjoints));

    return disjoints.size() / std::fmax(this->innovation_set.size(), other.innovation_set.size());
}

// weights
float Genotype::Genome::delta_weights(const Genotype::Genome& other) const
{
    float sum = 0.f;
    size_t coincident = 0;

    for(const auto& gene : this->genes){
        if(other.innovation_gene_map.count(gene->innovation)){
            sum += std::abs(gene->weight - other.innovation_gene_map.at(gene->innovation)->weight);
            coincident += 1;
        }
    }

    return sum / coincident;
}

// sameSpecies
bool Genotype::Genome::same_specie(const Genotype::Genome& other) const
{
    return (this->d_disjoint * this->delta_disjoints(other)) + (this->d_weight * this->delta_weights(other))
           < this->d_threshold;
}

void Genotype::Genome::crossover(const Genotype::Genome& other, Genotype::Genome& child) const
{
    const Genotype::Genome* p1_genome = this;
    const Genotype::Genome* p2_genome = &other;

    if(p2_genome->fitness < p1_genome->fitness){
        std::swap(p1_genome, p2_genome);
    }

    child.max_neuron = std::max(p1_genome->max_neuron, p2_genome->max_neuron);
    for(const auto& mutation_rate : p1_genome->mutation_rates){
        child.mutation_rates[mutation_rate.first] = mutation_rate.second;
    }

    for(const auto& gene : p1_genome->genes){
        if(p2_genome->innovation_gene_map.count(gene->innovation) &&
           p2_genome->innovation_gene_map.at(gene->innovation)->enabled &&
           std::rand() % 2){
            child.genes.push_back(
                    std::make_shared<Genotype::Gene>(*(p2_genome->innovation_gene_map.at(gene->innovation))));
        }else{
            child.genes.push_back(std::make_shared<Genotype::Gene>(*gene));
        }

        child.innovation_set.insert(child.genes.back()->innovation);
        child.innovation_gene_map.insert({child.genes.back()->innovation, child.genes.back()});
    }
}

// basicGenome
void Genotype::Genome::simple_genome(size_t& innovation)
{
    this->max_neuron = this->inputs - 1;
    this->mutate(innovation);
}

void Genotype::Genome::save(const std::string& file) const
{
    nlohmann::json j;

    std::vector<std::tuple<size_t, size_t, size_t, float, bool>> j_genes;
    for(const auto& gene : this->genes){
        j_genes.emplace_back(gene->into, gene->out, gene->innovation, gene->weight, gene->enabled);
    }

    j["max_neuron"] = this->max_neuron;
    j["mutation_rates"] = this->mutation_rates;
    j["genes"] = j_genes;

    std::ofstream os("../../log/saves/" + file + ".json");
    os << std::setw(4) << j << std::endl;
}

void Genotype::Genome::load(const std::string& file)
{
    nlohmann::json j;

    std::ifstream is("../../log/saves/" + file + ".json");
    is >> j;

    *this = Genotype::Genome();

    this->max_neuron = j["max_neuron"].get<size_t>();

    for(auto& mutation_rate : this->mutation_rates){
        mutation_rate.second = j["mutation_rates"].at(mutation_rate.first).get<float>();
    }

    for (const auto& j_gene : j["genes"]) {
        this->genes.push_back(std::make_shared<Genotype::Gene>(
            j_gene[0].get<size_t>(),
            j_gene[1].get<size_t>(),
            j_gene[2].get<size_t>(),
            j_gene[3].get<float>(),
            j_gene[4].get<bool>()
        ));

        this->innovation_set.insert(this->genes.back()->innovation);
        this->innovation_gene_map.insert({this->genes.back()->innovation, this->genes.back()});
    }
}

float Genotype::Genome::get_fitness() const { return this->fitness; }
size_t Genotype::Genome::get_global_rank() const { return this->global_rank; }

void Genotype::Genome::set_fitness(float fitness) { this->fitness = fitness; }
void Genotype::Genome::set_global_rank(size_t global_rank) { this->global_rank = global_rank; }
