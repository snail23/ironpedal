// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Chorus
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "DELAY     RATE");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u       %u.%02u", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] * 100.0f), (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3], (uint32_t)((this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] - floor(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3])) * 100.0f));
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "FEEDBACK DEPTH");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u        %3d", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] * 100.0f), (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] * 100.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "CHORUS");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->delay.Init(this->pedal->controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
            this->depth.Init(this->pedal->controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
            this->feedback.Init(this->pedal->controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);
            this->rate.Init(this->pedal->controls[KNOB_3], 0.01f, 6.0f, Parameter::LINEAR);

            this->chorus.Init(this->pedal->AudioSampleRate());

            this->chorus.SetDelay(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
            this->chorus.SetFeedback(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
            this->chorus.SetLfoDepth(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6]);
            this->chorus.SetLfoFreq(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = this->chorus.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] = this->delay.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] = this->rate.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] = this->feedback.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] = this->depth.Process();
            }

            this->chorus.SetDelay(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
            this->chorus.SetFeedback(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
            this->chorus.SetLfoDepth(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6]);
            this->chorus.SetLfoFreq(this->pedal->storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);
        }

    private:
        daisysp::ChorusEngine chorus;
        Ironpedal *pedal;

        Parameter depth;
        Parameter delay;
        Parameter feedback;
        Parameter rate;
    };
}
