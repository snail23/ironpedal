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

            this->depth.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->frequency.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 1.0f, 2880.0f, daisy::Parameter::LINEAR);
            this->shape.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], daisysp::Oscillator::WAVE_SIN, daisysp::Oscillator::WAVE_SQUARE, daisy::Parameter::LINEAR);

            this->tremolo.Init(this->ironpedal->AudioSampleRate());

            this->tremolo.SetDepth(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_1]);
            this->tremolo.SetFreq(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_3]);
            this->tremolo.SetWaveform(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "DEPTH", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "FREQ\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_1] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu HZ\n", (uint32_t)this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_3]);
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n");
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "SHAPE\n", ALIGN_CENTER);

            switch ((uint32_t)this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_5])
            {
            case daisysp::Oscillator::WAVE_RAMP:
                SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "RAMP WAVE\n", ALIGN_CENTER);

                break;

            case daisysp::Oscillator::WAVE_SAW:
                SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "SAW WAVE\n", ALIGN_CENTER);

                break;

            case daisysp::Oscillator::WAVE_SIN:
                SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "SINE WAVE\n", ALIGN_CENTER);

                break;

            case daisysp::Oscillator::WAVE_SQUARE:
                SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "SQUARE WAVE\n", ALIGN_CENTER);

                break;

            case daisysp::Oscillator::WAVE_TRI:
                SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "TRIANGLE WAVE\n", ALIGN_CENTER);

                break;
            }

            this->ironpedal->PrintFooter("TREMOLO\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->tremolo.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_TREMOLO).locked)
            {
                this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_1] = this->depth.Process();
                this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_3] = this->frequency.Process();
                this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_5] = this->shape.Process();
            }

            this->tremolo.SetDepth(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_1]);
            this->tremolo.SetFreq(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_3]);
            this->tremolo.SetWaveform(this->ironpedal->GetEffect(EFFECT_TREMOLO).values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter depth;
        daisy::Parameter frequency;
        daisy::Parameter shape;

        daisysp::Tremolo tremolo;
        Snailsoft::Ironpedal *ironpedal;
    };
}
