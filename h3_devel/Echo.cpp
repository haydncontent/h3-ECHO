// Haydn Content
// CS 246
// Fall 2026
// Assignment 3 - Echo

#include <cmath>

#include "Echo.h"

using namespace std;

// class Echo implementation //
// private:
//   int          max_samples;
//   RingBuffer   xvalues, yvalues;
//   float        rate, mix, feedback, offset;

Echo::Echo(float Tmax, float R) // float Tmax: max delay (seconds), float R: sampling rate
    : max_samples(static_cast<int>(Tmax*R))
    , xvalues(max_samples), yvalues(max_samples)
    , rate(R), mix(1.f), feedback(1.f), offset(0.f)
{}

void Echo::setDelay(float t) { offset = rate*t; } // t (seconds) assumed between 0 and Tmax 
void Echo::setMix(float a) { mix = a; }
void Echo::setFeedback(float b) { feedback = b; }

float Echo::operator()(float x) // y_n = x_n + (a - b)x_{n-k} + (b)y_{n-k}
{ 
    int k = floor(offset);

    float xnk = xvalues.get(k);
    float ynk = yvalues.get(k);

    float y = x + ((mix-feedback)*xnk) + (feedback*ynk);

    xvalues.put(x);
    yvalues.put(y);

    return y;
}


// float Echo::operator()(float x)
// {
//     return x + (mix-feedback)*interpolate(offset, xvalues) + (feedback)*interpolate(offset, yvalues);
// }

// float interpolate(float offset, RingBuffer & x)
// {
//     float f = floor(offset); 
//     int fi = static_cast<int>(f);

//     float m = offset-f;
//     float diff = x.get(0) - x.get(-1);

//     return f + m * diff;
// }


// end class Echo implementation //