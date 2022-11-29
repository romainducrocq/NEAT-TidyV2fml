#include "train.hpp"

void App::Train::run()
{
    Timer timer;

    std::cout << "\n";
    std::cout << "-------------------------------TRAIN-------------------------------" << "\n";
    std::cout << "\n";

    std::thread th([&]() {
        this->setup();

        while(this->loop()){}
    });

    std::thread th2([&]() {
        if(! CONF::LOG_SAV.empty()) {
            App::Eval::EVAL().run();
        }
    });

    if(th.joinable()){
        th.join();
    }

    if(th2.joinable()){
        th2.join();
    }
}

void App::Train::setup()
{
    this->env.init(CONF::Mode::TRAIN);
}

bool App::Train::loop()
{
    return this->env.train();
}
