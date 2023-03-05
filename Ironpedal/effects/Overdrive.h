// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    namespace Overdrive
    {
        daisysp::Overdrive Overdrive;

        Parameter Blend;
        Parameter Drive;

        void Draw()
        {
            char buf[16];
            PrintHeader();

            Ironpedal.display->setTextColor(COLOR_LIGHT);
            PrintlnCentered("BLEND");

            Ironpedal.display->setTextColor(COLOR);
            sprintf(buf, "%u", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] * 100.0f));
            PrintlnCentered(buf);
            PrintlnCentered(0);

            Ironpedal.display->setTextColor(COLOR_LIGHT);
            PrintlnCentered("DRIVE");

            Ironpedal.display->setTextColor(COLOR);
            sprintf(buf, "%u", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] * 100.0f));
            PrintlnCentered(buf);

            PrintFooter("OVERDRIVE");
        }

        void Init()
        {
            Blend.Init(Ironpedal.controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
            Drive.Init(Ironpedal.controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

            Overdrive.SetDrive(Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
                out[i] = in[i] * Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] + Overdrive.Process(in[i]) * (1.0f - Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2]);
        }

        void OnInput()
        {
            if (!Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].locked)
            {
                Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] = Blend.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] = Drive.Process();
            }

            Overdrive.SetDrive(Ironpedal.storage->GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
        }
    }
}
