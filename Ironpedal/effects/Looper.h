// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    float DSY_SDRAM_BSS Buffer[48000 * 60 * 5];

    class Looper
    {
    public:
        void Draw()
        {
            char buf[16];
            PrintHeader(this->pedal);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "MODE");

            this->pedal->display->setTextColor(COLOR);
            PrintlnCentered(this->pedal, (char *)(this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] > 0.5f ? "RECORD" : "PLAYBACK"));
            PrintlnCentered(this->pedal, 0);

            this->pedal->display->setTextColor(COLOR_LIGHT);
            PrintlnCentered(this->pedal, "VOLUME");

            this->pedal->display->setTextColor(COLOR);
            sprintf(buf, "%d", (int32_t)(this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_5] * 100.0f));
            PrintlnCentered(this->pedal, buf);

            PrintFooter(this->pedal, "LOOPER");
        }

        void Init(Ironpedal *pedal)
        {
            this->pedal = pedal;

            this->mode.Init(this->pedal->controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
            this->volume.Init(this->pedal->controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

            this->bufferIndex = 0;
            this->bufferSize = 0;

            this->recording = false;
        }

        void OnAudio(float *in, float *out, size_t size)
        {
            for (auto i = 0; i < size; ++i)
            {
                if (this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] > 0.5f)
                    Buffer[this->bufferSize = this->bufferSize < sizeof(Buffer) / sizeof(float) ? this->bufferSize + 1 : 0] = in[i];

                else
                    out[i] = in[i] + Buffer[this->bufferIndex = this->bufferIndex < this->bufferSize ? this->bufferIndex + 1 : 0] * this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_5];
            }
        }

        void OnInput()
        {
            if (!this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].locked)
            {
                this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] = this->mode.Process();
                this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_5] = this->volume.Process();

                if ((this->recording && this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] <= 0.5f) || (!this->recording && this->pedal->storage->GetSettings().effects[EFFECT_LOOPER].values[KNOB_2] > 0.5f))
                {
                    this->recording = !this->recording;

                    if (this->recording)
                        this->bufferSize = 0;

                    this->bufferIndex = 0;
                }
            }
        }

    private:
        bool recording;
        Ironpedal *pedal;

        Parameter mode;
        Parameter volume;

        uint32_t bufferIndex;
        uint32_t bufferSize;
    };
}
