// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Compressor
    {
    public:
        Compressor(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->attack.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 0.001f, 10.0f, daisy::Parameter::LINEAR);
            this->ratio.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 1.0f, 40.0f, daisy::Parameter::LINEAR);
            this->release.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.001f, 10.0f, daisy::Parameter::LINEAR);
            this->threshold.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, -80.0f, daisy::Parameter::LINEAR);

            this->compressor.Init(this->ironpedal->AudioSampleRate());

            this->compressor.SetAttack(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4]);
            this->compressor.SetRatio(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_3]);
            this->compressor.SetRelease(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6]);
            this->compressor.SetThreshold(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_1]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "THRESH", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "RATIO\n", ALIGN_RIGHT);

            sprintf(buf, "%ld dB", (int32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_1]);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu:1\n", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_3]);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n", COLOR);

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "ATTACK", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "RELEASE\n", ALIGN_RIGHT);

            sprintf(buf, "%lu.%03lu", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4], (uint32_t)((this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4] - floor(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4])) * 1000.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu.%03lu\n", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6], (uint32_t)((this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6] - floor(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6])) * 1000.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            PrintFooter(this->ironpedal, "COMPRESSOR\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->compressor.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_1] = this->threshold.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_3] = this->ratio.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4] = this->attack.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6] = this->release.Process();
            }

            this->compressor.SetAttack(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4]);
            this->compressor.SetRatio(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_3]);
            this->compressor.SetRelease(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6]);
            this->compressor.SetThreshold(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_1]);
        }

    private:
        daisy::Parameter attack;
        daisy::Parameter ratio;
        daisy::Parameter release;
        daisy::Parameter threshold;

        daisysp::Compressor compressor;
        Snailsoft::Ironpedal *ironpedal;
    };
}
