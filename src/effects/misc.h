// Part of Ironpedal
// https://github.com/snail23/ironpedal

#ifndef max
#define max(a, b) ((a < b) ? b : a)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

namespace Effect
{
    class Misc
    {
    public:
        bool Update()
        {
            if (this->tuner_updated)
            {
                uint32_t now = daisy::System::GetNow();

                if (now - this->tuner_time > 250)
                {
                    this->tuner_time = now;
                    this->tuner_updated = false;

                    return true;
                }
            }

            return false;
        }

        Misc(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->metronome_bpm.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.0f, 3.0f, daisy::Parameter::LINEAR);
            this->profile.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, PROFILES - 1.0f, daisy::Parameter::LINEAR);

            this->buffer_index = 0;

            this->metronome.Init(this->ironpedal->GetEffect(EFFECT_MISC).values[PedalPCB::KNOB_3], this->ironpedal->AudioSampleRate());
            this->metronome_bass.Init(this->ironpedal->AudioSampleRate());

            this->tuner_frequency = 0.0f;
            this->tuner_time = daisy::System::GetNow();
            this->tuner_updated = false;
        }

        void Draw()
        {
            char buf[24];
            char *buf2 = buf;

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "IRONPEDAL", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "METRO\n", ALIGN_RIGHT);

            SSD1351_write_string(COLOR, this->ironpedal->font, "VER " VERSION, ALIGN_LEFT);
            uint32_t bpm = this->ironpedal->GetEffect(EFFECT_MISC).values[PedalPCB::KNOB_3] * 60.0f;

            if (bpm)
                sprintf(buf, "%lu\n", bpm);

            else
                sprintf(buf, "OFF\n");

            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);
            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "TUNER", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "PROFILE\n", ALIGN_RIGHT);

            int16_t cents = max(-5, min(5, this->ironpedal->Cents(this->tuner_frequency, this->ironpedal->Note(this->tuner_frequency))));

            if (cents < 0)
            {
                for (int16_t i = cents; i < 0; ++i)
                {
                    *buf2++ = '-';
                    *buf2 = 0;
                }
            }

            sprintf(buf2, "%s%u", this->ironpedal->notes[this->ironpedal->Note(this->tuner_frequency) % 12], this->ironpedal->Octave(this->tuner_frequency));
            buf2 += strlen(buf2);

            if (cents > 0)
            {
                for (int16_t i = 0; i < cents; ++i)
                {
                    *buf2++ = '+';
                    *buf2 = 0;
                }
            }

            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%u\n", this->ironpedal->storage->GetSettings().profile + 1);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("MISC\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                if (this->buffer_index < sizeof(this->buffer) / sizeof(float))
                    this->buffer[this->buffer_index++] = in[i];

                else
                {
                    this->buffer_index = 0;
                    float frequency = round(this->ironpedal->Autocorrelate(this->buffer, sizeof(this->buffer) / sizeof(float)));

                    if (frequency && round(this->tuner_frequency) != frequency)
                    {
                        this->tuner_frequency = frequency;
                        this->tuner_updated = true;
                    }
                }
            }
        }

        void OnInput(bool all = false)
        {
            if (!all)
                this->ironpedal->storage->GetSettings().profile = this->profile.Process();

            if (!this->ironpedal->GetEffect(EFFECT_MISC).locked)
                this->ironpedal->GetEffect(EFFECT_MISC).values[PedalPCB::KNOB_3] = this->metronome_bpm.Process();

            this->metronome.SetFreq(this->ironpedal->GetEffect(EFFECT_MISC).values[PedalPCB::KNOB_3]);
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
        bool tuner_updated;

        daisy::Parameter metronome_bpm;
        daisy::Parameter profile;

        daisysp::AnalogBassDrum metronome_bass;
        daisysp::Metro metronome;

        float buffer[2048];
        float tuner_frequency;

        size_t buffer_index;
        Snailsoft::Ironpedal *ironpedal;

        uint32_t tuner_time;
    };
}
