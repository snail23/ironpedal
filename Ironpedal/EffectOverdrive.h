// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Overdrive {

daisysp::Overdrive Overdrive;

Parameter Blend;
Parameter Drive;
Parameter Gain;
Parameter High;
Parameter Low;

Svf Hpf;
Svf Lpf;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i) {
    Hpf.Process(Overdrive.Process(in[i]));
    Lpf.Process(Hpf.High());

    out[i] = (in[i] * Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_1] + Lpf.Low() * (1.0f - Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_1])) * Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_6];
  }
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("BLEND    DRIVE");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u        %3u", (uint32_t)(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_1] * 100.0f), (uint32_t)(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_3] * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("HIGH LOW  GAIN");

  auto low = Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] / 1000.0f;

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_4], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)((Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_6] - 1.0f) * 100.0f));
  printlnCentered(buf);

  printFooter("OVERDRIVE");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_OVERDRIVE].locked) {
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_1] = Blend.Process();
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_3] = Drive.Process();
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_4] = High.Process();
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5] = Low.Process();
    Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_6] = Gain.Process();
  }

  Overdrive.SetDrive(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_3]);
  Hpf.SetFreq(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_4]);
  Lpf.SetFreq(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
}

void onSetup() {
  Blend.Init(Terrarium.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Drive.Init(Terrarium.controls[KNOB_3], 0.0f, 1.0f, Parameter::LINEAR);
  Gain.Init(Terrarium.controls[KNOB_6], 0.0f, 2.0f, Parameter::LINEAR);
  High.Init(Terrarium.controls[KNOB_4], 20.0f, 160.0f, Parameter::LINEAR);
  Low.Init(Terrarium.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

  Hpf.Init(Seed.AudioSampleRate());
  Hpf.SetDrive(0.0f);
  Hpf.SetFreq(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_4]);
  Hpf.SetRes(0.0f);

  Lpf.Init(Seed.AudioSampleRate());
  Lpf.SetDrive(0.0f);
  Lpf.SetFreq(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_5]);
  Lpf.SetRes(0.0f);

  Overdrive.SetDrive(Storage.GetSettings().effects[EFFECT_OVERDRIVE].values[KNOB_3]);
}

}
}
