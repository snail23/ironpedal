// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    namespace Looper
    {
        bool Recording;
        float DSY_SDRAM_BSS Buffer[48000 * 60 * 5];

        Parameter Mode;
        Parameter Volume;

        uint32_t BufferIndex;
        uint32_t BufferSize;

        void Draw()
        {
            char buf[16];
            PrintHeader();

            Ironpedal.display->setTextColor(COLOR_LIGHT);
            PrintlnCentered("MODE");

            Ironpedal.display->setTextColor(COLOR);
            PrintlnCentered(Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] > 0.5f ? "RECORD" : "PLAYBACK");
            PrintlnCentered(0);

            Ironpedal.display->setTextColor(COLOR_LIGHT);
            PrintlnCentered("VOLUME");

            Ironpedal.display->setTextColor(COLOR);
            sprintf(buf, "%d", (int32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_5] * 100.0f));
            PrintlnCentered(buf);

            PrintFooter("LOOPER");
        }

        void Init()
        {
            BufferIndex = 0;
            BufferSize = 0;

            Mode.Init(Ironpedal.controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
            Recording = false;
            Volume.Init(Ironpedal.controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
            {
                if (Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] > 0.5f)
                    Buffer[BufferSize = BufferSize < sizeof(Buffer) / sizeof(float) ? BufferSize + 1 : 0] = in[i];

                else
                    out[i] = in[i] + Buffer[BufferIndex = BufferIndex < BufferSize ? BufferIndex + 1 : 0] * Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_5];
            }
        }

        void OnInput()
        {
            if (!Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].locked)
            {
                Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] = Mode.Process();
                Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_5] = Volume.Process();

                if ((Recording && Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] <= 0.5f) || (!Recording && Ironpedal.storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] > 0.5f))
                {
                    Recording = !Recording;

                    if (Recording)
                        BufferSize = 0;

                    BufferIndex = 0;
                }
            }
        }
    }
}
