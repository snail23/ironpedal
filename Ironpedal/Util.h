// Part of Ironpedal
// https://github.com/snail23/ironpedal

void printlnCentered(char *text) {
  auto length = strlen(text);

  for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
    Display.print(" ");

  Display.println(text);
}

void printFooter(char *effectName) {
  char buf[16];
  printlnCentered(0);

  Display.drawFastHLine(0, Display.getCursorY() - Px437_IBM_VGA_8x148pt7b.yAdvance - Px437_IBM_VGA_8x148pt7b.yAdvance / 3, SSD1351WIDTH, COLOR_LIGHT);
  sprintf(buf, "%c  %c  %c  %c", CurrentEffect.switch1 ? '1' : '0', CurrentEffect.switch2 ? '1' : '0', CurrentEffect.switch3 ? '1' : '0', CurrentEffect.switch4 ? '1' : '0');
  printlnCentered(buf);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered(effectName);

  Display.setTextColor(COLOR_DARK);

  if (CurrentEffect.id == Effect::EFFECT_MASTER) {
    if (Effects[CurrentEffect.id].locked) {
      Display.setTextColor(COLOR_DARK);
      printlnCentered("LOCKED");
    }
  } else {
    Display.setTextColor(COLOR_DARK);
    sprintf(buf, "%s%s", Effects[CurrentEffect.id].enabled ? "ON" : "OFF", Effects[CurrentEffect.id].locked ? " / LOCKED" : "");
    printlnCentered(buf);
  }
}
