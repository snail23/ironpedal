// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Overdrive
    {
    public:
        Overdrive(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->blend.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->drive.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.0f, daisy::Parameter::LINEAR);

            this->overdrive.SetDrive(this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            PrintlnCentered(this->ironpedal, "BLEND", COLOR_LIGHT);
            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_2] * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "DRIVE", COLOR_LIGHT);
            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_5] * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);

            PrintFooter(this->ironpedal, "OVERDRIVE");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
                out[i] = in[i] * this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_2] + this->overdrive.Process(in[i]) * (1.0f - this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_2]);
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_2] = this->blend.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_5] = this->drive.Process();
            }

            this->overdrive.SetDrive(this->ironpedal->storage->GetSettings().effects[EFFECT_OVERDRIVE].values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter blend;
        daisy::Parameter drive;

        daisysp::Overdrive overdrive;
        Snailsoft::Ironpedal *ironpedal;
    };
}
