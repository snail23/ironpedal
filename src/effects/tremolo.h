// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Tremolo
    {
    public:
        Tremolo(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->depth.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->frequency.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 1.0f, 2880.0f, daisy::Parameter::LINEAR);
            this->shape.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], daisysp::Oscillator::WAVE_SIN, daisysp::Oscillator::WAVE_SQUARE, daisy::Parameter::LINEAR);

            this->tremolo.Init(this->ironpedal->AudioSampleRate());

            this->tremolo.SetDepth(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_1]);
            this->tremolo.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_3]);
            this->tremolo.SetWaveform(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            PrintlnCentered(this->ironpedal, "DEPTH     FREQ", COLOR_LIGHT);
            sprintf(buf, "%3lu     %3lu HZ", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_1] * 100.0f), (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_3]);
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "SHAPE", COLOR_LIGHT);

            switch ((uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_5])
            {
            case daisysp::Oscillator::WAVE_RAMP:
                PrintlnCentered(this->ironpedal, "RAMP WAVE", COLOR);

                break;

            case daisysp::Oscillator::WAVE_SAW:
                PrintlnCentered(this->ironpedal, "SAW WAVE", COLOR);

                break;

            case daisysp::Oscillator::WAVE_SIN:
                PrintlnCentered(this->ironpedal, "SINE WAVE", COLOR);

                break;

            case daisysp::Oscillator::WAVE_SQUARE:
                PrintlnCentered(this->ironpedal, "SQUARE WAVE", COLOR);

                break;

            case daisysp::Oscillator::WAVE_TRI:
                PrintlnCentered(this->ironpedal, "TRIANGLE WAVE", COLOR);

                break;
            }

            PrintFooter(this->ironpedal, "TREMOLO");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
                out[i] = this->tremolo.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_1] = this->depth.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_3] = this->frequency.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_5] = this->shape.Process();
            }

            this->tremolo.SetDepth(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_1]);
            this->tremolo.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_3]);
            this->tremolo.SetWaveform(this->ironpedal->storage->GetSettings().effects[EFFECT_TREMOLO].values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter depth;
        daisy::Parameter frequency;
        daisy::Parameter shape;

        daisysp::Tremolo tremolo;
        Snailsoft::Ironpedal *ironpedal;
    };
}
