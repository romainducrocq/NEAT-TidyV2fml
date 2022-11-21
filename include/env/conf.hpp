#ifndef _ENV_CONF_HPP
#define _ENV_CONF_HPP

/*** DEF DEFAULT ARGS HERE */
/*
 * TRAIN
   -g 100 -t 1000 -n 0 -p plt -s sav
 * EVAL
   -e 10 -t 0 -n 0 -s sav
 * PLAY
   -e 10
 * TEST

*/

#include <unistd.h>

#include <iostream>
#include <cstddef>
#include <cstring>
#include <cstdlib>

template<typename T>
struct DefaultConf{
    enum Mode{
        TRAIN, EVAL, PLAY, TEST, NONE
    };

    /*** DEC ACTIONS HERE */
    enum Action{
        JUMP, NOOP
    };

    const static size_t INPUTS;
    const static size_t OUTPUTS;

    const static size_t LIM_HIDDEN;

    const static float MUTATE_WEIGHT_RATE;
    const static float MUTATE_GENE_RATE;
    const static float MUTATE_LINK_RATE;
    const static float MUTATE_BIAS_RATE;
    const static float MUTATE_NEURON_RATE;
    const static float MUTATE_ENABLE_RATE;
    const static float MUTATE_DISABLE_RATE;
    const static float MUTATE_OFFSET_SIZE;
    const static float MUTATE_RATE_DECAY;

    const static float DELTA_WEIGHTS;
    const static float DELTA_DISJOINT;
    const static float DELTA_THRESHOLD;

    const static float CROSSOVER_PROB;
    const static size_t STALE_SPECIES;
    const static size_t POPULATION_SIZE;
    const static size_t MAX_POPULATION_SIZE;
    const static size_t POPULATION_GENS_INC;
    const static size_t POPULATION_INC_FREQ;

    const static size_t ACT_REPEAT;
    const static size_t MVG_AVG;
    const static size_t PLT_FREQ;
    const static size_t SAV_FREQ;

    static size_t GENERATIONS_TRAIN;
    static size_t EPOCHS_EVAL;
    static size_t MAX_STEP;
    static size_t MAX_NOOP;

    static std::string LOG_PLT;
    static std::string LOG_SAV;

    static Mode MODE;

    /* SFML */
    const static std::string NAME;
    const static size_t WIN_W;
    const static size_t WIN_H;
    const static size_t FRAMERATE;
    const static size_t ANTIALIAS;
    const static size_t BG_COL[3];

    /*** DEC OPT PARAMS HERE */


    static inline bool argParse(int argc, char** argv)
    {
        // https://github.com/gnif/LookingGlass/blob/c0c63fd93bf999b6601a782fec8b56e9133388cc/client/main.c#L1391

        /*** DEF CMDS HERE */
        const char cmds[] = "h:m:g:e:t:n:p:s:";

        for(;;){
            switch(getopt(argc, argv, cmds)){

                /*** DEF HELP HERE */
                case '?': // help
                case 'h':
                default :
                    std::cerr << "usage: apps/exec [-h] [-m MOD] [-g GEN] [-e EPO] [-t STP] [-n NOP] [-p PLT] [-s SAV] \n";
                    std::cerr << "\n";
                    std::cerr << "NEAT FlappyBaboon                                                                    \n";
                    std::cerr << "\n";
                    std::cerr << "optional args:                                                                       \n";
                    std::cerr << "  -h      Print help and exit                                                        \n";
                    std::cerr << "  -m MOD  Set mode < train | eval | play | test >                                    \n";
                    std::cerr << "  params:                                                                            \n";
                    std::cerr << "  -g GEN  [train]       Set number generation (0=inf)                                \n";
                    std::cerr << "  -e EPO  [eval, play]  Set number epoch      (0=inf)                                \n";
                    std::cerr << "  -t STP  [train, eval] Set number max step   (0=inf)                                \n";
                    std::cerr << "  -n NOP  [train, eval] Set number max noop   (0=inf)                                \n";
                    std::cerr << "  utils:                                                                             \n";
                    std::cerr << "  -p PLT  [train]       Set file name plot plt                                       \n";
                    std::cerr << "  -s SAV  [train, eval] Set file name save sav                                       \n";
                    std::cerr << "  keys:                                                                              \n";
                    std::cerr << "  Up      [play]        Jump                                                         \n";
                    std::cerr << "  Space   [train, eval] Ai view                                                      \n";

                    return false;

                case -1:
                    break;

                case 'm': // mode < train | eval | play | test >
                    if(std::strcmp(optarg, "train") == 0){
                        DefaultConf<T>::MODE = DefaultConf<T>::Mode::TRAIN;
                    }else if(std::strcmp(optarg, "eval") == 0){
                        DefaultConf<T>::MODE = DefaultConf<T>::Mode::EVAL;
                    }else if(std::strcmp(optarg, "play") == 0){
                        DefaultConf<T>::MODE = DefaultConf<T>::Mode::PLAY;
                    }else if(std::strcmp(optarg, "test") == 0){
                        DefaultConf<T>::MODE = DefaultConf<T>::Mode::TEST;
                    }
                continue;

                case 'g': // number generation
                    DefaultConf<T>::GENERATIONS_TRAIN = static_cast<size_t>(std::atoi(optarg));
                    continue;

                case 'e': // number epoch
                    DefaultConf<T>::EPOCHS_EVAL = static_cast<size_t>(std::atoi(optarg));
                    continue;

                case 't': // number max step
                    DefaultConf<T>::MAX_STEP = static_cast<size_t>(std::atoi(optarg));
                    continue;

                case 'n': // number max noop
                    DefaultConf<T>::MAX_NOOP = static_cast<size_t>(std::atoi(optarg));
                    continue;

                case 'p': // file name plot plt
                    DefaultConf<T>::LOG_PLT = std::string(optarg);
                    continue;

                case 's': // file name save sav
                    DefaultConf<T>::LOG_SAV = std::string(optarg);
                    continue;

                /*** DEF OPT CMDS HERE */


            }
            break;
        }

        return true;
    }
};

template<typename T>
typename DefaultConf<T>::Mode DefaultConf<T>::MODE = DefaultConf<T>::Mode::NONE;
template<typename T>
std::string DefaultConf<T>::LOG_PLT = "";
template<typename T>
std::string DefaultConf<T>::LOG_SAV = "";

/*** DEF PARAMS HERE */
template<typename T>
const size_t DefaultConf<T>::INPUTS = 3;
template<typename T>
const size_t DefaultConf<T>::OUTPUTS = 1;
template<typename T>
const size_t DefaultConf<T>::LIM_HIDDEN = 1000000;
template<typename T>
const float DefaultConf<T>::MUTATE_WEIGHT_RATE = 0.1f;
template<typename T>
const float DefaultConf<T>::MUTATE_GENE_RATE = 0.25f;
template<typename T>
const float DefaultConf<T>::MUTATE_LINK_RATE = 2.f;
template<typename T>
const float DefaultConf<T>::MUTATE_BIAS_RATE = 0.4f;
template<typename T>
const float DefaultConf<T>::MUTATE_NEURON_RATE = 0.5f;
template<typename T>
const float DefaultConf<T>::MUTATE_ENABLE_RATE = 0.2f;
template<typename T>
const float DefaultConf<T>::MUTATE_DISABLE_RATE = 0.4f;
template<typename T>
const float DefaultConf<T>::MUTATE_OFFSET_SIZE = 0.1f;
template<typename T>
const float DefaultConf<T>::MUTATE_RATE_DECAY = 0.f; // (0.f=false) -> no decay, rand perturbs
template<typename T>
const float DefaultConf<T>::DELTA_DISJOINT = 2.f;
template<typename T>
const float DefaultConf<T>::DELTA_WEIGHTS = 0.4f;
template<typename T>
const float DefaultConf<T>::DELTA_THRESHOLD = 1.f;
template<typename T>
const float DefaultConf<T>::CROSSOVER_PROB = 0.75f;
template<typename T>
const size_t DefaultConf<T>::STALE_SPECIES = 15;
template<typename T>
const size_t DefaultConf<T>::POPULATION_SIZE = 150;
template<typename T>
const size_t DefaultConf<T>::MAX_POPULATION_SIZE = 0; // (0=false) -> const population size // &&
template<typename T>
const size_t DefaultConf<T>::POPULATION_GENS_INC = 200; // (0=false) -> const population size // &&
template<typename T>
const size_t DefaultConf<T>::POPULATION_INC_FREQ = 10; // (0=false) -> const population size
template<typename T>
const size_t DefaultConf<T>::ACT_REPEAT = 3;
template<typename T>
const size_t DefaultConf<T>::MVG_AVG = 100;
template<typename T>
const size_t DefaultConf<T>::PLT_FREQ = 0; // (0=false) -> plot at end train only
template<typename T>
const size_t DefaultConf<T>::SAV_FREQ = 1; // (0=false) -> save at end train only

template<typename T>
size_t DefaultConf<T>::GENERATIONS_TRAIN = 100; // (0=inf) -> infinite train
template<typename T>
size_t DefaultConf<T>::EPOCHS_EVAL = 10; // (0=inf) -> infinite eval
template<typename T>
size_t DefaultConf<T>::MAX_STEP = 1000; // (0=inf) -> no step early stop
template<typename T>
size_t DefaultConf<T>::MAX_NOOP = 0; // (0=inf) -> no noop early stop

/* SFML */
template<typename T>
const std::string DefaultConf<T>::NAME = "SFML";
template<typename T>
const size_t DefaultConf<T>::WIN_W = 360;
template<typename T>
const size_t DefaultConf<T>::WIN_H = 640;
template<typename T>
const size_t DefaultConf<T>::FRAMERATE = 60;
template<typename T>
const size_t DefaultConf<T>::ANTIALIAS = 0;
template<typename T>
const size_t DefaultConf<T>::BG_COL[3] = { 51, 51, 51 };

/*** DEF OPT PARAMS HERE */


using CONF = DefaultConf<int>;

#endif
