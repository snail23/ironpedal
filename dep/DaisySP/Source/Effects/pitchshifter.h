#pragma once
#ifndef DSY_PITCHSHIFTER_H
#define DSY_PITCHSHIFTER_H
#include <stdint.h>
#include <cmath>
#ifdef USE_ARM_DSP
#include "arm_math.h"
#endif
#include "Utility/dsp.h"
#include "Utility/delayline.h"
#include "Control/phasor.h"

/** Shift can be 30-100 ms lets just start with 50 for now.
0.050 * SR = 2400 samples (at 48kHz)
*/
#define SHIFT_BUFFER_SIZE 4800
//#define SHIFT_BUFFER_SIZE 4800
//#define SHIFT_BUFFER_SIZE 8192
//#define SHIFT_BUFFER_SIZE 1024

namespace daisysp
{
static inline uint32_t hash_xs32(uint32_t x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

inline uint32_t myrand()
{
    static uint32_t seed = 1;
    seed                 = hash_xs32(seed);
    return seed;
}

/**  time-domain pitchshifter

Author: shensley

Based on "Pitch Shifting" from ucsd.edu 

t = 1 - ((s *f) / R)

where:
    s is the size of the delay
    f is the frequency of the lfo
    r is the sample_rate

solving for t = 12.0
f = (12 - 1) * 48000 / SHIFT_BUFFER_SIZE;

\todo - move hash_xs32 and myrand to dsp.h and give appropriate names
*/
class PitchShifter
{
  public:
    PitchShifter() {}
    ~PitchShifter() {}
    /** Initialize pitch shifter
    */
    void Init(float sr)
    {
        force_recalc_ = false;
        sr_           = sr;
        mod_freq_     = 5.0f;
        SetSemitones();
            gain_ = 0.0f;
            d_.Init();
            phs_.Init(sr, 50, 0);
        shift_up_ = true;
        del_size_ = SHIFT_BUFFER_SIZE;
        SetDelSize(del_size_);
        fun_ = 0.0f;
    }

    /** process pitch shifter
    */
    float Process(float &in)
    {
        float val, fade;
        // First Process delay mod/crossfade
        fade = phs_.Process();
        if(prev_phs > fade)
        {
            mod_amt_ = fun_ * ((float)(myrand() % 255) / 255.0f)
                         * (del_size_ * 0.5f);
            mod_coeff_
                = 0.0002f + (((float)(myrand() % 255) / 255.0f) * 0.001f);
        }
        slewed_mod_ += mod_coeff_ * (mod_amt_ - slewed_mod_);
        prev_phs = fade;
        if(shift_up_)
        {
            fade = 1.0f - fade;
        }
        mod_ = fade * (del_size_ - 1);
#ifdef USE_ARM_DSP
        gain_ = arm_sin_f32(fade * (float)M_PI);
#else
        gain_ = sinf(fade * PI_F);
#endif

        // Handle Delay Writing
        d_.Write(in);
        // Modulate Delay Lines
        //mod_a_amt = mod_b_amt = 0.0f;
        d_.SetDelay(mod_ + mod_amt_);
        d_.SetDelay(mod_ + slewed_mod_);
        val = 0.0f;
        val += (d_.Read() * gain_);
        return val;
    }

    /** sets transposition in semitones
    */
    void SetTransposition(const float &transpose)
    {
        float   ratio;
        uint8_t idx;
        if(transpose_ != transpose || force_recalc_)
        {
            transpose_ = transpose;
            idx        = (uint8_t)fabsf(transpose);
            ratio      = semitone_ratios_[idx % 12];
            ratio *= (uint8_t)(fabsf(transpose) / 12) + 1;
            if(transpose > 0.0f)
            {
                shift_up_ = true;
            }
            else
            {
                shift_up_ = false;
            }
            mod_freq_ = ((ratio - 1.0f) * sr_) / del_size_;
            if(mod_freq_ < 0.0f)
            {
                mod_freq_ = 0.0f;
            }
            phs_.SetFreq(mod_freq_);
            if(force_recalc_)
            {
                force_recalc_ = false;
            }
        }
    }

    /** sets delay size changing the timbre of the pitchshifting 
    */
    void SetDelSize(uint32_t size)
    {
        del_size_     = size < SHIFT_BUFFER_SIZE ? size : SHIFT_BUFFER_SIZE;
        force_recalc_ = true;
        SetTransposition(transpose_);
    }

    /** sets an amount of internal random modulation, kind of sounds like tape-flutter
    */
    inline void SetFun(float f) { fun_ = f; }

  private:
    inline void SetSemitones()
    {
        for(size_t i = 0; i < 12; i++)
        {
            semitone_ratios_[i] = powf(2.0f, (float)i / 12);
        }
    }
    typedef DelayLine<float, SHIFT_BUFFER_SIZE> ShiftDelay;
    ShiftDelay                                  d_;
    float                                       pitch_shift_, mod_freq_;
    uint32_t                                    del_size_;
    /** lfo stuff
*/
    bool   force_recalc_;
    float  sr_;
    bool   shift_up_;
    Phasor phs_;
    float  gain_, mod_, transpose_;
    float  fun_, mod_amt_, prev_phs;
    float  slewed_mod_, mod_coeff_;
    /** pitch stuff
*/
    float semitone_ratios_[12];
};
} // namespace daisysp

#endif
