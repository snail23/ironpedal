// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Pitch
    {
    public:
        Pitch(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->flutter.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 1.01f, daisy::Parameter::LINEAR);
            this->rate.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 100.01f, 1.0f, daisy::Parameter::LINEAR);
            this->semitones.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], -12.01f, 12.01f, daisy::Parameter::LINEAR);

            this->pitch.Init(this->ironpedal->AudioSampleRate());

            this->pitch.SetDelSize(round(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_4] / 1000.0f) * this->ironpedal->AudioSampleRate());
            this->pitch.SetFun(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_6]);
            this->pitch.SetTransposition(round(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_2]));
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "SEMITONES\n", ALIGN_CENTER);
            sprintf(buf, "%+ld\n", (int32_t)round(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_2]));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_CENTER);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n", this->ironpedal->GetColor().base);

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "RATE", ALIGN_LEFT);
            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "FLUTTER\n", ALIGN_RIGHT);

            sprintf(buf, "%lu", (uint32_t)round(101.0f - this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_4]));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu\n", (uint32_t)(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("PITCH\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] = this->pitch.Process(in[i]);
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_PITCH).locked)
            {
                this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_2] = this->semitones.Process();
                this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_4] = this->rate.Process();
                this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_6] = this->flutter.Process();
            }

            this->pitch.SetDelSize(round(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_4]) / 1000.0f * this->ironpedal->AudioSampleRate());
            this->pitch.SetFun(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_6]);
            this->pitch.SetTransposition(round(this->ironpedal->GetEffect(EFFECT_PITCH).values[PedalPCB::KNOB_2]));
        }

    private:
        daisy::Parameter flutter;
        daisy::Parameter rate;
        daisy::Parameter semitones;

        daisysp::PitchShifter pitch;
        Snailsoft::Ironpedal *ironpedal;
    };
}
