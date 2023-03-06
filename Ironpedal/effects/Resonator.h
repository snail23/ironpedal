// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Resonator
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "DECAY     FREQ");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u     %3u HZ", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1] * 100.0f), (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "STIFF   BRIGHT");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u        %3u", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4] * 100.0f), (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6] * 100.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "RESONATOR");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->brightness.Init(this->pedal->controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
            this->decay.Init(this->pedal->controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
            this->frequency.Init(this->pedal->controls[KNOB_3], 65.0f, 880.0f, Parameter::LINEAR);
            this->stiffness.Init(this->pedal->controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);

            this->resonator.Init(0.0f, 24, this->pedal->AudioSampleRate());

            this->resonator.SetBrightness(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6]);
            this->resonator.SetDamping(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1]);
            this->resonator.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
            this->resonator.SetStructure(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = this->resonator.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1] = this->decay.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3] = this->frequency.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4] = this->stiffness.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6] = this->brightness.Process();
            }

            this->resonator.SetBrightness(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6]);
            this->resonator.SetDamping(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1]);
            this->resonator.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
            this->resonator.SetStructure(this->pedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4]);
        }

    private:
        daisysp::Resonator resonator;
        Ironpedal *pedal;

        Parameter brightness;
        Parameter decay;
        Parameter frequency;
        Parameter stiffness;
    };
}
