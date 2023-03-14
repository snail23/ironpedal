// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Resonator
    {
    public:
        Resonator(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->brightness.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->decay.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->frequency.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 65.0f, 880.0f, daisy::Parameter::LINEAR);
            this->stiffness.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 0.0f, 1.0f, daisy::Parameter::LINEAR);

            this->resonator.Init(0.0f, 24, this->ironpedal->AudioSampleRate());

            this->resonator.SetBrightness(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_6]);
            this->resonator.SetDamping(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_1]);
            this->resonator.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_3]);
            this->resonator.SetStructure(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_4]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "DECAY", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "FREQ\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_1] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu HZ\n", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_3]);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);
            
            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "STIFF", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "BRIGHT\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_4] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            PrintFooter(this->ironpedal, "RESONATOR\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->resonator.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_1] = this->decay.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_3] = this->frequency.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_4] = this->stiffness.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_6] = this->brightness.Process();
            }

            this->resonator.SetBrightness(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_6]);
            this->resonator.SetDamping(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_1]);
            this->resonator.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_3]);
            this->resonator.SetStructure(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_4]);
        }

    private:
        daisy::Parameter brightness;
        daisy::Parameter decay;
        daisy::Parameter frequency;
        daisy::Parameter stiffness;

        daisysp::Resonator resonator;
        Snailsoft::Ironpedal *ironpedal;
    };
}
