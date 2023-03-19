// Part of Ironpedal
// https://github.com/snail23/ironpedal

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

                if (now - this->tuner_time > 1000)
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

            this->metronome.Init(this->ironpedal->GetEffect(EFFECT_MISC).values[PedalPCB::KNOB_3], this->ironpedal->AudioSampleRate());
            this->metronome_bass.Init(this->ironpedal->AudioSampleRate());

            this->tuner_frequency = 0.0f;
            this->tuner_time = daisy::System::GetNow();
            this->tuner_updated = false;
        }

        void Draw()
        {
            char buf[16];

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

            sprintf(buf, "%lu", (uint32_t)round(this->tuner_frequency));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%u\n", this->ironpedal->storage->GetSettings().profile + 1);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("MISC\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            float frequency = this->Autocorrelate(in, size);

            if (round(this->tuner_frequency) != round(frequency))
            {
                this->tuner_frequency = frequency;
                this->tuner_updated = true;
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

        float tuner_frequency;
        Snailsoft::Ironpedal *ironpedal;
        uint32_t tuner_time;

        float Autocorrelate(float *in, size_t size)
        {
            float rms = 0.0f;

            for (size_t i = 0; i < size; ++i)
                rms += in[i] * in[i];

            rms = sqrt(rms / size);

            if (rms < 0.01f)
                return 0.0f;

            bool found = false;

            size_t index = 0;
            size_t new_size = size - 1;

            for (size_t i = 0; i < size / 2; ++i)
            {
                if (fabs(in[i]) < 0.2f)
                {
                    found = true;
                    index = i;
                }

                if (i + 1 < size && fabs(in[size - i + 1]) < 0.2f)
                {
                    found = true;
                    new_size = size - i;
                }

                if(found)
                    break;
            }

            in = &in[index];
            size = new_size;

            float diffs[size] = {0.0f};

            for (size_t i = 0; i < size; ++i)
            {
                for (size_t j = 0; j < size - i; ++j)
                    diffs[i] = diffs[i] + in[j] * in[j + i];
            }

            index = 0;

            while (index < size && diffs[index] > diffs[index + 1])
                index++;

            float max = -1.0f;

            for (size_t i = index; i < size; ++i)
            {
                if (diffs[i] > max)
                {
                    max = diffs[i];
                    index = i;
                }
            }

            float val = (diffs[index - 1] + diffs[index + 1] - 2.0f * diffs[index]) / 2.0f;
            return this->ironpedal->AudioSampleRate() / 2.0f / (val ? index - (diffs[index + 1] - diffs[index - 1]) / 2.0f / (2.0f * val) : index);
        }
    };
}
