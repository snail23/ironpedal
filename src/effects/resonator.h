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

            PrintlnCentered(this->ironpedal, "DECAY     FREQ", COLOR_LIGHT);
            sprintf(buf, "%3lu     %3lu HZ", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_1] * 100.0f), (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_3]);
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "STIFF   BRIGHT", COLOR_LIGHT);
            sprintf(buf, "%3lu        %3lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_4] * 100.0f), (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_RESONATOR].values[PedalPCB::KNOB_6] * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);

            PrintFooter(this->ironpedal, "RESONATOR");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
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
