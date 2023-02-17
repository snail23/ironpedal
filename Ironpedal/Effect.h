// Part of Ironpedal
// https://github.com/snail23/ironpedal

enum EffectType {
  MASTER,
  OVERDRIVE
};

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

union {
  EffectType type;

  struct {
    bool switch4 : 1;
    bool switch3 : 1;
    bool switch2 : 1;
    bool switch1 : 1;
  };
} CurrentEffect;
