// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Autowah
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "BLEND");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%u", (uint32_t)this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
            PrintlnCentered(this->pedal, buf);
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "WAH");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%u", (uint32_t)(this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5] * 100.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "AUTOWAH");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->blend.Init(this->pedal->controls[KNOB_2], 0.0f, 100.0f, Parameter::LINEAR);
            this->wah.Init(this->pedal->controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

            this->autowah.Init(this->pedal->AudioSampleRate());

            this->autowah.SetDryWet(100.0f - this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
            this->autowah.SetLevel(1.0f);
            this->autowah.SetWah(this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = this->autowah.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2] = this->blend.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5] = this->wah.Process();
            }

            this->autowah.SetDryWet(100.0f - this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
            this->autowah.SetWah(this->pedal->storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5]);
        }

    private:
        daisysp::Autowah autowah;
        Ironpedal *pedal;

        Parameter blend;
        Parameter wah;
    };
}
