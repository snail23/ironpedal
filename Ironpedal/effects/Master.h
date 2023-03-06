// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Master
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "HIGH LOW  GAIN");

            auto low = this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2] / 1000.0f;

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)((this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_3] - 1.0f) * 100.0f));
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "HIGH LOW   VOL");

            low = this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5] / 1000.0f;

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_6] * 100.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "MASTER");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->postHigh.Init(this->pedal->controls[KNOB_4], 20.0f, 160.0f, Parameter::LINEAR);
            this->postLow.Init(this->pedal->controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

            this->preGain.Init(this->pedal->controls[KNOB_3], 0.0f, 2.0f, Parameter::LINEAR);
            this->preHigh.Init(this->pedal->controls[KNOB_1], 20.0f, 160.0f, Parameter::LINEAR);
            this->preLow.Init(this->pedal->controls[KNOB_2], 1280.0f, 10200.0f, Parameter::LINEAR);

            this->volume.Init(this->pedal->controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);

            this->postHpf.Init(this->pedal->AudioSampleRate());
            this->postHpf.SetDrive(0.0f);
            this->postHpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
            this->postHpf.SetRes(0.0f);

            this->postLpf.Init(this->pedal->AudioSampleRate());
            this->postLpf.SetDrive(0.0f);
            this->postLpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5]);
            this->postLpf.SetRes(0.0f);

            this->preHpf.Init(this->pedal->AudioSampleRate());
            this->preHpf.SetDrive(0.0f);
            this->preHpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1]);
            this->preHpf.SetRes(0.0f);

            this->preLpf.Init(this->pedal->AudioSampleRate());
            this->preLpf.SetDrive(0.0f);
            this->preLpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2]);
            this->preLpf.SetRes(0.0f);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
            {
                this->preHpf.Process(in[i]);
                this->preLpf.Process(this->preHpf.High());

                out[i] = this->preLpf.Low() * this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_3];
            }
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_MASTER].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1] = this->preHigh.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2] = this->preLow.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_3] = this->preGain.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4] = this->postHigh.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5] = this->postLow.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_6] = this->volume.Process();
            }

            this->postHpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
            this->postLpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5]);

            this->preHpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1]);
            this->preLpf.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] *= this->pedal->storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_6];
        }

    private:
        Ironpedal *pedal;

        Parameter postHigh;
        Parameter postLow;

        Parameter preGain;
        Parameter preHigh;
        Parameter preLow;

        Parameter volume;

        Svf postHpf;
        Svf postLpf;

        Svf preHpf;
        Svf preLpf;
    };
}
