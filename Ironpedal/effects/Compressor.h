// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Compressor
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "THRESH   RATIO");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3d dB    %2u:1", (int32_t)this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1], (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "ATTACK RELEASE");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%2u.%03u  %2u.%03u", (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4], (uint32_t)((this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4] - floor(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4])) * 1000.0f), (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6], (uint32_t)((this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6] - floor(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6])) * 1000.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "COMPRESSOR");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->attack.Init(this->pedal->controls[KNOB_4], 0.001f, 10.0f, Parameter::LINEAR);
            this->ratio.Init(this->pedal->controls[KNOB_3], 1.0f, 40.0f, Parameter::LINEAR);
            this->release.Init(this->pedal->controls[KNOB_6], 0.001f, 10.0f, Parameter::LINEAR);
            this->threshold.Init(this->pedal->controls[KNOB_1], 0.0f, -80.0f, Parameter::LINEAR);

            this->compressor.Init(this->pedal->AudioSampleRate());

            this->compressor.SetAttack(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4]);
            this->compressor.SetRatio(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
            this->compressor.SetRelease(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6]);
            this->compressor.SetThreshold(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = this->compressor.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1] = this->threshold.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3] = this->ratio.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4] = this->attack.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6] = this->release.Process();
            }

            this->compressor.SetAttack(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4]);
            this->compressor.SetRatio(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
            this->compressor.SetRelease(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6]);
            this->compressor.SetThreshold(this->pedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1]);
        }

    private:
        daisysp::Compressor compressor;
        Ironpedal *pedal;

        Parameter attack;
        Parameter ratio;
        Parameter release;
        Parameter threshold;
    };
}
