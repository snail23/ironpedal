// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Reverb {

daisysp::ReverbSc Reverb;

Parameter Feedback;
Parameter Low;

void Draw() {
  char buf[16];
  PrintHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  PrintlnCentered("FEEDBACK");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%d", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2] * 100.0f));
  PrintlnCentered(buf);
  PrintlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  PrintlnCentered("LOW PASS");

  auto low = Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5] / 1000.0f;

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%u.%u KHZ", (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f));
  PrintlnCentered(buf);

  PrintFooter("REVERB");
}

void Init() {
  Feedback.Init(Ironpedal.controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
  Low.Init(Ironpedal.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

  Reverb.Init(Ironpedal.AudioSampleRate());

  Reverb.SetFeedback(Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
  Reverb.SetLpFreq(Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5]);
}

void OnAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    Reverb.Process(in[i], &out[i]);
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].locked) {
    Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2] = Feedback.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5] = Low.Process();
  }

  Reverb.SetFeedback(Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_2]);
  Reverb.SetLpFreq(Ironpedal.storage->GetSettings().effects[EFFECT_REVERB].values[KNOB_5]);
}

}
}
