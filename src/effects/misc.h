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

        void Update()
        {
            if (this->tuner_updated)
            {
                uint32_t now = daisy::System::GetNow();

                if (now - this->tuner_time > 1000)
                {
                    this->Draw();

                    this->tuner_time = now;
                    this->tuner_updated = false;
                }
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
            // Implements the ACF2+ algorithm
            float rms = 0;

            for (size_t i = 0; i < size; i++)
            {
                float val = in[i];
                rms += val * val;
            }
            rms = sqrt(rms / size);
            if (rms < 0.01) // not enough signal
                return 0.0f;

            size_t r1 = 0, r2 = size - 1;
            float thres = 0.2;
            for (size_t i = 0; i < size / 2; i++)
                if (fabs(in[i]) < thres)
                {
                    r1 = i;
                    break;
                }
            for (size_t i = 1; i < size / 2; i++)
                if (fabs(in[size - i]) < thres)
                {
                    r2 = size - i;
                    break;
                }

            in = &in[r1];
            size = r2;

            float c[size] = {0.0f};
            for (size_t i = 0; i < size; i++)
                for (size_t j = 0; j < size - i; j++)
                    c[i] = c[i] + in[j] * in[j + i];

            size_t d = 0;
            while (c[d] > c[d + 1])
                d++;
            float maxval = -1;
            size_t maxpos = -1;
            for (size_t i = d; i < size; i++)
            {
                if (c[i] > maxval)
                {
                    maxval = c[i];
                    maxpos = i;
                }
            }
            size_t T0 = maxpos;

            float x1 = c[T0 - 1];
            float x2 = c[T0];
            float x3 = c[T0 + 1];
            float a = (x1 + x3 - 2 * x2) / 2;
            float b = (x3 - x1) / 2;
            if (a)
                T0 = T0 - b / (2 * a);

            return this->ironpedal->AudioSampleRate() / T0;
            /*float rms = 0.0f;

            for (size_t i = 0; i < size; ++i)
                rms += in[i] * in[i];

            rms = sqrt(rms / size);

            if (rms < 0.01f)
                return;

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

            while (diffs[index] > diffs[index + 1])
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
            this->tuner_frequency = this->ironpedal->AudioSampleRate() / (val ? index - (diffs[index + 1] - diffs[index - 1]) / 2.0f / (2.0f * val) : index);*/
        }
    };
}
