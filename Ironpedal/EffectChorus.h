// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Chorus {

daisysp::ChorusEngine Chorus;

Parameter Depth;
Parameter Delay;
Parameter Feedback;
Parameter Rate;

void Draw() {
  char buf[16];
  printHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("DELAY     RATE");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%3u       %u.%02u", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] * 100.0f), (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3], (uint32_t)((Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] - floor(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3])) * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("FEEDBACK DEPTH");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%3u        %3d", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] * 100.0f), (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] * 100.0f));
  printlnCentered(buf);

  printFooter("CHORUS");
}

void Init() {
  Delay.Init(Ironpedal.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Depth.Init(Ironpedal.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
  Feedback.Init(Ironpedal.controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);
  Rate.Init(Ironpedal.controls[KNOB_3], 0.01f, 6.0f, Parameter::LINEAR);

  Chorus.Init(Ironpedal.AudioSampleRate());

  Chorus.SetDelay(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
  Chorus.SetFeedback(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
  Chorus.SetLfoDepth(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6]);
  Chorus.SetLfoFreq(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);
}

void OnAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Chorus.Process(in[i]);
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].locked) {
    Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] = Delay.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] = Rate.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] = Feedback.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] = Depth.Process();
  }

  Chorus.SetDelay(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
  Chorus.SetFeedback(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
  Chorus.SetLfoDepth(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_6]);
  Chorus.SetLfoFreq(Ironpedal.storage->GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);
}

}
}
