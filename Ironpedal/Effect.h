// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {

enum Id {
  EFFECT_MASTER,     // 0000
  EFFECT_OVERDRIVE,  // 0001
  EFFECT_LAST
};

union Type {
  Id id = EFFECT_MASTER;

  struct {
    bool switch4 : 1;
    bool switch3 : 1;
    bool switch2 : 1;
    bool switch1 : 1;
  };
};

struct Effect {
  bool enabled = false;
  bool locked = false;

  Type type;
};

}
