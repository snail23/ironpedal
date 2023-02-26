// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Autowah {

daisysp::Autowah Autowah;

Parameter Blend;
Parameter Wah;

void Draw() {
  char buf[16];
  printHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("BLEND");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%u", (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
  printlnCentered(buf);
  printlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("WAH");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%u", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5] * 100.0f));
  printlnCentered(buf);

  printFooter("AUTOWAH");
}

void Init() {
  Blend.Init(Ironpedal.controls[KNOB_2], 0.0f, 100.0f, Parameter::LINEAR);
  Wah.Init(Ironpedal.controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

  Autowah.Init(Ironpedal.AudioSampleRate());

  Autowah.SetDryWet(100.0f - Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
  Autowah.SetLevel(1.0f);
  Autowah.SetWah(Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5]);
}

void OnAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Autowah.Process(in[i]);
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].locked) {
    Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2] = Blend.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5] = Wah.Process();
  }

  Autowah.SetDryWet(100.0f - Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_2]);
  Autowah.SetWah(Ironpedal.storage->GetSettings().effects[EFFECT_AUTOWAH].values[KNOB_5]);
}

}
}
