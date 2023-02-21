// Part of Ironpedal
// https://github.com/snail23/ironpedal

struct StorageData {
  Effect::Effect effects[Effect::EFFECT_LAST];

  bool operator==(StorageData &rhs) {
    return this->effects == rhs.effects;
  }

  bool operator!=(StorageData &rhs) {
    return !this->operator==(rhs);
  }
};

Adafruit_SSD1351 Display(SSD1351WIDTH, SSD1351HEIGHT, PIN_CS, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCK, PIN_RST);  // TODO: Make this hardware accelerated
AudioClass Seed;

DaisyHardware Terrarium;
Effect::Type CurrentEffect;
QSPIHandle QSPI;
PersistentStorage<StorageData> Storage(QSPI);

uint32_t FootSwitch1TimeHeld = 0;
uint32_t FootSwitch2TimeHeld = 0;
