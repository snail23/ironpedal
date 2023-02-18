// Part of Ironpedal
// https://github.com/snail23/ironpedal

enum TerrariumKnob {
  KNOB_1,
  KNOB_2,
  KNOB_3,
  KNOB_4,
  KNOB_5,
  KNOB_6
};

enum TerrariumLed {
  LED_1,
  LED_2
};

enum TerrariumSwitch {
  FOOT_SWITCH_1,
  FOOT_SWITCH_2,

  SWITCH_1,
  SWITCH_2,
  SWITCH_3,
  SWITCH_4
};

Adafruit_SSD1351 Display = Adafruit_SSD1351(SSD1351WIDTH, SSD1351HEIGHT, PIN_CS, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCK, PIN_RST);  // TODO: Make this hardware accelerated
AudioClass Seed;

DaisyHardware Terrarium;

Effect::Effect Effects[Effect::EFFECT_LAST];
Effect::Type CurrentEffect;