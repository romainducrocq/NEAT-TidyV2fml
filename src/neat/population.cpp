#include "neat/population.hpp"

// newSpecies
Population::Specie::Specie()
{
}

// calculateAverageFitness
void Population::Specie::calc_avg_fitness()
{
    this->avg_fitness = 0.f;
    for(const auto& genome : this->genomes){
        this->avg_fitness += genome.get_global_rank();
    }

    this->avg_fitness /= this->genomes.size();
}

// breedChild
void Population::Specie::breed_child(Genotype::Genome& child, size_t& innovation) const
{
    if((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) < this->crossover_prob){
        this->genomes[std::rand() % this->genomes.size()]
            .crossover(this->genomes[std::rand() % this->genomes.size()], child);
    }else{
        child.copy_genome(this->genomes[std::rand() % this->genomes.size()]);
    }

    child.mutate(innovation);
}

float Population::Specie::get_avg_fitness() const { return this->avg_fitness; }
size_t Population::Specie::get_staleness() const { return this->staleness; }
float Population::Specie::get_max_fitness() const { return this->max_fitness; }

void Population::Specie::set_staleness(size_t staleness) { this->staleness = staleness; }
void Population::Specie::set_max_fitness(float max_fitness) { this->max_fitness = max_fitness; }

std::vector<Genotype::Genome>& Population::Specie::self_genomes() { return this->genomes; }

// newPool
Population::Pool::Pool()
{
}

// totalAverageFitness
void Population::Pool::calc_tot_avg_fitness()
{
    this->tot_avg_fitness = 0.f;
    for(const auto& specie : this->species){
        this->tot_avg_fitness += specie.get_avg_fitness();
    }
}

// rankGlobally
void Population::Pool::calc_glob_rank()
{
    std::vector<std::reference_wrapper<Genotype::Genome>> glob_genomes;
    for(auto& specie : this->species){
        for(auto& genome : specie.self_genomes()){
            glob_genomes.push_back(std::ref(genome));
        }
    }

    std::sort(glob_genomes.begin(), glob_genomes.end(), [](const auto& a, const auto& b) {
        return a.get().get_fitness() < b.get().get_fitness();
    });

    for(size_t i = 0; i < glob_genomes.size(); i++){
        glob_genomes[i].get().set_global_rank(i);
    }
}

// cullSpecies
void Population::Pool::cull_species(bool half)
{
    size_t threshold = 1;

    for(auto& specie : this->species){
        std::sort(specie.self_genomes().begin(), specie.self_genomes().end(), [](const auto& a, const auto& b) {
            return a.get_fitness() > b.get_fitness();
        });

        if(half){
            threshold = static_cast<size_t>(std::ceil(specie.self_genomes().size() / 2.f));
        }

        while(specie.self_genomes().size() > threshold){
            specie.self_genomes().pop_back();
        }
    }
}

// removeStaleSpecies
void Population::Pool::pop_stale_species()
{
    for(size_t i = this->species.size(); i-- > 0 ;){

        if(!this->species[i].self_genomes().empty() &&
           this->species[i].self_genomes()[0].get_fitness() > this->species[i].get_max_fitness()){
            this->species[i].set_max_fitness(this->species[i].self_genomes()[0].get_fitness());
            this->species[i].set_staleness(0);
        }else{
            this->species[i].set_staleness(this->species[i].get_staleness() + 1);
        }

        if(this->species[i].get_staleness() >= this->stale_species &&
           this->species[i].get_max_fitness() < this->max_fitness){
            std::swap(this->species[i], this->species.back());
            this->species.pop_back();
        }
    }
}

// removeWeakSpecies
void Population::Pool::pop_weak_species()
{
    this->calc_glob_rank();

    for(auto& specie : this->species){
        specie.calc_avg_fitness();
    }

    this->calc_tot_avg_fitness();

    for(size_t i = this->species.size(); i-- > 0 ;){
        if(static_cast<size_t>(this->species[i].get_avg_fitness() / this->tot_avg_fitness * this->population_size) < 1){
            std::swap(this->species[i], this->species.back());
            this->species.pop_back();
        }
    }

    this->calc_tot_avg_fitness();
}

// addToSpecies
void Population::Pool::add_to_species(Genotype::Genome& child){
    for(auto& specie : this->species){
        if(!specie.self_genomes().empty() && specie.self_genomes()[0].same_specie(child)){
            specie.self_genomes().emplace_back(std::move(child));
            return;
        }
    }

    this->species.emplace_back();
    this->species.back().self_genomes().emplace_back(std::move(child));
}

// newGeneration
void Population::Pool::new_generation()
{
    this->cull_species(true);
    this->pop_stale_species();
    this->pop_weak_species();

    if(this->max_population_size && this->population_gens_inc && this->population_inc_freq &&
       this->generation > 0 && (this->generation+1) % this->population_inc_freq == 0){
        this->population_size = static_cast<size_t>(this->interp_func(
            this->generation+1, 0.f, this->population_gens_inc, this->min_population_size, this->max_population_size));
    }

    std::vector<Genotype::Genome> children;
    for(const auto& specie : this->species){
        for(size_t i = 0;
            i < static_cast<size_t>(specie.get_avg_fitness() / this->tot_avg_fitness * this->population_size) - 1; i++){
            children.emplace_back();
            specie.breed_child(children.back(), this->innovation);
        }
    }

    this->cull_species(false);

    while(children.size() + this->species.size() < this->population_size){
        children.emplace_back();
        this->species[std::rand() % this->species.size()].breed_child(children.back(), this->innovation);
    }

    for(auto& child : children){
        this->add_to_species(child);
    }

    this->generation++;
}

// initializePool
void Population::Pool::init()
{
    *this = Population::Pool();

    for(size_t i = 0; i < this->population_size; i++){
        Genotype::Genome starter;
        starter.simple_genome(this->innovation);
        this->add_to_species(starter);
    }
}

// fitnessAlreadyMeasured
bool Population::Pool::fitness_pass()
{
    return this->species[this->curr_specie].self_genomes()[this->curr_genome].get_fitness() != 0.f;
}

// nextGenome
void Population::Pool::next_genome(Genotype::Genome& best)
{
    this->curr_genome++;

    if(this->curr_genome >= this->species[this->curr_specie].self_genomes().size()){
        this->curr_genome = 0;
        this->curr_specie++;

        if(this->curr_specie >= this->species.size()){
            this->curr_specie = 0;

            this->copy_best_genome(best);

            this->new_generation();
        }
    }
}

void Population::Pool::copy_best_genome(Genotype::Genome& best)
{
    size_t best_specie = 0;
    size_t best_genome = 0;

    [&] {
        for(size_t s = 0; s < this->species.size(); s++){
            for(size_t g = 0; g < this->species[s].self_genomes().size(); g++){
                best_specie = s;
                best_genome = g;
                return;
            }
        }
    }();

    for(size_t s = 0; s < this->species.size(); s++){
        for(size_t g = 0; g < this->species[s].self_genomes().size(); g++){
            if(this->species[s].self_genomes()[g].get_fitness() >
               this->species[best_specie].self_genomes()[best_genome].get_fitness()){
                best_specie = s;
                best_genome = g;
            }
        }
    }

    best.copy_genome(this->species[best_specie].self_genomes()[best_genome]);
}

// evaluateCurrent
void Population::Pool::eval_curr_genome(const std::vector<float>& obs, std::vector<float>& act)
{
    this->species[this->curr_specie].self_genomes()[this->curr_genome].eval_network(obs, act);
}

size_t Population::Pool::get_generation() const { return this->generation; }
float Population::Pool::get_max_fitness() const { return this->max_fitness; }

void Population::Pool::set_max_fitness(float max_fitness) { this->max_fitness = max_fitness; }

Genotype::Genome& Population::Pool::self_curr_genome()
{ return this->species[this->curr_specie].self_genomes()[this->curr_genome]; }
