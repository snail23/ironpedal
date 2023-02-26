// Part of Ironpedal
// https://github.com/snail23/ironpedal

#define PIN_FOOT_SWITCH_1 D25
#define PIN_FOOT_SWITCH_2 D26

#define PIN_KNOB_1 A1
#define PIN_KNOB_2 A2
#define PIN_KNOB_3 A3
#define PIN_KNOB_4 A4
#define PIN_KNOB_5 A5
#define PIN_KNOB_6 A6

#define PIN_LED_1 A7
#define PIN_LED_2 A8

#define PIN_QSPI_CLK dsy_pin(DSY_GPIOF, 10)
#define PIN_QSPI_IO0 dsy_pin(DSY_GPIOF, 8)
#define PIN_QSPI_IO1 dsy_pin(DSY_GPIOF, 9)
#define PIN_QSPI_IO2 dsy_pin(DSY_GPIOF, 7)
#define PIN_QSPI_IO3 dsy_pin(DSY_GPIOF, 6)
#define PIN_QSPI_NCS dsy_pin(DSY_GPIOG, 6)

#define PIN_SWITCH_1 D10
#define PIN_SWITCH_2 D9
#define PIN_SWITCH_3 D8
#define PIN_SWITCH_4 D7

enum TerrariumKnob {
  KNOB_1,
  KNOB_2,
  KNOB_3,
  KNOB_4,
  KNOB_5,
  KNOB_6,
  KNOB_LAST
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

void OnAudio(float **in, float **out, size_t size);

class Terrarium : public AudioClass, public DaisyHardware {
public:
  Led systemLed;
  QSPIHandle qspi;

  Terrarium() {
    this->systemLed.Init(LED_BUILTIN, false);
    this->systemLed.Set(true);

    *(DaisyHardware *)this = this->init(DAISY_SEED);

    this->num_channels = 1;
    this->numControls = 6;
    this->numLeds = 2;
    this->numSwitches = 6;

    this->buttons[FOOT_SWITCH_1].Init(1000.0f, true, PIN_FOOT_SWITCH_1, INPUT_PULLUP);
    this->buttons[FOOT_SWITCH_2].Init(1000.0f, true, PIN_FOOT_SWITCH_2, INPUT_PULLUP);

    this->buttons[SWITCH_1].Init(1000.0f, true, PIN_SWITCH_1, INPUT_PULLUP);
    this->buttons[SWITCH_2].Init(1000.0f, true, PIN_SWITCH_2, INPUT_PULLUP);
    this->buttons[SWITCH_3].Init(1000.0f, true, PIN_SWITCH_3, INPUT_PULLUP);
    this->buttons[SWITCH_4].Init(1000.0f, true, PIN_SWITCH_4, INPUT_PULLUP);

    this->controls[KNOB_1].Init(PIN_KNOB_1, 1000.0f);
    this->controls[KNOB_2].Init(PIN_KNOB_2, 1000.0f);
    this->controls[KNOB_3].Init(PIN_KNOB_3, 1000.0f);
    this->controls[KNOB_4].Init(PIN_KNOB_4, 1000.0f);
    this->controls[KNOB_5].Init(PIN_KNOB_5, 1000.0f);
    this->controls[KNOB_6].Init(PIN_KNOB_6, 1000.0f);

    this->leds[LED_1].Init(PIN_LED_1, false);
    this->leds[LED_2].Init(PIN_LED_2, false);

    this->SetAudioBlockSize(128);
    QSPIHandle::Config qspiConfig;

    qspiConfig.device = QSPIHandle::Config::IS25LP064A;
    qspiConfig.mode = QSPIHandle::Config::MEMORY_MAPPED;

    qspiConfig.pin_config.clk = PIN_QSPI_CLK;
    qspiConfig.pin_config.io0 = PIN_QSPI_IO0;
    qspiConfig.pin_config.io1 = PIN_QSPI_IO1;
    qspiConfig.pin_config.io2 = PIN_QSPI_IO2;
    qspiConfig.pin_config.io3 = PIN_QSPI_IO3;
    qspiConfig.pin_config.ncs = PIN_QSPI_NCS;

    this->qspi.Init(qspiConfig);

    this->StartAudio(OnAudio);
    this->systemLed.Set(false);
  }
};
