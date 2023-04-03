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
            this->wah.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.01f, daisy::Parameter::LINEAR);

            this->autowah.Init(this->ironpedal->AudioSampleRate());

            this->autowah.SetDryWet(100.0f - this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_2]);
            this->autowah.SetLevel(1.0f);
            this->autowah.SetWah(this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_5]);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "BLEND\n", ALIGN_CENTER);
            sprintf(buf, "%lu\n", (uint32_t)this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_2]);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "WAH\n", ALIGN_CENTER);
            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_5] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            this->ironpedal->PrintFooter("AUTOWAH\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->autowah.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_AUTOWAH).locked)
            {
                this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_2] = this->blend.Process();
                this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_5] = this->wah.Process();
            }

            this->autowah.SetDryWet(100.0f - this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_2]);
            this->autowah.SetWah(this->ironpedal->GetEffect(EFFECT_AUTOWAH).values[PedalPCB::KNOB_5]);
        }

    private:
        daisy::Parameter blend;
        daisy::Parameter wah;

        daisysp::Autowah autowah;
        Snailsoft::Ironpedal *ironpedal;
    };
}
