// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Misc {

Metro Metronome;
Parameter MetronomeBPM;

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("IRONPEDAL");

  Display.setTextColor(COLOR);
  printlnCentered("VER " VERSION);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("METRONOME");

  auto bpm = (uint32_t)(Storage.GetSettings().effects[EFFECT_MISC].values[KNOB_6] * 60.0f);
  Display.setTextColor(COLOR);

  if (bpm)
    sprintf(buf, "%u BPM", bpm);

  else
    sprintf(buf, "OFF");

  printlnCentered(buf);
  printFooter("MISC");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_MISC].locked)
    Storage.GetSettings().effects[EFFECT_MISC].values[KNOB_6] = MetronomeBPM.Process();

  Metronome.SetFreq(Storage.GetSettings().effects[EFFECT_MISC].values[KNOB_6]);
}

void onPostAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] += Metronome.Process();
}

void onSetup() {
  MetronomeBPM.Init(Terrarium.controls[KNOB_6], 0.0f, 3.0f, Parameter::LINEAR);
  Metronome.Init(Storage.GetSettings().effects[EFFECT_MISC].values[KNOB_6], Seed.AudioSampleRate());
}

}
}
