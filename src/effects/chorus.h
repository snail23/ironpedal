// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Chorus
    {
    public:
        Chorus(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->delay.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->depth.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->feedback.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->rate.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.01f, 6.0f, daisy::Parameter::LINEAR);

            this->chorus.Init(this->ironpedal->AudioSampleRate());

            this->chorus.SetDelay(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_1]);
            this->chorus.SetFeedback(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_4]);
            this->chorus.SetLfoDepth(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_6]);
            this->chorus.SetLfoFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_3]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "DELAY", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "RATE\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_1] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu.%02lu\n", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_3], (uint32_t)((this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_3] - floor(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_3])) * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n", COLOR);

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "FEEDBACK", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "DEPTH\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_4] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            PrintFooter(this->ironpedal, "CHORUS\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->chorus.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_1] = this->delay.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_3] = this->rate.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_4] = this->feedback.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_6] = this->depth.Process();
            }

            this->chorus.SetDelay(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_1]);
            this->chorus.SetFeedback(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_4]);
            this->chorus.SetLfoDepth(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_6]);
            this->chorus.SetLfoFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_CHORUS].values[PedalPCB::KNOB_3]);
        }

    private:
        daisy::Parameter depth;
        daisy::Parameter delay;
        daisy::Parameter feedback;
        daisy::Parameter rate;

        daisysp::ChorusEngine chorus;
        Snailsoft::Ironpedal *ironpedal;
    };
}
