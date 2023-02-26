// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Compressor {

daisysp::Compressor Compressor;

Parameter Attack;
Parameter Ratio;
Parameter Release;
Parameter Threshold;

void Draw() {
  char buf[16];
  printHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("THRESH   RATIO");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%3d dB    %2u:1", (int32_t)Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1], (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
  printlnCentered(buf);
  printlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("ATTACK RELEASE");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%2u.%03u  %2u.%03u", (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4], (uint32_t)((Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4] - floor(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4])) * 1000.0f), (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6], (uint32_t)((Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6] - floor(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6])) * 1000.0f));
  printlnCentered(buf);

  printFooter("COMPRESSOR");
}

void Init() {
  Attack.Init(Ironpedal.controls[KNOB_4], 0.001f, 10.0f, Parameter::LINEAR);
  Ratio.Init(Ironpedal.controls[KNOB_3], 1.0f, 40.0f, Parameter::LINEAR);
  Release.Init(Ironpedal.controls[KNOB_6], 0.001f, 10.0f, Parameter::LINEAR);
  Threshold.Init(Ironpedal.controls[KNOB_1], 0.0f, -80.0f, Parameter::LINEAR);

  Compressor.Init(Ironpedal.AudioSampleRate());

  Compressor.SetAttack(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4]);
  Compressor.SetRatio(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
  Compressor.SetRelease(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6]);
  Compressor.SetThreshold(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1]);
}

void OnAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Compressor.Process(in[i]);
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].locked) {
    Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1] = Threshold.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3] = Ratio.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4] = Attack.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6] = Release.Process();
  }

  Compressor.SetAttack(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4]);
  Compressor.SetRatio(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
  Compressor.SetRelease(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6]);
  Compressor.SetThreshold(Ironpedal.storage->GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1]);
}

}
}
