// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Tremolo {

daisysp::Tremolo Tremolo;

Parameter Depth;
Parameter Frequency;
Parameter Shape;

void onAudio(float *in, float *out, size_t size) {
  for (auto i = 0; i < size; ++i)
    out[i] = Tremolo.Process(in[i]);
}

void onDraw() {
  char buf[16];
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("DEPTH     FREQ");

  Display.setTextColor(COLOR);
  sprintf(buf, "%3u     %3u HZ", (uint32_t)(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1] * 100.0f), (uint32_t)Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
  printlnCentered(buf);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("SHAPE");

  Display.setTextColor(COLOR);

  switch ((uint32_t)Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]) {
    case Oscillator::WAVE_RAMP:
      printlnCentered("RAMP WAVE");
      break;

    case Oscillator::WAVE_SAW:
      printlnCentered("SAW WAVE");
      break;

    case Oscillator::WAVE_SIN:
    default:
      printlnCentered("SINE WAVE");
      break;

    case Oscillator::WAVE_SQUARE:
      printlnCentered("SQUARE WAVE");
      break;

    case Oscillator::WAVE_TRI:
      printlnCentered("TRIANGLE WAVE");
      break;
  }

  printFooter("TREMOLO");
}

void onInput() {
  if (!Storage.GetSettings().effects[EFFECT_TREMOLO].locked) {
    Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1] = Depth.Process();
    Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3] = Frequency.Process();
    Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5] = Shape.Process();
  }

  Tremolo.SetDepth(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1]);
  Tremolo.SetFreq(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
  Tremolo.SetWaveform(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]);
}

void onSetup() {
  Depth.Init(Terrarium.controls[KNOB_1], 0.0f, 1.0f, Parameter::LINEAR);
  Frequency.Init(Terrarium.controls[KNOB_3], 1.0f, 2880.0f, Parameter::LINEAR);
  Shape.Init(Terrarium.controls[KNOB_5], Oscillator::WAVE_SIN, Oscillator::WAVE_SQUARE, Parameter::LINEAR);

  Tremolo.Init(Seed.AudioSampleRate());

  Tremolo.SetDepth(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_1]);
  Tremolo.SetFreq(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_3]);
  Tremolo.SetWaveform(Storage.GetSettings().effects[EFFECT_TREMOLO].values[KNOB_5]);
}

}
}
