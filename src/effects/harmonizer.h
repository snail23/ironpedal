// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Harmonizer
    {
    public:
        Harmonizer(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->flutter1.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->flutter2.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 1.01f, daisy::Parameter::LINEAR);

            this->rate1.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 100.01f, 1.0f, daisy::Parameter::LINEAR);
            this->rate2.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 100.01f, 1.0f, daisy::Parameter::LINEAR);

            this->semitones1.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], -12.01f, 12.01f, daisy::Parameter::LINEAR);
            this->semitones2.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], -12.01f, 12.01f, daisy::Parameter::LINEAR);

            this->pitch1.Init(this->ironpedal->AudioSampleRate());

            this->pitch1.SetDelSize(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_2]) / 1000.0f * this->ironpedal->AudioSampleRate());
            this->pitch1.SetFun(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_3]);
            this->pitch1.SetTransposition(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_1]));

            this->pitch2.Init(this->ironpedal->AudioSampleRate());

            this->pitch2.SetDelSize(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_5]) / 1000.0f * this->ironpedal->AudioSampleRate());
            this->pitch2.SetFun(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_6]);
            this->pitch2.SetTransposition(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_4]));
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "SEMI", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "RATE", ALIGN_CENTER);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "FLUT\n", ALIGN_RIGHT);

            sprintf(buf, "%+ld", (int32_t)round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_1]));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu", (uint32_t)round(101.0f - this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_2]));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_CENTER);

            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_3] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n", this->ironpedal->GetColor().base);

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "SEMI", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "RATE", ALIGN_CENTER);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "FLUT\n", ALIGN_RIGHT);

            sprintf(buf, "%+ld", (int32_t)round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_4]));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu", (uint32_t)round(101.0f - this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_5]));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_CENTER);

            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("HARMONIZER\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = (in[i] + this->pitch1.Process(in[i]) + this->pitch2.Process(in[i])) / 3.0f;
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_HARMONIZER).locked)
            {
                this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_1] = this->semitones1.Process();
                this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_2] = this->rate1.Process();
                this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_3] = this->flutter1.Process();
                this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_4] = this->semitones2.Process();
                this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_5] = this->rate2.Process();
                this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_6] = this->flutter2.Process();
            }

            this->pitch1.SetDelSize(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_2]) / 1000.0f * this->ironpedal->AudioSampleRate());
            this->pitch1.SetFun(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_3]);
            this->pitch1.SetTransposition(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_1]));

            this->pitch2.SetDelSize(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_5]) / 1000.0f * this->ironpedal->AudioSampleRate());
            this->pitch2.SetFun(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_6]);
            this->pitch2.SetTransposition(round(this->ironpedal->GetEffect(EFFECT_HARMONIZER).values[PedalPCB::KNOB_4]));
        }

    private:
        daisy::Parameter flutter1;
        daisy::Parameter flutter2;

        daisy::Parameter rate1;
        daisy::Parameter rate2;

        daisy::Parameter semitones1;
        daisy::Parameter semitones2;

        daisysp::PitchShifter pitch1;
        daisysp::PitchShifter pitch2;

        Snailsoft::Ironpedal *ironpedal;
    };
}
