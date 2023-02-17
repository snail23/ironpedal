// Part of Ironpedal
// https://github.com/snail23/ironpedal

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <DaisyDuino.h>
#include <utility/DaisySP/daisysp.h>

#include "Config.h"
#include "Effect.h"
#include "Globals.h"
#include "Res.h"
#include "Util.h"

#include "EffectMaster.h"
#include "EffectOverdrive.h"

bool switchPressed() {
  bool inputReceived = false;

  // Handle foot switch 1

  if (Terrarium.buttons[FOOT_SWITCH_1].RisingEdge()) {
    Profile1 = !Profile1;
    inputReceived = true;

    Terrarium.leds[LED_1].Set(Profile1 ? true : false);
  }

  // Handle foot switch 2

  if (Terrarium.buttons[FOOT_SWITCH_2].RisingEdge()) {
    Profile2 = !Profile2;
    inputReceived = true;

    Terrarium.leds[LED_2].Set(Profile2 ? true : false);
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

  uint32_t switchValues[Terrarium.numSwitches];
  uint32_t val;

  while (true) {
    Terrarium.ProcessAllControls();
    inputReceived = false;

    for (auto i = 0; i < Terrarium.numSwitches; ++i) {
      val = (uint32_t)round(Terrarium.controls[i].Value() * 100.0f);

      if (switchValues[i] != val) {
        switchValues[i] = val;
        inputReceived = true;
      }
    }

    if (inputReceived || switchPressed())
      onInput();

    delay(1);
  }
}

void onAudio(float **in, float **out, size_t size) {
  Effect::Master::onAudio(in[0], out[0], size);

  switch (CurrentEffect.type) {
    case OVERDRIVE:
      Effect::Overdrive::onAudio(in[0], out[0], size);
      break;

    default:
      break;
  }

  Effect::Master::onPostAudio(in[0], out[0], size);
}

void onInput() {
  switch (CurrentEffect.type) {
    case OVERDRIVE:
      Effect::Overdrive::onInput();
      Effect::Overdrive::onDraw();

      break;

    case MASTER:
    default:
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
  Terrarium = Seed.init(DAISY_SEED, AUDIO_SR_96K);

  Terrarium.num_channels = 1;
  Terrarium.numControls = 6;
  Terrarium.numLeds = 2;
  Terrarium.numSwitches = 6;

  Terrarium.buttons[FOOT_SWITCH_1].Init(1000, true, PIN_FOOT_SWITCH_1, INPUT_PULLUP);
  Terrarium.buttons[FOOT_SWITCH_2].Init(1000, true, PIN_FOOT_SWITCH_2, INPUT_PULLUP);

  Terrarium.buttons[SWITCH_1].Init(1000, true, PIN_SWITCH_1, INPUT_PULLUP);
  Terrarium.buttons[SWITCH_2].Init(1000, true, PIN_SWITCH_2, INPUT_PULLUP);
  Terrarium.buttons[SWITCH_3].Init(1000, true, PIN_SWITCH_3, INPUT_PULLUP);
  Terrarium.buttons[SWITCH_4].Init(1000, true, PIN_SWITCH_4, INPUT_PULLUP);

  Terrarium.controls[KNOB_1].Init(PIN_KNOB_1, 1000);
  Terrarium.controls[KNOB_2].Init(PIN_KNOB_2, 1000);
  Terrarium.controls[KNOB_3].Init(PIN_KNOB_3, 1000);
  Terrarium.controls[KNOB_4].Init(PIN_KNOB_4, 1000);
  Terrarium.controls[KNOB_5].Init(PIN_KNOB_5, 1000);
  Terrarium.controls[KNOB_6].Init(PIN_KNOB_6, 1000);

  Terrarium.leds[LED_1].Init(PIN_LED_1, false);
  Terrarium.leds[LED_2].Init(PIN_LED_2, false);

  Effect::Master::onSetup();
  Effect::Overdrive::onSetup();

  Seed.SetAudioBlockSize(1);
  Seed.StartAudio(onAudio);

  // System led off
  seedLed.Set(false);

  // Init display

  Display.begin();
  Display.setFont(&Px437_IBM_VGA_8x148pt7b);

  // Display splash image

  Display.fillScreen(0);
  Display.drawBitmap(0, 0, SplashBitmap, SSD1351WIDTH, SSD1351HEIGHT, COLOR);

  delay(SPLASH_WAIT_MS);

  // Initial update
  onInput();
}
