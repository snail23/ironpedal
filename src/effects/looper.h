// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    float DSY_SDRAM_BSS Buffer[48000 * 60 * 5];

    class Looper
    {
    public:
        Looper(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->mode.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 0.0f, 1.0f, daisy::Parameter::LINEAR);
            this->volume.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 0.0f, 1.0f, daisy::Parameter::LINEAR);

            this->buffer_index = 0;
            this->buffer_size = 0;

            this->recording = false;
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "MODE\n", ALIGN_CENTER);
            SSD1351_write_string(COLOR, this->ironpedal->font, this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_2] > 0.5f ? "RECORD\n" : "PLAYBACK\n", ALIGN_CENTER);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "VOLUME\n", ALIGN_CENTER);
            sprintf(buf, "%ld\n", (int32_t)(this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_5] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            this->ironpedal->PrintFooter("LOOPER\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                if (this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_2] > 0.5f)
                    Buffer[this->buffer_size = this->buffer_size < sizeof(Buffer) / sizeof(float) ? this->buffer_size + 1 : 0] = in[i];

                else
                    out[i] = in[i] + Buffer[this->buffer_index = this->buffer_index < this->buffer_size ? this->buffer_index + 1 : 0] * this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_5];
            }
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_LOOPER).locked)
            {
                this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_2] = this->mode.Process();
                this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_5] = this->volume.Process();

                if ((this->recording && this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_2] <= 0.5f) || (!this->recording && this->ironpedal->GetEffect(EFFECT_LOOPER).values[PedalPCB::KNOB_2] > 0.5f))
                {
                    this->recording = !this->recording;

                    if (this->recording)
                        this->buffer_size = 0;

                    this->buffer_index = 0;
                }
            }
        }

    private:
        bool recording;

        daisy::Parameter mode;
        daisy::Parameter volume;

        Snailsoft::Ironpedal *ironpedal;

        size_t buffer_index;
        size_t buffer_size;
    };
}
