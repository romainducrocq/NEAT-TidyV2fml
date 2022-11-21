#include <cstdlib>
#include <ctime>

#include <unordered_map>
#include <tuple>
#include <mutex>

#include <gtest/gtest.h>

#include "env/conf.hpp"

#include "train.hpp"
#include "eval.hpp"
#include "play.hpp"

std::unordered_map<std::string, std::mutex> mutex_map;

int main(int argc, char** argv)
{
    if(CONF::argParse(argc, argv)) {

        for(const auto& key : {"rw_sav"}){
            mutex_map.emplace(std::piecewise_construct,
                              std::forward_as_tuple(key),
                              std::forward_as_tuple());
        }

        switch(CONF::MODE){

            case CONF::Mode::TRAIN:{
                std::srand(time(nullptr));
                App::Train::TRAIN();
                return 0;
            }

            case CONF::Mode::EVAL:{
                if(CONF::LOG_SAV.empty()) { return 1; }
                std::srand(time(nullptr));
                App::Eval::EVAL();
                return 0;
            }

            case CONF::Mode::PLAY:{
                std::srand(time(nullptr));
                App::Play::PLAY();
                return 0;
            }

            case CONF::Mode::TEST:{
                std::srand(42);
                testing::InitGoogleTest(&argc, argv);
                return RUN_ALL_TESTS();
            }

            default:
                return 1;
        }

    }

    return 1;
}
