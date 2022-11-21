#ifndef _UTILS_MVGAVG_HPP
#define _UTILS_MVGAVG_HPP

#include <vector>

struct MovingAverage
{
private:
    size_t n = 0;
    size_t i = 0;
    float t = 0.f;

    float avg = 0.f;

    std::vector<float> xs;

public:
    MovingAverage() {}

    MovingAverage(size_t n)
            : n(n) {}

    void add(float x)
    {
        this->t += x;
        this->i = (this->i + 1) % this->n;
        if(this->xs.size() < this->n){
            this->xs.push_back(x);
        }else{
            this->t -= this->xs[this->i];
            this->xs[this->i] = x;
        }

        this->avg = this->t / this->xs.size();
    }

    float get() const
    {
        return this->avg;
    }

    size_t max() const
    {
        return this->n;
    }
};

#endif