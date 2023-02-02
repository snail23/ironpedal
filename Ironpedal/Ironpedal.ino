// Part of Ironpedal
// https://github.com/snail23/ironpedal

// Definitions

#define DC_PIN D1
#define CS_PIN D3
#define MOSI_PIN D6
#define RST_PIN D30
#define SCLK_PIN D2

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Includes

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <DaisyDuino.h>

// Globals

Adafruit_SSD1351 oled = Adafruit_SSD1351(SSD1351WIDTH, SSD1351HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
DaisyHardware hw;

// Functions

void loop() {
  while (true) {
  }
}

void on_audio(float **in, float **out, size_t size) {
  // Bypass
  memcpy(*in, *out, size * sizeof(float) * hw.num_channels);
}

void setup() {
  // System led on

  Led system_led;

  system_led.Init(LED_BUILTIN, false);
  system_led.Set(true);

  // Init seed

  hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  DAISY.begin(on_audio);

  // Init display
  oled.begin();

  // System led off

  system_led.Set(false);
}