// Part of Ironpedal
// https://github.com/snail23/ironpedal

void printlnCentered(char *text) {
  auto length = strlen(text);

  for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
    Display.print(" ");

  Display.println(text);
}
