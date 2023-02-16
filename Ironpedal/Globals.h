// Part of Ironpedal
// https://github.com/snail23/ironpedal

Adafruit_SSD1351 Display = Adafruit_SSD1351(SSD1351WIDTH, SSD1351HEIGHT, PIN_CS, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCK, PIN_RST);  // TODO: Make this hardware accelerated
AudioClass Seed;

bool Profile1 = false;
bool Profile2 = false;

DaisyHardware Terrarium;
