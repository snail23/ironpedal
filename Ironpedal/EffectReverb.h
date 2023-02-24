// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Reverb {

daisysp::ReverbSc Reverb;

Parameter Feedback;
Parameter Low;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    Reverb.Process(in[i], &out[i]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("FEEDBACK");

  Display.setTextColor(COLOR);
  sprintf(buf, "%d", (uint32_t)(Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_2] * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("LOW PASS");

  auto low = Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_5] / 1000.0f;

  Display.setTextColor(COLOR);
  sprintf(buf, "%u.%u KHZ", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
  printlnCentered(buf);

  printFooter("REVERB");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_REVERB].locked) {
    Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_2] = Feedback.Process();
    Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_5] = Low.Process();
  }

  Reverb.SetFeedback(Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
  Reverb.SetLpFreq(Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_5]);
}

void onSetup() {
  Feedback.Init(Terrarium.controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
  Low.Init(Terrarium.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

  Reverb.Init(Seed.AudioSampleRate());

  Reverb.SetFeedback(Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
  Reverb.SetLpFreq(Storage.GetSettings().effects[EFFECT_REVERB].values[KNOB_5]);
}

}
}
