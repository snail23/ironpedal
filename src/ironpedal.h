// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Snailsoft
{
    class Ironpedal;
}

void Draw();
void MessageBox(Snailsoft::Ironpedal *ironpedal, const char *text, uint32_t event);

void OnInput();
void OnInputAll();

namespace Snailsoft
{
    struct StorageData
    {
        Effect::Effect effects[Effect::EFFECT_LAST];

        bool operator==(StorageData &rhs)
        {
            return &this->effects == &rhs.effects;
        }

        bool operator!=(StorageData &rhs)
        {
            return !this->operator==(rhs);
        }
    };

    class Ironpedal : public PedalPCB::Terrarium
    {
    public:
        daisy::PersistentStorage<StorageData> *storage;
        Effect::Type current_effect;

        Ironpedal()
        {
            this->current_effect.id = Effect::EFFECT_MASTER;

            for (auto i = 0; i <= PedalPCB::FOOT_SWITCH_2; ++i)
                this->ResetFootSwitchData(i);

            this->storage = new daisy::PersistentStorage<StorageData>(this->qspi);
            this->storage->Init({});
            this->storage->GetDefaultSettings() = this->storage->GetSettings();
        }

        void Run()
        {
            SSD1351_init();

            this->Splash();
            this->Start();

            bool input_received;

            while (true)
            {
                for (auto i = 0; i < PedalPCB::KNOB_LAST; ++i)
                    this->knobs[i].Process();

                for (auto i = 0; i < PedalPCB::SWITCH_LAST; ++i)
                    this->switches[i].Debounce();

                this->ProcessFootSwitches();
                input_received = false;

                if (this->IsSwitchPressed())
                    input_received = true;

                if (!this->storage->GetSettings().effects[this->current_effect.id].locked && this->HasControlChanged())
                    input_received = true;

                if (input_received)
                    OnInput();
            }
        }

    private:
        uint32_t control_values[PedalPCB::KNOB_LAST];

        struct
        {
            bool events[5];
            uint32_t hold_time;
        } foot_switch_data[PedalPCB::FOOT_SWITCH_2 + 1];

        bool HasControlChanged()
        {
            auto input_received = false;
            uint32_t val;

            for (auto i = 0; i < PedalPCB::KNOB_LAST; ++i)
            {
                val = (uint32_t)round(this->knobs[i].Value() * 100.0f);

                if (this->control_values[i] != val)
                {
                    this->control_values[i] = val;
                    input_received = true;
                }
            }

            return input_received;
        }

        bool IsSwitchPressed()
        {
            auto input_received = false;
            auto now = daisy::System::GetNow();

            for (auto i = 0; i < PedalPCB::SWITCH_LAST; ++i)
            {
                if (this->switches[i].RisingEdge())
                {
                    if (this->SwitchPressed(i))
                        input_received = true;
                }
                else if (this->switches[i].FallingEdge())
                {
                    if (i <= PedalPCB::FOOT_SWITCH_2)
                    {
                        if (now - this->foot_switch_data[i].hold_time < 4000)
                            input_received = this->SwitchReleased(i);

                        else
                        {
                            this->ResetFootSwitchData(i);
                            input_received = true;
                        }
                    }
                    else
                        input_received = this->SwitchReleased(i);
                }
            }

            return input_received;
        }

        bool SwitchPressed(uint8_t id)
        {
            switch (id)
            {
            case PedalPCB::FOOT_SWITCH_1:
            case PedalPCB::FOOT_SWITCH_2:
                this->foot_switch_data[id].hold_time = daisy::System::GetNow();

                break;

            case PedalPCB::SWITCH_1:
                this->current_effect.switch1 = true;

                return true;

            case PedalPCB::SWITCH_2:
                this->current_effect.switch2 = true;

                return true;

            case PedalPCB::SWITCH_3:
                this->current_effect.switch3 = true;

                return true;

            case PedalPCB::SWITCH_4:
                this->current_effect.switch4 = true;

                return true;
            }

            return false;
        }

        bool SwitchReleased(uint8_t id)
        {
            switch (id)
            {
            case PedalPCB::FOOT_SWITCH_1:
                this->storage->GetSettings().effects[this->current_effect.id].enabled = !this->storage->GetSettings().effects[this->current_effect.id].enabled;
                this->ResetFootSwitchData(id);

                return true;

            case PedalPCB::FOOT_SWITCH_2:
                this->storage->GetSettings().effects[this->current_effect.id].locked = !this->storage->GetSettings().effects[this->current_effect.id].locked;
                this->ResetFootSwitchData(id);

                return true;

            case PedalPCB::SWITCH_1:
                this->current_effect.switch1 = false;

                return true;

            case PedalPCB::SWITCH_2:
                this->current_effect.switch2 = false;

                return true;

            case PedalPCB::SWITCH_3:
                this->current_effect.switch3 = false;

                return true;

            case PedalPCB::SWITCH_4:
                this->current_effect.switch4 = false;

                return true;
            }

            return false;
        }

        void ProcessFootSwitches()
        {
            auto now = daisy::System::GetNow();
            uint32_t event;

            for (auto i = 0; i <= PedalPCB::FOOT_SWITCH_2; ++i)
            {
                event = (now - this->foot_switch_data[i].hold_time) / 1000;

                if (this->foot_switch_data[i].hold_time && this->foot_switch_data[i].events[event])
                {
                    switch (event)
                    {
                    case 0:
                        this->foot_switch_data[i].events[event] = false;

                        break;

                    case 1:
                    case 2:
                    case 3:
                        MessageBox(this, (i == PedalPCB::FOOT_SWITCH_1) ? "LOADING IN %u" : "SAVING IN %u", 4 - event);
                        this->foot_switch_data[i].events[event] = false;

                        break;

                    case 4:
                        if (i == PedalPCB::FOOT_SWITCH_1)
                        {
                            this->storage->RestoreDefaults();
                            OnInputAll();
                        }
                        else
                        {
                            this->storage->GetDefaultSettings() = this->storage->GetSettings();
                            this->storage->Save();

                            Draw();
                        }

                        this->ResetFootSwitchData(i);

                        break;
                    }
                }
            }
        }

        void ResetFootSwitchData(uint8_t id)
        {
            this->foot_switch_data[id].hold_time = 0;

            for (auto i = 0u; i < sizeof(this->foot_switch_data[id].events); ++i)
                this->foot_switch_data[id].events[i] = true;
        }

        void Splash()
        {
            SSD1351_fill(COLOR_BLACK);
            SSD1351_draw_bitmap(0, 0, 128, 128, SplashBitmap, COLOR);
            SSD1351_update();

            daisy::System::Delay(1500);
            OnInput();
        }
    };
}
