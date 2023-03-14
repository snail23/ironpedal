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

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "IRONPEDAL\n", ALIGN_CENTER);
            SSD1351_write_string(COLOR, this->ironpedal->font, "VER " VERSION "\n", ALIGN_CENTER);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "METRONOME\n", ALIGN_CENTER);

            uint32_t bpm = this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6] * 60.0f;

            if (bpm)
                sprintf(buf, "%lu BPM\n", bpm);

            else
                sprintf(buf, "OFF\n");

            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);
            PrintFooter(this->ironpedal, "MISC\n");
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].locked)
                this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6] = this->metronome_bpm.Process();

            this->metronome.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MISC].values[PedalPCB::KNOB_6]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
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
