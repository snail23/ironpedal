// Part of Ironpedal
// https://github.com/snail23/ironpedal

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

#include <DaisyDuino.h>
#include <EEPROM.h>
#include <utility/DaisySP/daisysp.h>

#include "Config.h"
#include "Effect.h"
#include "Globals.h"
#include "Res.h"
#include "Util.h"

#include "EffectMaster.h"
#include "EffectOverdrive.h"

bool switchPressed() {
  auto inputReceived = false;

  // Handle foot switch 1

  if (Terrarium.buttons[FOOT_SWITCH_1].RisingEdge()) {
    FootSwitch1TimeHeld = System::GetNow();
  } else if (Terrarium.buttons[FOOT_SWITCH_1].FallingEdge()) {
    auto now = System::GetNow();

    if (now - FootSwitch1TimeHeld > 3000) {
      eepromRead(&Effects, 0, sizeof(Effect::Effect) * Effect::EFFECT_LAST);
      FootSwitch1TimeHeld = now;
    } else
      Effects[CurrentEffect.id].enabled = !Effects[CurrentEffect.id].enabled;

    inputReceived = true;
  }

  // Handle foot switch 2

  if (Terrarium.buttons[FOOT_SWITCH_2].RisingEdge()) {
    FootSwitch2TimeHeld = System::GetNow();
  } else if (Terrarium.buttons[FOOT_SWITCH_2].FallingEdge()) {
    auto now = System::GetNow();

    if (now - FootSwitch2TimeHeld > 3000) {
      eepromWrite(&Effects, 0, sizeof(Effect::Effect) * Effect::EFFECT_LAST);
      FootSwitch2TimeHeld = now;
    } else
      Effects[CurrentEffect.id].locked = !Effects[CurrentEffect.id].locked;

    inputReceived = true;
  }

  // Handle switch 1

  if (Terrarium.buttons[SWITCH_1].RisingEdge()) {
    CurrentEffect.switch1 = true;
    inputReceived = true;
  } else if (Terrarium.buttons[SWITCH_1].FallingEdge()) {
    CurrentEffect.switch1 = false;
    inputReceived = true;
  }

  // Handle switch 2

  if (Terrarium.buttons[SWITCH_2].RisingEdge()) {
    CurrentEffect.switch2 = true;
    inputReceived = true;
  } else if (Terrarium.buttons[SWITCH_2].FallingEdge()) {
    CurrentEffect.switch2 = false;
    inputReceived = true;
  }

  // Handle switch 3

  if (Terrarium.buttons[SWITCH_3].RisingEdge()) {
    CurrentEffect.switch3 = true;
    inputReceived = true;
  } else if (Terrarium.buttons[SWITCH_3].FallingEdge()) {
    CurrentEffect.switch3 = false;
    inputReceived = true;
  }

  // Handle switch 4

  if (Terrarium.buttons[SWITCH_4].RisingEdge()) {
    CurrentEffect.switch4 = true;
    inputReceived = true;
  } else if (Terrarium.buttons[SWITCH_4].FallingEdge()) {
    CurrentEffect.switch4 = false;
    inputReceived = true;
  }

  return inputReceived;
}

void loop() {
  bool inputReceived;

  uint32_t controlValues[Terrarium.numControls];
  uint32_t val;

  while (true) {
    Terrarium.ProcessAllControls();
    inputReceived = false;

    for (auto i = 0; i < Terrarium.numControls; ++i) {
      val = (uint32_t)round(Terrarium.controls[i].Value() * 100.0f);

      if (controlValues[i] != val) {
        controlValues[i] = val;
        inputReceived = true;
      }
    }

    if (inputReceived || switchPressed())
      onInput();

    System::Delay(1);
  }
}

void onAudio(float **in, float **out, size_t size) {
  Effect::Master::onAudio(in[0], out[0], size);

  if (Effects[Effect::EFFECT_OVERDRIVE].enabled)
    Effect::Overdrive::onAudio(in[0], out[0], size);

  Effect::Master::onPostAudio(in[0], out[0], size);
}

void onInput() {
  Terrarium.leds[LED_1].Set(Effects[CurrentEffect.id].enabled || CurrentEffect.id == Effect::EFFECT_MASTER || CurrentEffect.id >= Effect::EFFECT_LAST ? true : false);
  Terrarium.leds[LED_2].Set(Effects[CurrentEffect.id].locked ? true : false);

  switch (CurrentEffect.id) {
    case Effect::EFFECT_OVERDRIVE:
      if (!Effects[Effect::EFFECT_OVERDRIVE].locked)
        Effect::Overdrive::onInput();

      Effect::Overdrive::onDraw();

      break;

    case Effect::EFFECT_MASTER:
    default:
      if (!Effects[Effect::EFFECT_MASTER].locked)
        Effect::Master::onInput();

      Effect::Master::onDraw();

      break;
  }
}

void setup() {
  // Seed led on
  Led seedLed;

  seedLed.Init(LED_BUILTIN, false);
  seedLed.Set(true);

  // Init terrarium
  Terrarium = Seed.init(DAISY_SEED);

  Terrarium.num_channels = 1;
  Terrarium.numControls = 6;
  Terrarium.numLeds = 2;
  Terrarium.numSwitches = 6;

  Terrarium.buttons[FOOT_SWITCH_1].Init(1000.0f, true, PIN_FOOT_SWITCH_1, INPUT_PULLUP);
  Terrarium.buttons[FOOT_SWITCH_2].Init(1000.0f, true, PIN_FOOT_SWITCH_2, INPUT_PULLUP);

  Terrarium.buttons[SWITCH_1].Init(1000.0f, true, PIN_SWITCH_1, INPUT_PULLUP);
  Terrarium.buttons[SWITCH_2].Init(1000.0f, true, PIN_SWITCH_2, INPUT_PULLUP);
  Terrarium.buttons[SWITCH_3].Init(1000.0f, true, PIN_SWITCH_3, INPUT_PULLUP);
  Terrarium.buttons[SWITCH_4].Init(1000.0f, true, PIN_SWITCH_4, INPUT_PULLUP);

  Terrarium.controls[KNOB_1].Init(PIN_KNOB_1, 1000.0f);
  Terrarium.controls[KNOB_2].Init(PIN_KNOB_2, 1000.0f);
  Terrarium.controls[KNOB_3].Init(PIN_KNOB_3, 1000.0f);
  Terrarium.controls[KNOB_4].Init(PIN_KNOB_4, 1000.0f);
  Terrarium.controls[KNOB_5].Init(PIN_KNOB_5, 1000.0f);
  Terrarium.controls[KNOB_6].Init(PIN_KNOB_6, 1000.0f);

  Terrarium.leds[LED_1].Init(PIN_LED_1, false);
  Terrarium.leds[LED_2].Init(PIN_LED_2, false);

  Effect::Master::onSetup();
  Effect::Overdrive::onSetup();

  Seed.SetAudioBlockSize(128);
  Seed.StartAudio(onAudio);

  // System led off
  seedLed.Set(false);

  // Init display

  Display.begin();
  Display.setFont(&Px437_IBM_VGA_8x148pt7b);

  // Display splash image

  Display.fillScreen(0);
  Display.drawBitmap(0, 0, SplashBitmap, SSD1351WIDTH, SSD1351HEIGHT, COLOR);

  System::Delay(1500);

  // Initial update
  onInput();
}
