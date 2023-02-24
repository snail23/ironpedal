// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Overdrive {

daisysp::Overdrive Overdrive;

Parameter Blend;
Parameter Drive;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = in[i] * Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_1] + Overdrive.Process(in[i]) * (1.0f - Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_1]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("BLEND");

  Display.setTextColor(COLOR);
  sprintf(buf, "%u", (uint32_t)(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("DRIVE");

  Display.setTextColor(COLOR);
  sprintf(buf, "%u", (uint32_t)(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] * 100.0f));
  printlnCentered(buf);

  printFooter("OVERDRIVE");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_OVERDRIVE].locked) {
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_2] = Blend.Process();
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] = Drive.Process();
  }

  Overdrive.SetDrive(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
}

void onSetup() {
  Blend.Init(Terrarium.controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
  Drive.Init(Terrarium.controls[KNOB_5], 0.0f, 1.0f, Parameter::LINEAR);

  Overdrive.SetDrive(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
}

}
}
