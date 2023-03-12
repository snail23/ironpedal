// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Misc
    {
    public:
        Misc(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;
            this->metronome_bpm.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 3.0f, daisy::Parameter::LINEAR);

            this->metronome.Init(this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6], this->ironpedal->AudioSampleRate());
            this->metronome_bass.Init(this->ironpedal->AudioSampleRate());
        }

        void Draw()
        {
            char buf[16];

            PrintlnCentered(this->ironpedal, "IRONPEDAL", COLOR_LIGHT);
            PrintlnCentered(this->ironpedal, "VER " VERSION, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "METRONOME", COLOR_LIGHT);

            auto bpm = (uint32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6] * 60.0f);

            if (bpm)
                sprintf(buf, "%lu BPM", bpm);

            else
                sprintf(buf, "OFF");

            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintFooter(this->ironpedal, "MISC");
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].locked)
                this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6] = this->metronome_bpm.Process();

            this->metronome.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
            {
                if (this->metronome.Process())
                    this->metronome_bass.Trig();

                out[i] += this->metronome_bass.Process();
            }
        }

    private:
        daisy::Parameter metronome_bpm;

        daisysp::AnalogBassDrum metronome_bass;
        daisysp::Metro metronome;

        Snailsoft::Ironpedal *ironpedal;
    };
}
