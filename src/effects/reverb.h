// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Reverb
    {
    public:
        Reverb(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->feedback.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->lpf.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 1280.0f, 10200.0f, daisy::Parameter::LINEAR);

            this->reverb.Init(this->ironpedal->AudioSampleRate());

            this->reverb.SetFeedback(this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_2]);
            this->reverb.SetLpFreq(this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "FEEDBACK\n", ALIGN_CENTER);
            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_2] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "LOW PASS\n", ALIGN_CENTER);
            float low = this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_5] / 1000.0f;
            sprintf(buf, "%lu.%lu KHZ\n", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            this->ironpedal->PrintFooter("REVERB\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            float t;

            for (size_t i = 0; i < size; ++i)
            {
                t = in[i] * (1.0f - this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_2]);
                this->reverb.Process(t, &out[i]);
                out[i] += t;
            }
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_REVERB).locked)
            {
                this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_2] = this->feedback.Process();
                this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_5] = this->lpf.Process();
            }

            this->reverb.SetFeedback(this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_2]);
            this->reverb.SetLpFreq(this->ironpedal->GetEffect(EFFECT_REVERB).values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter feedback;
        daisy::Parameter lpf;

        daisysp::ReverbSc reverb;
        Snailsoft::Ironpedal *ironpedal;
    };
}
