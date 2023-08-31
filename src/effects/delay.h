// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    float DSY_SDRAM_BSS DelayBuffer[2400];

    class Delay
    {
    public:
        Delay(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->delay.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 0.0f, 50.01f, daisy::Parameter::LINEAR);

            this->delay_line.Init(DelayBuffer, sizeof(DelayBuffer) / sizeof(float));
            this->delay_line.SetDelay((size_t)(this->ironpedal->AudioSampleRate() / this->ironpedal->GetEffect(EFFECT_DELAY).values[PedalPCB::KNOB_2]));
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(this->ironpedal->GetColor().light, this->ironpedal->font, "DELAY\n", ALIGN_CENTER);
            sprintf(buf, "%lu MS\n", (uint32_t)this->ironpedal->GetEffect(EFFECT_DELAY).values[PedalPCB::KNOB_2]);
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, buf, ALIGN_CENTER);

            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n");
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n");
            SSD1351_write_string(this->ironpedal->GetColor().base, this->ironpedal->font, "\n");

            this->ironpedal->PrintFooter("DELAY\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                this->delay_line.Write(in[i]);
                out[i] = this->delay_line.Read();
            }
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_DELAY).locked)
                this->ironpedal->GetEffect(EFFECT_DELAY).values[PedalPCB::KNOB_2] = this->delay.Process();

            this->delay_line.SetDelay((size_t)(this->ironpedal->AudioSampleRate() / this->ironpedal->GetEffect(EFFECT_DELAY).values[PedalPCB::KNOB_2]));
        }

    private:
        daisy::Parameter delay;
        daisysp::DelayLine delay_line;

        Snailsoft::Ironpedal *ironpedal;
    };
}
