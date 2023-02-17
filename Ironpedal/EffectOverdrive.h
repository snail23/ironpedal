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

    out[i] = Lpf.Low() * Gain.Value();
  }
}

void onDraw() {
  char buf[16];

  Display.fillScreen(0);
  Display.setCursor(0, Px437_IBM_VGA_8x148pt7b.yAdvance);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("BLEND    DRIVE");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u        %3u", (uint32_t)round(Blend.Value() * 100.0f), (uint32_t)round(Drive.Value() * 100.0f));
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("HIGH LOW  GAIN");

  auto low = Low.Value() / 1000.0f;

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u %2u.%uK  %3u", (uint32_t)round(High.Value()), (uint32_t)low, (uint32_t)((low - floor(low)) * 10.0f), (uint32_t)round(Gain.Value() * 100.0f));
  printlnCentered(buf);

  printFooter("OVERDRIVE");
}

void onInput() {
  Blend.Process();
  Overdrive.SetDrive(Drive.Process());
  Gain.Process();
  Hpf.SetFreq(High.Process());
  Lpf.SetFreq(Low.Process());
}

void onSetup() {
  Blend.Init(Terrarium.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Drive.Init(Terrarium.controls[KNOB_3], 0.0f, 1.0f, Parameter::LINEAR);
  Gain.Init(Terrarium.controls[KNOB_6], 0.0f, 1.0f, Parameter::LINEAR);
  High.Init(Terrarium.controls[KNOB_4], 20.0f, 160.0f, Parameter::LINEAR);
  Low.Init(Terrarium.controls[KNOB_5], 1280.0f, 10200.0f, Parameter::LINEAR);

  Hpf.Init(Seed.AudioSampleRate());
  Hpf.SetRes(0.0f);
  Hpf.SetDrive(0.0f);

  Lpf.Init(Seed.AudioSampleRate());
  Lpf.SetRes(0.0f);
  Lpf.SetDrive(0.0f);
}

}
}
