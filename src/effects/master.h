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
            this->post_hpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_4]);
            this->post_hpf.SetRes(0.0f);

            this->post_lpf.Init(this->ironpedal->AudioSampleRate());
            this->post_lpf.SetDrive(0.0f);
            this->post_lpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_5]);
            this->post_lpf.SetRes(0.0f);

            this->pre_hpf.Init(this->ironpedal->AudioSampleRate());
            this->pre_hpf.SetDrive(0.0f);
            this->pre_hpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_1]);
            this->pre_hpf.SetRes(0.0f);

            this->pre_lpf.Init(this->ironpedal->AudioSampleRate());
            this->pre_lpf.SetDrive(0.0f);
            this->pre_lpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_2]);
            this->pre_lpf.SetRes(0.0f);
        }

        void Draw()
        {
            char buf[16];

            PrintlnCentered(this->ironpedal, "HIGH LOW  GAIN", COLOR_LIGHT);
            auto low = this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_2] / 1000.0f;
            sprintf(buf, "%3lu %2lu.%luK %4ld", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_1], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)((this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_3] - 1.0f) * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);
            PrintlnCentered(this->ironpedal, "", COLOR);

            PrintlnCentered(this->ironpedal, "HIGH LOW   VOL", COLOR_LIGHT);
            low = this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_5] / 1000.0f;
            sprintf(buf, "%3lu %2lu.%luK %4ld", (uint32_t)this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_4], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_6] * 100.0f));
            PrintlnCentered(this->ironpedal, buf, COLOR);

            PrintFooter(this->ironpedal, "MASTER");
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
            {
                this->pre_hpf.Process(in[i]);
                this->pre_lpf.Process(this->pre_hpf.High());

                out[i] = this->pre_lpf.Low() * this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_3];
            }
        }

        void OnInput()
        {
            if (!this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].locked)
            {
                this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_1] = this->pre_high.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_2] = this->pre_low.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_3] = this->pre_gain.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_4] = this->post_high.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_5] = this->post_low.Process();
                this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_6] = this->volume.Process();
            }

            this->post_hpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_4]);
            this->post_lpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_5]);

            this->pre_hpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_1]);
            this->pre_lpf.SetFreq(this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_2]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0u; i < size; ++i)
                out[i] *= this->ironpedal->storage->GetSettings().effects[EFFECT_MASTER].values[PedalPCB::KNOB_6];
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
