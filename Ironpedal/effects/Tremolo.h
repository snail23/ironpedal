// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Tremolo {

daisysp::Tremolo Tremolo;

Parameter Depth;
Parameter Frequency;
Parameter Shape;

void Draw() {
  char buf[16];
  PrintHeader();

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  PrintlnCentered("DEPTH     FREQ");

  Ironpedal.display->setTextColor(COLOR);
  sprintf(buf, "%3u     %3u HZ", (uint32_t)(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1] * 100.0f), (uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
  PrintlnCentered(buf);
  PrintlnCentered(0);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  PrintlnCentered("SHAPE");

  Ironpedal.display->setTextColor(COLOR);

  switch ((uint32_t)Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]) {
    case Oscillator::WAVE_RAMP:
      PrintlnCentered("RAMP WAVE");
      break;

    case Oscillator::WAVE_SAW:
      PrintlnCentered("SAW WAVE");
      break;

    case Oscillator::WAVE_SIN:
      PrintlnCentered("SINE WAVE");
      break;

    case Oscillator::WAVE_SQUARE:
      PrintlnCentered("SQUARE WAVE");
      break;

    case Oscillator::WAVE_TRI:
      PrintlnCentered("TRIANGLE WAVE");
      break;
  }

  PrintFooter("TREMOLO");
}

void Init() {
  Depth.Init(Ironpedal.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Frequency.Init(Ironpedal.controls[KNOB_3], 1.0f, 2880.0f, Parameter::LINEAR);
  Shape.Init(Ironpedal.controls[KNOB_5], Oscillator::WAVE_SIN, Oscillator::WAVE_SQUARE, Parameter::LINEAR);

  Tremolo.Init(Ironpedal.AudioSampleRate());

  Tremolo.SetDepth(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1]);
  Tremolo.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
  Tremolo.SetWaveform(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]);
}

void OnAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Tremolo.Process(in[i]);
}

void OnInput() {
  if (!Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].locked) {
    Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1] = Depth.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3] = Frequency.Process();
    Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5] = Shape.Process();
  }

  Tremolo.SetDepth(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1]);
  Tremolo.SetFreq(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
  Tremolo.SetWaveform(Ironpedal.storage->GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]);
}

}
}
