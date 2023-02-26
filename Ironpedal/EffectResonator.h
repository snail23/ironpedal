// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Resonator {

daisysp::Resonator Resonator;

Parameter Brightness;
Parameter Decay;
Parameter Frequency;
Parameter Stiffness;

void Draw() {
  char buf[16];
  printHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("DECAY     FREQ");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%3u     %3u HZ", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1] * 100.0f), (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
  printlnCentered(buf);
  printlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("STIFF   BRIGHT");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%3u        %3u", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4] * 100.0f), (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6] * 100.0f));
  printlnCentered(buf);

  printFooter("RESONATOR");
}

void Init() {
  Brightness.Init(Ironpedal.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
  Decay.Init(Ironpedal.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Frequency.Init(Ironpedal.controls[KNOB_3], 65.0f, 880.0f, Parameter::LINEAR);
  Stiffness.Init(Ironpedal.controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);

  Resonator.Init(0.0f, 24, Ironpedal.AudioSampleRate());

  Resonator.SetBrightness(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6]);
  Resonator.SetDamping(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1]);
  Resonator.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
  Resonator.SetStructure(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4]);
}

void OnAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Resonator.Process(in[i]);
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].locked) {
    Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1] = Decay.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3] = Frequency.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4] = Stiffness.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6] = Brightness.Process();
  }

  Resonator.SetBrightness(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_6]);
  Resonator.SetDamping(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_1]);
  Resonator.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_3]);
  Resonator.SetStructure(Ironpedal.storage->GetSettings().effects[EFFECT_RESONATOR].values[KNOB_4]);
}

}
}
