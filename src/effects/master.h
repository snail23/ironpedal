// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    class Master
    {
    public:
        Master(Snailsoft::Ironpedal *ironpedal)
        {
            this->ironpedal = ironpedal;

            this->post_high.Init(this->ironpedal->knobs[PedalPCB::KNOB_4], 20.0f, 160.0f, daisy::Parameter::LINEAR);
            this->post_low.Init(this->ironpedal->knobs[PedalPCB::KNOB_5], 1280.0f, 10200.0f, daisy::Parameter::LINEAR);

            this->pre_gain.Init(this->ironpedal->knobs[PedalPCB::KNOB_3], 0.0f, 2.0f, daisy::Parameter::LINEAR);
            this->pre_high.Init(this->ironpedal->knobs[PedalPCB::KNOB_1], 20.0f, 160.0f, daisy::Parameter::LINEAR);
            this->pre_low.Init(this->ironpedal->knobs[PedalPCB::KNOB_2], 1280.0f, 10200.0f, daisy::Parameter::LINEAR);

            this->volume.Init(this->ironpedal->knobs[PedalPCB::KNOB_6], 0.0f, 1.0f, daisy::Parameter::LINEAR);

            this->post_hpf.Init(this->ironpedal->AudioSampleRate());
            this->post_hpf.SetDrive(0.0f);
            this->post_hpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4]);
            this->post_hpf.SetRes(0.0f);

            this->post_lpf.Init(this->ironpedal->AudioSampleRate());
            this->post_lpf.SetDrive(0.0f);
            this->post_lpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5]);
            this->post_lpf.SetRes(0.0f);

            this->pre_hpf.Init(this->ironpedal->AudioSampleRate());
            this->pre_hpf.SetDrive(0.0f);
            this->pre_hpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_1]);
            this->pre_hpf.SetRes(0.0f);

            this->pre_lpf.Init(this->ironpedal->AudioSampleRate());
            this->pre_lpf.SetDrive(0.0f);
            this->pre_lpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_2]);
            this->pre_lpf.SetRes(0.0f);
        }

        void Draw()
        {
            char buf[16];

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "HIGH", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "LOW", ALIGN_CENTER);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "GAIN\n", ALIGN_RIGHT);

            float low = this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_2] / 1000.0f;

            sprintf(buf, "%lu", (uint32_t)this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_1]);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);

            sprintf(buf, "%lu.%luK", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);

            sprintf(buf, "%ld\n", (int32_t)((this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_3] - 1.0f) * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            SSD1351_write_string(COLOR, this->ironpedal->font, "\n");

            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "HIGH", ALIGN_LEFT);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "LOW", ALIGN_CENTER);
            SSD1351_write_string(COLOR_LIGHT, this->ironpedal->font, "VOL\n", ALIGN_RIGHT);

            low = this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5] / 1000.0f;
            
            sprintf(buf, "%lu", (uint32_t)this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4]);
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_LEFT);
            
            sprintf(buf, "%lu.%luK", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_CENTER);
            
            sprintf(buf, "%ld\n", (int32_t)(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_6] * 100.0f));
            SSD1351_write_string(COLOR, this->ironpedal->font, buf, ALIGN_RIGHT);

            this->ironpedal->PrintFooter("MASTER\n");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                this->pre_hpf.Process(in[i]);
                this->pre_lpf.Process(this->pre_hpf.High());

                out[i] = this->pre_lpf.Low() * this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_3];
            }
        }

        void OnInput()
        {
            if (!this->ironpedal->GetEffect(EFFECT_MASTER).locked)
            {
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_1] = this->pre_high.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_2] = this->pre_low.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_3] = this->pre_gain.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4] = this->post_high.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5] = this->post_low.Process();
                this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_6] = this->volume.Process();
            }

            this->post_hpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_4]);
            this->post_lpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_5]);

            this->pre_hpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_1]);
            this->pre_lpf.SetFreq(this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_2]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
                out[i] *= this->ironpedal->GetEffect(EFFECT_MASTER).values[PedalPCB::KNOB_6];
        }

    private:
        daisy::Parameter post_high;
        daisy::Parameter post_low;

        daisy::Parameter pre_gain;
        daisy::Parameter pre_high;
        daisy::Parameter pre_low;

        daisy::Parameter volume;

        daisysp::Svf post_hpf;
        daisysp::Svf post_lpf;

        daisysp::Svf pre_hpf;
        daisysp::Svf pre_lpf;

        Snailsoft::Ironpedal *ironpedal;
    };
}
