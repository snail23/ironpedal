// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Master {

Parameter PostHigh;
Parameter PostLow;

Parameter PreGain;
Parameter PreHigh;
Parameter PreLow;

Parameter Volume;

Svf PostHpf;
Svf PostLpf;

Svf PreHpf;
Svf PreLpf;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i) {
    PreHpf.Process(in[i]);
    PreLpf.Process(PreHpf.High());

    out[i] = PreLpf.Low() * Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_3];
  }
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("HIGH LOW  GAIN");

  auto low = Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_2] / 1000.0f;

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_1], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)((Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_3] - 1.0f) * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("HIGH LOW   VOL");

  low = Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_5] / 1000.0f;

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u %2u.%uK %4d", (uint32_t)Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_4], (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (int32_t)((Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_6] - 1.0f) * 100.0f));
  printlnCentered(buf);

  printFooter("MASTER");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_MASTER].locked) {
    Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_1] = PreHigh.Process();
    Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_2] = PreLow.Process();
    Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_3] = PreGain.Process();
    Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_4] = PostHigh.Process();
    Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_5] = PostLow.Process();
    Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_6] = Volume.Process();
  }

  PostHpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
  PostLpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_5]);

  PreHpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_1]);
  PreLpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_2]);
}

void onPostAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] *= Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_6];
}

void onSetup() {
  PostHigh.Init(Terrarium.controls[KNOB_4], 20.0f, 160.0f, Parameter::LINEAR);
  PostLow.Init(Terrarium.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

  PreGain.Init(Terrarium.controls[KNOB_3], 0.0f, 2.0f, Parameter::LINEAR);
  PreHigh.Init(Terrarium.controls[KNOB_1], 20.0f, 160.0f, Parameter::LINEAR);
  PreLow.Init(Terrarium.controls[KNOB_2], 1280.0f, 10200.0f, Parameter::LINEAR);

  Volume.Init(Terrarium.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);

  PostHpf.Init(Seed.AudioSampleRate());
  PostHpf.SetDrive(0.0f);
  PostHpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_4]);
  PostHpf.SetRes(0.0f);

  PostLpf.Init(Seed.AudioSampleRate());
  PostLpf.SetDrive(0.0f);
  PostLpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_5]);
  PostLpf.SetRes(0.0f);

  PreHpf.Init(Seed.AudioSampleRate());
  PreHpf.SetDrive(0.0f);
  PreHpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_1]);
  PreHpf.SetRes(0.0f);

  PreLpf.Init(Seed.AudioSampleRate());
  PreLpf.SetDrive(0.0f);
  PreLpf.SetFreq(Storage.GetSettings().effects[EFFECT_MASTER].values[KNOB_2]);
  PreLpf.SetRes(0.0f);
}

}
}
