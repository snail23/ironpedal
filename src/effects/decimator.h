// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Decimator
    {
    public:
        Decimator(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->crush.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->downsample.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->mode.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.0f, 1.01f, daisy::Parameter::LINEAR);

            this->decimator.SetBitcrushFactor(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_5]);
            this->decimator.SetDownsampleFactor(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_1]);
            this->decimator.SetSmoothCrushing(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_3] > 0.5f);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "DOWNSAMPLE", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "MODE\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_1] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_LEFT);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_3] > 0.5f ? "SOFT\n" : "HARD\n", ALIGN_RIGHT);
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n");

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "CRUSH\n", ALIGN_CENTER);
            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_5] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_CENTER);

            this->ironpedal->PrintFooter("DECIMATOR\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->decimator.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_DECIMATOR).locked)
            {
                this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_1] = this->downsample.Process();
                this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_3] = this->mode.Process();
                this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_5] = this->crush.Process();
            }

            this->decimator.SetBitcrushFactor(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_5]);
            this->decimator.SetDownsampleFactor(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_1]);
            this->decimator.SetSmoothCrushing(this->ironpedal->GetEffect(EFFECT_DECIMATOR).values[PedalPCB::KNOB_3] > 0.5f);
        }

    private:
        daisy::Parameter crush;
        daisy::Parameter downsample;
        daisy::Parameter mode;

        daisysp::Decimator decimator;
        Snailsoft::Ironpedal *ironpedal;
    };
}
