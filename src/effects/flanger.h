// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Flanger
    {
    public:
        Flanger(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->delay.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->depth.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->feedback.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->rate.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.01f, 6.0f, daisy::Parameter::LINEAR);

            this->flanger.Init(this->ironpedal->AudioSampleRate());

            this->flanger.SetDelay(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_1]);
            this->flanger.SetFeedback(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_4]);
            this->flanger.SetLfoDepth(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_6]);
            this->flanger.SetLfoFreq(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_3]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "DELAY", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "RATE\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_1] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu.%02lu\n", (uint32_t)this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_3], (uint32_t)((this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_3] - floor(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_3])) * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n", COLOR);

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "FEEDBACK", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "DEPTH\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_4] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("FLANGER\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->flanger.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_FLANGER).locked)
            {
                this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_1] = this->delay.Process();
                this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_3] = this->rate.Process();
                this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_4] = this->feedback.Process();
                this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_6] = this->depth.Process();
            }

            this->flanger.SetDelay(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_1]);
            this->flanger.SetFeedback(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_4]);
            this->flanger.SetLfoDepth(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_6]);
            this->flanger.SetLfoFreq(this->ironpedal->GetEffect(EFFECT_FLANGER).values[PedalPCB::KNOB_3]);
        }

    private:
        daisy::Parameter delay;
        daisy::Parameter depth;
        daisy::Parameter feedback;
        daisy::Parameter rate;

        daisysp::Flanger flanger;
        Snailsoft::Ironpedal *ironpedal;
    };
}
