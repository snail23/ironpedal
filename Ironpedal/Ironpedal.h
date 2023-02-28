// Part of Ironpedal
// https://github.com/snail23/ironpedal

#define PIN_CS D3
#define PIN_RST D30

#define PIN_SPI_MISO D1
#define PIN_SPI_MOSI D6
#define PIN_SPI_SCK D2

struct StorageData {
  Effect::Effect effects[Effect::EFFECT_LAST];

  bool operator==(StorageData &rhs) {
    return this->effects == rhs.effects;
  }

  bool operator!=(StorageData &rhs) {
    return !this->operator==(rhs);
  }
};

void OnInit();
void OnInput();
void OnInputAll();

class Ironpedal : public Terrarium {
public:
  Adafruit_SSD1351 *display;
  Effect::Type currentEffect;
  PersistentStorage<StorageData> *storage;

  Ironpedal() {
    this->controlValues = new uint32_t[this->numControls];
    this->currentEffect.id = Effect::EFFECT_MASTER;

    for (auto i = 0; i <= FOOT_SWITCH_2; ++i)
      this->footSwitchHoldTimes[i] = 0;

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

  ~Ironpedal() {
    delete[] this->controlValues;
    delete this->display;
    delete this->storage;

    this->controlValues = 0;
    this->display = 0;
    this->storage = 0;
  }

  void Loop() {
    bool inputReceived;

    while (true) {
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
  uint32_t footSwitchHoldTimes[FOOT_SWITCH_2 + 1];

  bool HasControlChanged() {
    auto inputReceived = false;
    uint32_t val;

    for (auto i = 0; i < this->numControls; ++i) {
      val = (uint32_t)round(this->controls[i].Value() * 100.0f);

      if (this->controlValues[i] != val) {
        this->controlValues[i] = val;
        inputReceived = true;
      }
    }

    return inputReceived;
  }

  bool IsSwitchPressed() {
    auto inputReceived = false;
    auto now = System::GetNow();

    for (auto i = 0; i < this->numSwitches; ++i) {
      if (this->buttons[i].RisingEdge()) {
        if (this->SwitchPressed(i))
          inputReceived = true;
      } else if (this->buttons[i].FallingEdge()) {
        if (i <= FOOT_SWITCH_2) {
          if (now - this->footSwitchHoldTimes[i] <= 1000 && this->SwitchReleased(i)) {
            this->footSwitchHoldTimes[i] = 0;
            inputReceived = true;
          }
        } else if (this->SwitchReleased(i))
          inputReceived = true;
      }
    }

    return inputReceived;
  }

  bool SwitchPressed(uint8_t id) {
    switch (id) {
      case FOOT_SWITCH_1:
      case FOOT_SWITCH_2:
        this->footSwitchHoldTimes[id] = System::GetNow();

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

  bool SwitchReleased(uint8_t id) {
    switch (id) {
      case FOOT_SWITCH_1:
        this->storage->GetSettings().effects[this->currentEffect.id].enabled = !this->storage->GetSettings().effects[this->currentEffect.id].enabled;

        return true;

      case FOOT_SWITCH_2:
        this->storage->GetSettings().effects[this->currentEffect.id].locked = !this->storage->GetSettings().effects[this->currentEffect.id].locked;

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

  void ProcessFootSwitches() {
    auto now = System::GetNow();

    for (auto i = 0; i <= FOOT_SWITCH_2; ++i) {
      if (this->footSwitchHoldTimes[i] && now - this->footSwitchHoldTimes[i] > 3000) {
        if (i == FOOT_SWITCH_1) {
          this->storage->RestoreDefaults();
          OnInputAll();
        } else {
          this->storage->GetDefaultSettings() = this->storage->GetSettings();
          this->storage->Save();
        }

        this->footSwitchHoldTimes[i] = 0;
      }
    }
  }
};
