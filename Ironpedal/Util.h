// Part of Ironpedal
// https://github.com/snail23/ironpedal

void printlnCentered(char *text) {
  auto length = strlen(text);

  for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
    Ironpedal.display->print(" ");

  Ironpedal.display->println(text);
}

void printFooter(char *effectName) {
  char buf[16];

  printlnCentered(0);
  Ironpedal.display->drawFastHLine(0, Ironpedal.display->getCursorY() - Px437_IBM_VGA_8x148pt7b.yAdvance - Px437_IBM_VGA_8x148pt7b.yAdvance / 3, SSD1351WIDTH, COLOR_LIGHT);

  sprintf(buf, "%c  %c  %c  %c", Ironpedal.currentEffect.switch1 ? '1' : '0', Ironpedal.currentEffect.switch2 ? '1' : '0', Ironpedal.currentEffect.switch3 ? '1' : '0', Ironpedal.currentEffect.switch4 ? '1' : '0');
  printlnCentered(buf);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  printlnCentered(effectName);

  Ironpedal.display->setTextColor(COLOR_DARK);

  if (Ironpedal.currentEffect.id == Effect::EFFECT_MASTER || Ironpedal.currentEffect.id == Effect::EFFECT_MISC) {
    if (Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].locked) {
      Ironpedal.display->setTextColor(COLOR_DARK);
      printlnCentered("LOCKED");
    }
  } else {
    Ironpedal.display->setTextColor(COLOR_DARK);
    sprintf(buf, "%s%s", Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].enabled ? "ON" : "OFF", Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].locked ? " / LOCKED" : "");
    printlnCentered(buf);
  }
}

void printHeader() {
  Ironpedal.display->fillScreen(0);
  Ironpedal.display->setCursor(0, Px437_IBM_VGA_8x148pt7b.yAdvance);
}
