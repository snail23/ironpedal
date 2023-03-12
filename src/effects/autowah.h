// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Autowah
    {
    public:
        Autowah(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->blend.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 0.0f, 100.0f, daisy::Parameter::LINEAR);
            this->wah.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.0f, daisy::Parameter::LINEAR);

            this->autowah.Init(this->ironpedal->AudioSampleRate());

            this->autowah.SetDryWet(100.0f - this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_2]);
            this->autowah.SetLevel(1.0f);
            this->autowah.SetWah(this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            PrintlnCentered(this->ironpedal, "BLEND", COLOR_LIGHT);
            sprintf(buf, "%lu", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_2]);
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "WAH", COLOR_LIGHT);
            sprintf(buf, "%lu", (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_5] * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);

            PrintFooter(this->ironpedal, "AUTOWAH");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
                out[i] = this->autowah.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_2] = this->blend.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_5] = this->wah.Process();
            }

            this->autowah.SetDryWet(100.0f - this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_2]);
            this->autowah.SetWah(this->ironpedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter blend;
        daisy::Parameter wah;

        daisysp::Autowah autowah;
        Snailsoft::Ironpedal *ironpedal;
    };
}