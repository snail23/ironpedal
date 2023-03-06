// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Tremolo
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "DEPTH     FREQ");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u     %3u HZ", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1] * 100.0f), (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "SHAPE");

            this->pedal->display->setTextColor(COLOR);

            switch ((uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5])
            {
            case Oscillator::WAVE_RAMP:
                PrintlnCentered(this->pedal, "RAMP WAVE");
                break;

            case Oscillator::WAVE_SAW:
                PrintlnCentered(this->pedal, "SAW WAVE");
                break;

            case Oscillator::WAVE_SIN:
                PrintlnCentered(this->pedal, "SINE WAVE");
                break;

            case Oscillator::WAVE_SQUARE:
                PrintlnCentered(this->pedal, "SQUARE WAVE");
                break;

            case Oscillator::WAVE_TRI:
                PrintlnCentered(this->pedal, "TRIANGLE WAVE");
                break;
            }

            PrintFooter(this->pedal, "TREMOLO");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->depth.Init(this->pedal->controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
            this->frequency.Init(this->pedal->controls[KNOB_3], 1.0f, 2880.0f, Parameter::LINEAR);
            this->shape.Init(this->pedal->controls[KNOB_5], Oscillator::WAVE_SIN, Oscillator::WAVE_SQUARE, Parameter::LINEAR);

            this->tremolo.Init(this->pedal->AudioSampleRate());

            this->tremolo.SetDepth(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1]);
            this->tremolo.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
            this->tremolo.SetWaveform(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = this->tremolo.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1] = this->depth.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3] = this->frequency.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5] = this->shape.Process();
            }

            this->tremolo.SetDepth(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1]);
            this->tremolo.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
            this->tremolo.SetWaveform(this->pedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]);
        }

    private:
        daisysp::Tremolo tremolo;
        Ironpedal *pedal;

        Parameter depth;
        Parameter frequency;
        Parameter shape;
    };
}
