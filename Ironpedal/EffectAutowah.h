// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Autowah {

daisysp::Autowah Autowah;

Parameter Blend;
Parameter Wah;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i) 
    out[i] = Autowah.Process(in[i]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("BLEND");

  Display.setTextColor(COLOR);
  sprintf(buf, "%u", (uint32_t)Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("WAH");

  Display.setTextColor(COLOR);
  sprintf(buf, "%u", (uint32_t)(Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5] * 100.0f));
  printlnCentered(buf);

  printFooter("AUTOWAH");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_AUTOWAH].locked) {
    Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2] = Blend.Process();
    Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5] = Wah.Process();
  }

  Autowah.SetDryWet(100.0f - Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
  Autowah.SetWah(Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5]);
}

void onSetup() {
  Blend.Init(Terrarium.controls[KNOB_2], 0.0f, 100.0f, Parameter::LINEAR);
  Wah.Init(Terrarium.controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

  Autowah.Init(Seed.AudioSampleRate());

  Autowah.SetDryWet(100.0f - Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
  Autowah.SetLevel(1.0f);
  Autowah.SetWah(Storage.GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5]);
}

}
}
