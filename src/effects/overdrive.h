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

            this->blend.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->drive.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->mode.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.0f, 1.0f, daisy::Parameter::LINEAR);

            this->overdrive.SetDrive(this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_5]);
            this->overdrive.SetSoft(this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_1] > 0.5f);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "BLEND", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "MODE\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_1] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            SSD1351_write_string(COLOR, this->ironpedal->font, this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_3] > 0.5f ? "SOFT\n" : "HARD\n", ALIGN_RIGHT);
            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "DRIVE\n", ALIGN_CENTER);
            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_5] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            this->ironpedal->PrintFooter("OVERDRIVE\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = in[i] * this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_1] + this->overdrive.Process(in[i]) * (1.0f - this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_1]);
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_OVERDRIVE).locked)
            {
                this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_1] = this->blend.Process();
                this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_3] = this->mode.Process();
                this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_5] = this->drive.Process();
            }

            this->overdrive.SetSoft(this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_1] > 0.5f);
            this->overdrive.SetDrive(this->ironpedal->GetEffect(EFFECT_OVERDRIVE).values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter blend;
        daisy::Parameter drive;
        daisy::Parameter mode;

        daisysp::Overdrive overdrive;
        Snailsoft::Ironpedal *ironpedal;
    };
}
