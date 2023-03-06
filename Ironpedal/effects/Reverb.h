// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Reverb
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "FEEDBACK");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%d", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2] * 100.0f));
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "LOW PASS");

            auto low = this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5] / 1000.0f;

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%u.%u KHZ", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "REVERB");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->feedback.Init(this->pedal->controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
            this->low.Init(this->pedal->controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

            this->reverb.Init(this->pedal->AudioSampleRate());

            this->reverb.SetFeedback(this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
            this->reverb.SetLpFreq(this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            float t;

            for (auto i = 0; i < size; ++i)
            {
                t = in[i] * (1.0f - this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
                this->reverb.Process(t, &out[i]);
                out[i] += t;
            }
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_REVERB].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2] = this->feedback.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5] = this->low.Process();
            }

            this->reverb.SetFeedback(this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
            this->reverb.SetLpFreq(this->pedal->storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5]);
        }

    private:
        daisysp::ReverbSc reverb;
        Ironpedal *pedal;

        Parameter feedback;
        Parameter low;
    };
}
