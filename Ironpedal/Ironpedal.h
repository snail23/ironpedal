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

  bool SwitchPressed() {
    auto inputReceived = false;

    // Handle foot switch 1

    if (this->buttons[FOOT_SWITCH_1].RisingEdge()) {
      this->footSwitch1TimeHeld = System::GetNow();
    } else if (this->buttons[FOOT_SWITCH_1].FallingEdge()) {
      auto now = System::GetNow();

      if (now - this->footSwitch1TimeHeld > 3000) {
        this->storage->RestoreDefaults();
        OnInputAll();

        this->footSwitch1TimeHeld = now;
      } else {
        this->storage->GetSettings().effects[this->currentEffect.id].enabled = !this->storage->GetSettings().effects[this->currentEffect.id].enabled;
        inputReceived = true;
      }
    }

    // Handle foot switch 2

    if (this->buttons[FOOT_SWITCH_2].RisingEdge()) {
      this->footSwitch2TimeHeld = System::GetNow();
    } else if (this->buttons[FOOT_SWITCH_2].FallingEdge()) {
      auto now = System::GetNow();

      if (now - this->footSwitch2TimeHeld > 3000) {
        this->storage->GetDefaultSettings() = this->storage->GetSettings();
        this->storage->Save();

        this->footSwitch2TimeHeld = now;
      } else
        this->storage->GetSettings().effects[this->currentEffect.id].locked = !this->storage->GetSettings().effects[this->currentEffect.id].locked;

      inputReceived = true;
    }

    // Handle switch 1

    if (this->buttons[SWITCH_1].RisingEdge()) {
      this->currentEffect.switch1 = true;
      inputReceived = true;
    } else if (this->buttons[SWITCH_1].FallingEdge()) {
      this->currentEffect.switch1 = false;
      inputReceived = true;
    }

    // Handle switch 2

    if (this->buttons[SWITCH_2].RisingEdge()) {
      this->currentEffect.switch2 = true;
      inputReceived = true;
    } else if (this->buttons[SWITCH_2].FallingEdge()) {
      this->currentEffect.switch2 = false;
      inputReceived = true;
    }

    // Handle switch 3

    if (this->buttons[SWITCH_3].RisingEdge()) {
      this->currentEffect.switch3 = true;
      inputReceived = true;
    } else if (this->buttons[SWITCH_3].FallingEdge()) {
      this->currentEffect.switch3 = false;
      inputReceived = true;
    }

    // Handle switch 4

    if (this->buttons[SWITCH_4].RisingEdge()) {
      this->currentEffect.switch4 = true;
      inputReceived = true;
    } else if (this->buttons[SWITCH_4].FallingEdge()) {
      this->currentEffect.switch4 = false;
      inputReceived = true;
    }

    return inputReceived;
  }

  Ironpedal() {
    this->currentEffect.id = Effect::EFFECT_MASTER;

    this->footSwitch1TimeHeld = 0;
    this->footSwitch2TimeHeld = 0;

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
    delete this->display;
    delete this->storage;

    this->display = 0;
    this->storage = 0;
  }

  void Loop() {
    bool inputReceived;

    uint32_t controlValues[this->numControls];
    uint32_t val;

    while (true) {
      this->ProcessAllControls();
      inputReceived = this->SwitchPressed();

      if (!this->storage->GetSettings().effects[this->currentEffect.id].locked) {
        for (auto i = 0; i < this->numControls; ++i) {
          val = (uint32_t)round(this->controls[i].Value() * 100.0f);

          if (controlValues[i] != val) {
            controlValues[i] = val;
            inputReceived = true;
          }
        }
      }

      if (inputReceived)
        OnInput();
    }
  }

private:
  uint32_t footSwitch1TimeHeld;
  uint32_t footSwitch2TimeHeld;
};
