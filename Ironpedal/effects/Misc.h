// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Misc
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "IRONPEDAL");

            this->pedal->display->setTextColor(COLOR);
            PrintlnCentered(this->pedal, "VER " VERSION);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "METRONOME");

            auto bpm = (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6] * 60.0f);
            this->pedal->display->setTextColor(COLOR);

            if (bpm)
                sprintf(buf, "%u BPM", bpm);

            else
                sprintf(buf, "OFF");

            PrintlnCentered(this->pedal, buf);
            PrintFooter(this->pedal, "MISC");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;
            this->metronomeBPM.Init(this->pedal->controls[KNOB_6], 0.0f, 3.0f, Parameter::LINEAR);

            this->metronome.Init(this->pedal->storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6], this->pedal->AudioSampleRate());
            // this->metronomeBass.Init(this->pedal->AudioSampleRate());
            // this->metronomeBassTurn = true;
            // this->metronomeSnare.Init(this->pedal->AudioSampleRate());
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_MISC].locked)
                this->pedal->storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6] = this->metronomeBPM.Process();

            this->metronome.SetFreq(this->pedal->storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            /*for (auto i = 0; i < size; ++i)
            {
                if (this->metronome.Process())
                {
                    this->metronomeBassTurn ? this->metronomeBass.Trig() : this->metronomeSnare.Trig();
                    this->metronomeBassTurn = !this->metronomeBassTurn;
                }

                out[i] += this->metronomeBass.Process() + this->metronomeSnare.Process();
            }*/
        }

    private:
        // AnalogBassDrum metronomeBass;
        // AnalogSnareDrum metronomeSnare;

        // bool metronomeBassTurn;

        Ironpedal *pedal;
        Metro metronome;
        Parameter metronomeBPM;
    };
}
