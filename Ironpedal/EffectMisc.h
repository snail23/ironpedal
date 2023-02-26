// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Misc {

Metro Metronome;
Parameter MetronomeBPM;

void Draw() {
  char buf[16];
  printHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("IRONPEDAL");

  Ironpedal.display->setTextColor(COLOR);
  printlnCentered("VER " VERSION);
  printlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered("METRONOME");

  auto bpm = (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6] * 60.0f);
  Ironpedal.display->setTextColor(COLOR);

  if (bpm)
    sprintf(buf, "%u BPM", bpm);

  else
    sprintf(buf, "OFF");

  printlnCentered(buf);
  printFooter("MISC");
}

void Init() {
  MetronomeBPM.Init(Ironpedal.controls[KNOB_6], 0.0f, 3.0f, Parameter::LINEAR);
  Metronome.Init(Ironpedal.storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6], Ironpedal.AudioSampleRate());
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_MISC].locked)
    Ironpedal.storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6] = MetronomeBPM.Process();

  Metronome.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_MISC].values[KNOB_6]);
}

void OnPostAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] += Metronome.Process();
}

}
}
