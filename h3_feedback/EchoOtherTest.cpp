// EchoOtherTest.cpp
// -- a more extensive test of the Echo class
// cs 246 10/14

#include <fstream>
#include <cmath>
#include "Echo.h"
using namespace std;


float envelope(float x) {
  return (x < 0.5f) ? 2*x : 2*(1-x);
}


float signal(float t) {
  const float pi = 4.0f*atan(1.0f),
              frequency = 150.0f,
              multiplier = 0.5f*pi,
              modulation = 1.3f,
              duration = 0.05f,
              iduration = 1.0f/duration;
  if (t > duration)
    return 0;
  float theta = 2*pi*frequency*t;
  return envelope(t*iduration)
         * sin(theta + modulation*sin(multiplier*theta));
}


int main(void) {

  const unsigned rate = 22050,
                 count = 6*rate;
  const float MAX = float((1<<15)-1),
              irate = 1.0f/rate;
  short *samples = new short[count];

  // single echo #1
  unsigned offset = 0;
  Echo echo(2,rate);
  echo.setDelay(0.2f);
  echo.setMix(0.5f);
  echo.setFeedback(0.0f);
  for (unsigned i=0; i < rate; ++i) {
    float y = 0.8f*MAX*signal(i*irate);
    y = echo(y);
    samples[offset*rate+i] = short(y);
  }

  // single echo #2
  offset = 1;
  echo.setDelay(0.06f);
  echo.setMix(0.75f);
  for (unsigned i=0; i < rate; ++i) {
    float y = samples[(offset-1)*rate+i];
    y = echo(y);
    samples[offset*rate+i] = short(y);
  }

  // feedback only echo #1
  offset = 2;
  echo.setDelay(0.15f);
  echo.setMix(1);
  echo.setFeedback(0.6f);
  for (unsigned i=0; i < rate; ++i) {
    float y = 0.8f*MAX*signal(i*irate);
    y = echo(y);
    samples[offset*rate+i] = short(y);
  }

  // feedback only echo #2
  offset = 3;
  echo.setDelay(0.04f);
  echo.setFeedback(0.75f);
  for (unsigned i=0; i < rate; ++i) {
    float y = samples[(offset-1)*rate+i];
    y = echo(y);
    samples[offset*rate+i] = short(y);
  }

  // combined echo
  offset = 4;
  Echo echo2(3);
  echo2.setDelay(0.2f);
  echo2.setMix(0.4f);
  echo2.setFeedback(0.5f);
  for (unsigned i=0; i < rate; ++i) {
    float y = 0.7f*samples[(offset-1)*rate+i];
    y = echo2(y);
    samples[offset*rate+i] = short(y);
  }
  offset = 5;
  for (unsigned i=0; i < rate; ++i)
    samples[offset*rate+i] = short(echo2(0));

  // write output file
  struct {
    char riff_chunk[4];
    unsigned chunk_size;
    char wave_fmt[4];
    char fmt_chunk[4];
    unsigned fmt_chunk_size;
    unsigned short audio_format;
    unsigned short number_of_channels;
    unsigned sampling_rate;
    unsigned bytes_per_second;
    unsigned short block_align;
    unsigned short bits_per_sample;
    char data_chunk[4];
    unsigned data_chunk_size;
  }
  header = { {'R','I','F','F'},
             36 + 2*count,
             {'W','A','V','E'},
             {'f','m','t',' '},
             16,1,1,rate,2*rate,2,16,
             {'d','a','t','a'},
             2*count
           };
  fstream out("EchoOtherTest.wav",ios_base::binary|ios_base::out);
  out.write(reinterpret_cast<char*>(&header),44);
  out.write(reinterpret_cast<char*>(samples),2*count);

  delete[] samples;
  return 0;
}

