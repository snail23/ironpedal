// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Resonator {

daisysp::Resonator Resonator;

Parameter Brightness;
Parameter Decay;
Parameter Rate;
Parameter Stiffness;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Resonator.Process(in[i]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("DECAY     FREQ");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u     %3u HZ", (uint32_t)(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1] * 100.0f), (uint32_t)Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("STIFF   BRIGHT");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u        %3u", (uint32_t)(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4] * 100.0f), (uint32_t)(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6] * 100.0f));
  printlnCentered(buf);

  printFooter("RESONATOR");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_RESONATOR].locked) {
    Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1] = Decay.Process();
    Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3] = Rate.Process();
    Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4] = Stiffness.Process();
    Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6] = Brightness.Process();
  }

  Resonator.SetBrightness(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6]);
  Resonator.SetDamping(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1]);
  Resonator.SetFreq(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
  Resonator.SetStructure(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4]);
}

void onSetup() {
  Brightness.Init(Terrarium.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
  Decay.Init(Terrarium.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Rate.Init(Terrarium.controls[KNOB_3], 110.0f, 880.0f, Parameter::LINEAR);
  Stiffness.Init(Terrarium.controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);

  Resonator.Init(0.0f, 24, Seed.AudioSampleRate());

  Resonator.SetBrightness(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6]);
  Resonator.SetDamping(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1]);
  Resonator.SetFreq(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
  Resonator.SetStructure(Storage.GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4]);
}

}
}
