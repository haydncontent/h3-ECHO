// Haydn Content
// CS 246
// Fall 2026
// Assignment 3 - Echo

#include <cmath>

#include "Echo.h"

using namespace std;

namespace
{
float interpolate(RingBuffer & buf, float offset)
{
    int k = static_cast<int>(floor(offset));

    float a = buf.get(k);
    float b = buf.get(k+1);

    return a + (offset-k) * (b-a);
}
}

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

    float xnk = interpolate(xvalues, offset); //xvalues.get(k);
    float ynk = interpolate(yvalues, offset); //yvalues.get(k);

    float y = x + ((mix-feedback)*xnk) + (feedback*ynk);

    xvalues.put(x);
    yvalues.put(y);

    return y;
}

// end class Echo implementation //