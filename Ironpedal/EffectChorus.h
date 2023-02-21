// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Chorus {

daisysp::ChorusEngine Chorus;

Parameter Amount;
Parameter Feedback;
Parameter Gain;
Parameter Low;
Parameter Rate;
Parameter Sustain;

Svf Hpf;
Svf Lpf;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i) {
    Hpf.Process(Chorus.Process(in[i]));
    Lpf.Process(Hpf.High());

    out[i] = Lpf.Low() * Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6];
  }
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("SUST AMNT RATE");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u  %3u   %3u", (uint32_t)round(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] * 100.0f), (uint32_t)round(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_2] * 100.0f), (uint32_t)round(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] / 6.0f * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("FEED LOW  GAIN");

  auto low = Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_5] / 1000.0f;

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)round(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] * 100.0f), (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)round((Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] - 1.0f) * 100.0f));
  printlnCentered(buf);

  printFooter("CHORUS");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_CHORUS].locked) {
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1] = Sustain.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_2] = Amount.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3] = Rate.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4] = Feedback.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_5] = Low.Process();
    Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_6] = Gain.Process();
  }

  Chorus.SetFeedback(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
  Chorus.SetDelay(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
  Chorus.SetLfoDepth(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_2]);
  Chorus.SetLfoFreq(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);

  Lpf.SetFreq(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_5]);
}

void onSetup() {
  Amount.Init(Terrarium.controls[KNOB_2], 0.0f, 1.0f, Parameter::LINEAR);
  Feedback.Init(Terrarium.controls[KNOB_4], 0.0f, 1.0f, Parameter::LINEAR);
  Gain.Init(Terrarium.controls[KNOB_6], 0.0f, 2.0f, Parameter::LINEAR);
  Low.Init(Terrarium.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);
  Rate.Init(Terrarium.controls[KNOB_3], 0.01f, 6.0f, Parameter::LINEAR);
  Sustain.Init(Terrarium.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);

  Chorus.Init(Seed.AudioSampleRate());

  Chorus.SetFeedback(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_4]);
  Chorus.SetDelay(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_1]);
  Chorus.SetLfoDepth(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_2]);
  Chorus.SetLfoFreq(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_3]);

  Hpf.Init(Seed.AudioSampleRate());
  Hpf.SetDrive(0.0f);
  Hpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
  Hpf.SetRes(0.0f);

  Lpf.Init(Seed.AudioSampleRate());
  Lpf.SetDrive(0.0f);
  Lpf.SetFreq(Storage.GetSettings().effects[EFFECT_CHORUS].values[KNOB_5]);
  Lpf.SetRes(0.0f);
}

}
}
