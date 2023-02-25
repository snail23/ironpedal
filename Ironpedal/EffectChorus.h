// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Chorus {

daisysp::ChorusEngine Chorus;

Parameter Depth;
Parameter Delay;
Parameter Feedback;
Parameter Rate;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i) 
    out[i] = Chorus.Process(in[i]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("DELAY     RATE");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u       %u.%02u", (uint32_t)(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] * 100.0f), (uint32_t)Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3], (uint32_t)((Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] - floor(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3])) * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("FEEDBACK DEPTH");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u        %3d", (uint32_t)(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] * 100.0f), (uint32_t)(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] * 100.0f));
  printlnCentered(buf);

  printFooter("CHORUS");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_CHORUS].locked) {
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] = Delay.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] = Rate.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] = Feedback.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] = Depth.Process();
  }

  Chorus.SetDelay(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
  Chorus.SetFeedback(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
  Chorus.SetLfoDepth(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6]);
  Chorus.SetLfoFreq(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);
}

void onSetup() {
  Delay.Init(Terrarium.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Depth.Init(Terrarium.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
  Feedback.Init(Terrarium.controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);
  Rate.Init(Terrarium.controls[KNOB_3], 0.01f, 6.0f, Parameter::LINEAR);

  Chorus.Init(Seed.AudioSampleRate());

  Chorus.SetDelay(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
  Chorus.SetFeedback(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
  Chorus.SetLfoDepth(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6]);
  Chorus.SetLfoFreq(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);
}

}
}
