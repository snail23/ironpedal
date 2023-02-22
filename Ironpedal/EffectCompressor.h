// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Compressor {

daisysp::Compressor Compressor;

Parameter Attack;
Parameter Ratio;
Parameter Release;
Parameter Threshold;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Compressor.Process(in[i]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("THRESH   RATIO");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3d dB    %2u:1", (int32_t)Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1], (uint32_t)Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("ATTACK RELEASE");

  Display.setTextColor(COLOR);
  sprintf(buf, "%2u.%03u  %2u.%03u", (uint32_t)Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4], (uint32_t)((Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4] - floor(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4])) * 1000.0f), (uint32_t)Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6], (uint32_t)((Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6] - floor(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6])) * 1000.0f));
  printlnCentered(buf);

  printFooter("COMPRESSOR");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_COMPRESSOR].locked) {
    Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1] = Threshold.Process();
    Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3] = Ratio.Process();
    Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4] = Attack.Process();
    Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6] = Release.Process();
  }

  Compressor.SetAttack(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4]);
  Compressor.SetRatio(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
  Compressor.SetRelease(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6]);
  Compressor.SetThreshold(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1]);
}

void onSetup() {
  Attack.Init(Terrarium.controls[KNOB_4], 0.001f, 10.0f, Parameter::LINEAR);
  Ratio.Init(Terrarium.controls[KNOB_3], 1.0f, 40.0f, Parameter::LINEAR);
  Release.Init(Terrarium.controls[KNOB_6], 0.001f, 10.0f, Parameter::LINEAR);
  Threshold.Init(Terrarium.controls[KNOB_1], 0.0f, -80.0f, Parameter::LINEAR);

  Compressor.Init(Seed.AudioSampleRate());

  Compressor.SetAttack(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_4]);
  Compressor.SetRatio(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_3]);
  Compressor.SetRelease(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_6]);
  Compressor.SetThreshold(Storage.GetSettings().effects[EFFECT_COMPRESSOR].values[KNOB_1]);
}

}
}
