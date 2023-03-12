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

            PrintlnCentered(this->ironpedal, "THRESH   RATIO", COLOR_LIGHT);
            sprintf(buf, "%3ld dB    %2lu:1", (int32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_1], (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_3]);
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "ATTACK RELEASE", COLOR_LIGHT);
            sprintf(buf, "%2lu.%03lu  %2lu.%03lu", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4], (uint32_t)((this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4] - floor(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_4])) * 1000.0f), (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6], (uint32_t)((this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6] - floor(this->ironpedal->storage->GetSettings().effects[EFFECT_COMPRESSOR].values[PedalPCB::KNOB_6])) * 1000.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);

            PrintFooter(this->ironpedal, "COMPRESSOR");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
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
