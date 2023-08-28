// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Master
    {
    public:
        Master(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->low.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 1280.0f, 10200.0f, daisy::Parameter::LINEAR);
            this->profile.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.0f, PROFILES, daisy::Parameter::LINEAR);
            this->resonance.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->volume.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 2.01f, daisy::Parameter::LINEAR);

            this->dc_block.Init(this->ironpedal->AudioSampleRate());

            this->filter.Init(this->ironpedal->AudioSampleRate());
            this->filter.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4]);
            this->filter.SetRes(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "IRONPEDAL", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "PROF\n", ALIGN_RIGHT);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "VER " VERSION, ALIGN_LEFT);

            sprintf(buf, "%u\n", this->ironpedal->storage->GetSettings().profile + 1);
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n");

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "LOW", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "RES", ALIGN_CENTER);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "VOL\n", ALIGN_RIGHT);

            float low = this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4] / 1000.0f;

            sprintf(buf, "%lu.%luK", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_CENTER);

            sprintf(buf, "%ld\n", (int32_t)(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("MASTER\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->filter.Process(in[i]);
        }

        void OnInput(bool all = false)
        {
            if (!all)
                this->ironpedal->storage->GetSettings().profile = this->profile.Process();

            if (!this->ironpedal->GetEffect(EFFECT_MASTER).locked)
            {
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4] = this->low.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5] = this->resonance.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_6] = this->volume.Process();
            }

            this->filter.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4]);
            this->filter.SetRes(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->dc_block.Process(out[i]) * this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_6];
        }

    private:
        daisy::Parameter low;
        daisy::Parameter profile;
        daisy::Parameter resonance;
        daisy::Parameter volume;

        daisysp::DcBlock dc_block;
        daisysp::MoogLadder filter;

        Snailsoft::Ironpedal *ironpedal;
    };
}
