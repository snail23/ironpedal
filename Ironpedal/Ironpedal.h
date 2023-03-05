// Part of Ironpedal
// https://github.com/snail23/ironpedal

#define PIN_CS D3
#define PIN_RST D30

#define PIN_SPI_MISO D1
#define PIN_SPI_MOSI D6
#define PIN_SPI_SCK D2

struct StorageData
{
    Effect::Effect effects[Effect::EFFECT_LAST];

    bool operator==(StorageData &rhs)
    {
        return this->effects == rhs.effects;
    }

    bool operator!=(StorageData &rhs)
    {
        return !this->operator==(rhs);
    }
};

void Draw();
void MessageBox(const char *text, uint32_t event);

void OnInit();
void OnInput();
void OnInputAll();

class Ironpedal : public Terrarium
{
public:
    Adafruit_SSD1351 *display;
    Effect::Type currentEffect;
    PersistentStorage<StorageData> *storage;

    Ironpedal()
    {
        this->controlValues = new uint32_t[this->numControls];
        this->currentEffect.id = Effect::EFFECT_MASTER;

        for (auto i = 0; i <= FOOT_SWITCH_2; ++i)
            this->ResetFootSwitchData(i);

        this->storage = new PersistentStorage<StorageData>(this->qspi);
        this->storage->Init({});
        this->storage->GetDefaultSettings() = this->storage->GetSettings();

        OnInit();

        this->display = new Adafruit_SSD1351(SSD1351WIDTH, SSD1351HEIGHT, PIN_CS, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCK, PIN_RST);
        this->display->begin();
        this->display->setFont(&Px437_IBM_VGA_8x148pt7b);

        this->display->fillScreen(0);
        this->display->drawBitmap(0, 0, SplashBitmap, SSD1351WIDTH, SSD1351HEIGHT, COLOR);

        System::Delay(1500);
        OnInput();
    }

    void Loop()
    {
        bool inputReceived;

        while (true)
        {
            this->ProcessAllControls();
            this->ProcessFootSwitches();

            inputReceived = false;

            if (this->IsSwitchPressed())
                inputReceived = true;

            if (!this->storage->GetSettings().effects[this->currentEffect.id].locked && this->HasControlChanged())
                inputReceived = true;

            if (inputReceived)
                OnInput();
        }
    }

private:
    uint32_t *controlValues;

    struct
    {
        bool events[5];
        uint32_t holdTime;
    } footSwitchData[FOOT_SWITCH_2 + 1];

    bool HasControlChanged()
    {
        auto inputReceived = false;
        uint32_t val;

        for (auto i = 0; i < this->numControls; ++i)
        {
            val = (uint32_t)round(this->controls[i].Value() * 100.0f);

            if (this->controlValues[i] != val)
            {
                this->controlValues[i] = val;
                inputReceived = true;
            }
        }

        return inputReceived;
    }

    bool IsSwitchPressed()
    {
        auto inputReceived = false;
        auto now = System::GetNow();

        for (auto i = 0; i < this->numSwitches; ++i)
        {
            if (this->buttons[i].RisingEdge())
            {
                if (this->SwitchPressed(i))
                    inputReceived = true;
            }
            else if (this->buttons[i].FallingEdge())
            {
                if (i <= FOOT_SWITCH_2)
                {
                    if (now - this->footSwitchData[i].holdTime < 4000)
                        inputReceived = this->SwitchReleased(i);

                    else
                    {
                        this->ResetFootSwitchData(i);
                        inputReceived = true;
                    }
                }
                else
                    inputReceived = this->SwitchReleased(i);
            }
        }

        return inputReceived;
    }

    bool SwitchPressed(uint8_t id)
    {
        switch (id)
        {
        case FOOT_SWITCH_1:
        case FOOT_SWITCH_2:
            this->footSwitchData[id].holdTime = System::GetNow();

            break;

        case SWITCH_1:
            this->currentEffect.switch1 = true;

            return true;

        case SWITCH_2:
            this->currentEffect.switch2 = true;

            return true;

        case SWITCH_3:
            this->currentEffect.switch3 = true;

            return true;

        case SWITCH_4:
            this->currentEffect.switch4 = true;

            return true;
        }

        return false;
    }

    bool SwitchReleased(uint8_t id)
    {
        switch (id)
        {
        case FOOT_SWITCH_1:
            this->storage->GetSettings().effects[this->currentEffect.id].enabled = !this->storage->GetSettings().effects[this->currentEffect.id].enabled;
            this->ResetFootSwitchData(id);

            return true;

        case FOOT_SWITCH_2:
            this->storage->GetSettings().effects[this->currentEffect.id].locked = !this->storage->GetSettings().effects[this->currentEffect.id].locked;
            this->ResetFootSwitchData(id);

            return true;

        case SWITCH_1:
            this->currentEffect.switch1 = false;

            return true;

        case SWITCH_2:
            this->currentEffect.switch2 = false;

            return true;

        case SWITCH_3:
            this->currentEffect.switch3 = false;

            return true;

        case SWITCH_4:
            this->currentEffect.switch4 = false;

            return true;
        }

        return false;
    }

    void ProcessFootSwitches()
    {
        auto now = System::GetNow();
        uint32_t event;

        for (auto i = 0; i <= FOOT_SWITCH_2; ++i)
        {
            event = (now - this->footSwitchData[i].holdTime) / 1000;

            if (this->footSwitchData[i].holdTime && this->footSwitchData[i].events[event])
            {
                switch (event)
                {
                case 0:
                    this->footSwitchData[i].events[event] = false;

                    break;

                case 1:
                case 2:
                case 3:
                    MessageBox((i == FOOT_SWITCH_1) ? "LOADING IN %u" : "SAVING IN %u", 4 - event);
                    this->footSwitchData[i].events[event] = false;

                    break;

                case 4:
                    if (i == FOOT_SWITCH_1)
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
        this->footSwitchData[id].holdTime = 0;

        for (auto i = 0; i < sizeof(this->footSwitchData[id].events); ++i)
            this->footSwitchData[id].events[i] = true;
    }
};
