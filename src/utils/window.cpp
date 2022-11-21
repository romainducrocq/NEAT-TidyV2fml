#include "utils/window.hpp"

Window::Window()
    : ev_manager(this->window, true)
{
    sf::ContextSettings options;
    options.antialiasingLevel = this->antialias;

    this->window.create(sf::VideoMode(this->win_w, this->win_h, 32), this->name, sf::Style::Default, options);
    this->window.setVerticalSyncEnabled(false);

    this->window.setFramerateLimit(this->framerate);
}

void Window::app_run()
{
    this->app_setup();
    this->app_loop();
}

void Window::app_setup()
{
    this->ev_setup();
    this->setup();
    this->draw_setup();
}

void Window::app_loop()
{
    while(this->window.isOpen()){
        this->ev_manager.processEvents();

        if(! this->loop()){
            this->window.close();
            break;
        }

        this->window.clear(sf::Color(this->bg_col[0], this->bg_col[1], this->bg_col[2]));
        this->draw_loop();
        this->window.display();
    }
}