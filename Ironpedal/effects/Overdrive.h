// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Overdrive
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "BLEND");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%u", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] * 100.0f));
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "DRIVE");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%u", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] * 100.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "OVERDRIVE");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->blend.Init(this->pedal->controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
            this->drive.Init(this->pedal->controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

            this->overdrive.SetDrive(this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = in[i] * this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] + this->overdrive.Process(in[i]) * (1.0f - this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2]);
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] = this->blend.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] = this->drive.Process();
            }

            this->overdrive.SetDrive(this->pedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
        }

    private:
        daisysp::Overdrive overdrive;
        Ironpedal *pedal;

        Parameter blend;
        Parameter drive;
    };
}
