// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    namespace Master
    {
        Parameter PostHigh;
        Parameter PostLow;

        Parameter PreGain;
        Parameter PreHigh;
        Parameter PreLow;

        Parameter Volume;

        Svf PostHpf;
        Svf PostLpf;

        Svf PreHpf;
        Svf PreLpf;

        void Draw()
        {
            char buf[16];
            PrintHeader();

            Ironpedal.display->setTextColor(COLOR_LIGHT);
            PrintlnCentered("HIGH LOW  GAIN");

            auto low = Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2] / 1000.0f;

            Ironpedal.display->setTextColor(COLOR);
            sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)((Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_3] - 1.0f) * 100.0f));
            PrintlnCentered(buf);
            PrintlnCentered(0);

            Ironpedal.display->setTextColor(COLOR_LIGHT);
            PrintlnCentered("HIGH LOW   VOL");

            low = Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5] / 1000.0f;

            Ironpedal.display->setTextColor(COLOR);
            sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_6] * 100.0f));
            PrintlnCentered(buf);

            PrintFooter("MASTER");
        }

        void Init()
        {
            PostHigh.Init(Ironpedal.controls[KNOB_4], 20.0f, 160.0f, Parameter::LINEAR);
            PostLow.Init(Ironpedal.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

            PreGain.Init(Ironpedal.controls[KNOB_3], 0.0f, 2.0f, Parameter::LINEAR);
            PreHigh.Init(Ironpedal.controls[KNOB_1], 20.0f, 160.0f, Parameter::LINEAR);
            PreLow.Init(Ironpedal.controls[KNOB_2], 1280.0f, 10200.0f, Parameter::LINEAR);

            Volume.Init(Ironpedal.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);

            PostHpf.Init(Ironpedal.AudioSampleRate());
            PostHpf.SetDrive(0.0f);
            PostHpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
            PostHpf.SetRes(0.0f);

            PostLpf.Init(Ironpedal.AudioSampleRate());
            PostLpf.SetDrive(0.0f);
            PostLpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5]);
            PostLpf.SetRes(0.0f);

            PreHpf.Init(Ironpedal.AudioSampleRate());
            PreHpf.SetDrive(0.0f);
            PreHpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1]);
            PreHpf.SetRes(0.0f);

            PreLpf.Init(Ironpedal.AudioSampleRate());
            PreLpf.SetDrive(0.0f);
            PreLpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2]);
            PreLpf.SetRes(0.0f);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
            {
                PreHpf.Process(in[i]);
                PreLpf.Process(PreHpf.High());

                out[i] = PreLpf.Low() * Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_3];
            }
        }

        void OnInput()
        {
            if (!Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].locked)
            {
                Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1] = PreHigh.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2] = PreLow.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_3] = PreGain.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4] = PostHigh.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5] = PostLow.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_6] = Volume.Process();
            }

            PostHpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
            PostLpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_5]);

            PreHpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_1]);
            PreLpf.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_2]);
        }

        void OnPostAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] *= Ironpedal.storage->GetSettings().effects[EFFECT_MASTER].values[KNOB_6];
        }
    }
}
