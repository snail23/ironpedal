// Part of Ironpedal
// https://github.com/snail23/ironpedal

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

#include <DaisyDuino.h>

#include "Config.h"
#include "Effect.h"
#include "Res.h"
#include "Terrarium.h"
#include "Ironpedal.h"

#include "Globals.h"
#include "Util.h"

#include "Effects/Autowah.h"
#include "Effects/Chorus.h"
#include "Effects/Compressor.h"
#include "Effects/Master.h"
#include "Effects/Misc.h"
#include "Effects/Overdrive.h"
#include "Effects/Resonator.h"
#include "Effects/Reverb.h"
#include "Effects/Tremolo.h"

void Draw() {
  switch (Ironpedal.currentEffect.id) {
    case Effect::EFFECT_AUTOWAH:
      Effect::Autowah::Draw();

      break;
    case Effect::EFFECT_CHORUS:
      Effect::Chorus::Draw();

      break;

    case Effect::EFFECT_COMPRESSOR:
      Effect::Compressor::Draw();

      break;

    case Effect::EFFECT_MASTER:
      Effect::Master::Draw();

      break;

    case Effect::EFFECT_MISC:
      Effect::Misc::Draw();

      break;

    case Effect::EFFECT_OVERDRIVE:
      Effect::Overdrive::Draw();

      break;

    case Effect::EFFECT_RESONATOR:
      Effect::Resonator::Draw();

      break;

    case Effect::EFFECT_REVERB:
      Effect::Reverb::Draw();

      break;

    case Effect::EFFECT_TREMOLO:
      Effect::Tremolo::Draw();

      break;
  }
}

void loop() {
  Ironpedal.Loop();
}

void OnAudio(float **in, float **out, size_t size) {
  Effect::Master::OnAudio(in[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_AUTOWAH].enabled)
    Effect::Autowah::OnAudio(out[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_COMPRESSOR].enabled)
    Effect::Compressor::OnAudio(out[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_OVERDRIVE].enabled)
    Effect::Overdrive::OnAudio(out[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_RESONATOR].enabled)
    Effect::Resonator::OnAudio(out[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_CHORUS].enabled)
    Effect::Chorus::OnAudio(out[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_TREMOLO].enabled)
    Effect::Tremolo::OnAudio(out[0], out[0], size);

  if (Ironpedal.storage->GetSettings().effects[Effect::EFFECT_REVERB].enabled)
    Effect::Reverb::OnAudio(out[0], out[0], size);

  Effect::Misc::OnPostAudio(out[0], out[0], size);
  Effect::Master::OnPostAudio(out[0], out[0], size);
}

void OnInput() {
  Ironpedal.leds[LED_1].Set(Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].enabled || Ironpedal.currentEffect.id == Effect::EFFECT_MASTER || Ironpedal.currentEffect.id == Effect::EFFECT_MISC ? true : false);
  Ironpedal.leds[LED_2].Set(Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].locked);

  switch (Ironpedal.currentEffect.id) {
    case Effect::EFFECT_AUTOWAH:
      Effect::Autowah::OnInput();

      break;

    case Effect::EFFECT_CHORUS:
      Effect::Chorus::OnInput();

      break;

    case Effect::EFFECT_COMPRESSOR:
      Effect::Compressor::OnInput();

      break;

    case Effect::EFFECT_MASTER:
      Effect::Master::OnInput();

      break;

    case Effect::EFFECT_MISC:
      Effect::Misc::OnInput();

      break;

    case Effect::EFFECT_OVERDRIVE:
      Effect::Overdrive::OnInput();

      break;

    case Effect::EFFECT_RESONATOR:
      Effect::Resonator::OnInput();

      break;

    case Effect::EFFECT_REVERB:
      Effect::Reverb::OnInput();

      break;

    case Effect::EFFECT_TREMOLO:
      Effect::Tremolo::OnInput();

      break;
  }

  Draw();
}

void OnInputAll() {
  Ironpedal.leds[LED_1].Set(Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].enabled || Ironpedal.currentEffect.id == Effect::EFFECT_MASTER || Ironpedal.currentEffect.id == Effect::EFFECT_MISC ? true : false);
  Ironpedal.leds[LED_2].Set(Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].locked);

  Effect::Autowah::OnInput();
  Effect::Chorus::OnInput();
  Effect::Compressor::OnInput();
  Effect::Master::OnInput();
  Effect::Misc::OnInput();
  Effect::Overdrive::OnInput();
  Effect::Resonator::OnInput();
  Effect::Reverb::OnInput();
  Effect::Tremolo::OnInput();

  Draw();
}

void OnInit() {
  Effect::Autowah::Init();
  Effect::Chorus::Init();
  Effect::Compressor::Init();
  Effect::Master::Init();
  Effect::Misc::Init();
  Effect::Overdrive::Init();
  Effect::Resonator::Init();
  Effect::Reverb::Init();
  Effect::Tremolo::Init();
}

void setup() {}
