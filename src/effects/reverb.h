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

            this->reverb.SetFeedback(this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_2]);
            this->reverb.SetLpFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            PrintlnCentered(this->ironpedal, "FEEDBACK", COLOR_LIGHT);
            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_2] * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "LOW PASS", COLOR_LIGHT);
            auto lpf = this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_5] / 1000.0f;
            sprintf(buf, "%lu.%lu KHZ", (uint32_t)lpf, (uint32_t)((lpf - floor(lpf)) * 10.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);

            PrintFooter(this->ironpedal, "REVERB");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            float t;

            for (auto i = 0u; i < size; ++i)
            {
                t = in[i] * (1.0f - this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_2]);
                this->reverb.Process(t, &out[i]);
                out[i] += t;
            }
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_2] = this->feedback.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_5] = this->lpf.Process();
            }

            this->reverb.SetFeedback(this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_2]);
            this->reverb.SetLpFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_REVERB].values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter feedback;
        daisy::Parameter lpf;

        daisysp::ReverbSc reverb;
        Snailsoft::Ironpedal *ironpedal;
    };
}
