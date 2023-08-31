#pragma once
#ifndef DSY_DELAY_H
#define DSY_DELAY_H
#include <stdlib.h>
#include <stdint.h>
namespace daisysp
{
/** Simple Delay line.
November 2019

Converted to Template December 2019

declaration example: (1 second of floats)

DelayLine<float, SAMPLE_RATE> del;

By: shensley
*/

class DelayLine
{
  public:
    DelayLine() {}
    ~DelayLine() {}
    /** initializes the delay line by clearing the values within, and setting delay to 1 sample.
    */
    void Init(float *buffer, size_t buffer_size)
    {
        max_size = buffer_size;
        line_ = buffer;

        Reset();
    }
    /** clears buffer, sets write ptr to 0, and delay to 1 sample.
    */
    void Reset()
    {
        for(size_t i = 0; i < max_size; i++)
        {
            line_[i] = 0.0f;
        }
        write_ptr_ = 0;
        delay_     = 1;
    }

    /** sets the delay time in samples
        If a float is passed in, a fractional component will be calculated for interpolating the delay line.
    */
    inline void SetDelay(size_t delay)
    {
        frac_  = 0.0f;
        delay_ = delay < max_size ? delay : max_size - 1;
    }

    /** sets the delay time in samples
        If a float is passed in, a fractional component will be calculated for interpolating the delay line.
    */
    inline void SetDelay(float delay)
    {
        int32_t int_delay = static_cast<int32_t>(delay);
        frac_             = delay - static_cast<float>(int_delay);
        delay_ = static_cast<size_t>(int_delay) < max_size ? int_delay
                                                           : max_size - 1;
    }

    /** writes the sample of type float to the delay line, and advances the write ptr
    */
    inline void Write(const float sample)
    {
        line_[write_ptr_] = sample;
        write_ptr_        = (write_ptr_ - 1 + max_size) % max_size;
    }

    /** returns the next sample of type float in the delay line, interpolated if necessary.
    */
    inline const float Read() const
    {
        float a = line_[(write_ptr_ + delay_) % max_size];
        float b = line_[(write_ptr_ + delay_ + 1) % max_size];
        return a + (b - a) * frac_;
    }

    /** Read from a set location */
    inline const float Read(float delay) const
    {
        int32_t delay_integral   = static_cast<int32_t>(delay);
        float   delay_fractional = delay - static_cast<float>(delay_integral);
        const float a = line_[(write_ptr_ + delay_integral) % max_size];
        const float b = line_[(write_ptr_ + delay_integral + 1) % max_size];
        return a + (b - a) * delay_fractional;
    }

    inline const float ReadHermite(float delay) const
    {
        int32_t delay_integral   = static_cast<int32_t>(delay);
        float   delay_fractional = delay - static_cast<float>(delay_integral);

        int32_t     t     = (write_ptr_ + delay_integral + max_size);
        const float     xm1   = line_[(t - 1) % max_size];
        const float     x0    = line_[(t) % max_size];
        const float     x1    = line_[(t + 1) % max_size];
        const float     x2    = line_[(t + 2) % max_size];
        const float c     = (x1 - xm1) * 0.5f;
        const float v     = x0 - x1;
        const float w     = c + v;
        const float a     = w + v + (x2 - x0) * 0.5f;
        const float b_neg = w + a;
        const float f     = delay_fractional;
        return (((a * f) - b_neg) * f + c) * f + x0;
    }

    inline const float Allpass(const float sample, size_t delay, const float coefficient)
    {
        float read  = line_[(write_ptr_ + delay) % max_size];
        float write = sample + coefficient * read;
        Write(write);
        return -write * coefficient + read;
    }

  private:
    float  frac_;
    size_t write_ptr_;
    size_t delay_;
    size_t max_size;
    float      *line_;
};
} // namespace daisysp
#endif
