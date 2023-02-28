// Part of Ironpedal
// https://github.com/snail23/ironpedal

void PrintlnCentered(const char *text) {
  auto length = strlen(text);

  for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
    Ironpedal.display->print(" ");

  Ironpedal.display->println(text);
}

void MessageBox(const char *text, uint32_t event) {
  char buf[16];

  Ironpedal.display->setCursor(0, Px437_IBM_VGA_8x148pt7b.yAdvance * 3);
  Ironpedal.display->setTextColor(COLOR_DARK);

  sprintf(buf, text, event);
  auto length = strlen(text);

  for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
    Ironpedal.display->print(" ");

  int16_t x;
  int16_t y;

  uint16_t h;
  uint16_t w;

  Ironpedal.display->getTextBounds(buf, Ironpedal.display->getCursorX(), Ironpedal.display->getCursorY(), &x, &y, &w, &h);
  Ironpedal.display->fillRect(x - 10, y - 10, w + 20, h + 20, 0);

  Ironpedal.display->drawRoundRect(x - 6, y - 6, w + 12, h + 12, 4, COLOR_DARK);
  Ironpedal.display->drawRoundRect(x - 5, y - 5, w + 10, h + 10, 4, COLOR_DARK);

  Ironpedal.display->println(buf);
}

void PrintFooter(const char *effectName) {
  char buf[16];

  PrintlnCentered(0);
  Ironpedal.display->drawFastHLine(0, Ironpedal.display->getCursorY() - Px437_IBM_VGA_8x148pt7b.yAdvance - Px437_IBM_VGA_8x148pt7b.yAdvance / 3, SSD1351WIDTH, COLOR_LIGHT);

  sprintf(buf, "%c  %c  %c  %c", Ironpedal.currentEffect.switch1 ? '1' : '0', Ironpedal.currentEffect.switch2 ? '1' : '0', Ironpedal.currentEffect.switch3 ? '1' : '0', Ironpedal.currentEffect.switch4 ? '1' : '0');
  PrintlnCentered(buf);

  Ironpedal.display->setTextColor(COLOR_LIGHT);
  PrintlnCentered(effectName);

  Ironpedal.display->setTextColor(COLOR_DARK);

  if (Ironpedal.currentEffect.id == Effect::EFFECT_MASTER || Ironpedal.currentEffect.id == Effect::EFFECT_MISC) {
    if (Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].locked) {
      Ironpedal.display->setTextColor(COLOR_DARK);
      PrintlnCentered("LOCKED");
    }
  } else {
    Ironpedal.display->setTextColor(COLOR_DARK);
    sprintf(buf, "%s%s", Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].enabled ? "ON" : "OFF", Ironpedal.storage->GetSettings().effects[Ironpedal.currentEffect.id].locked ? " / LOCKED" : "");
    PrintlnCentered(buf);
  }
}

void PrintHeader() {
  Ironpedal.display->fillScreen(0);
  Ironpedal.display->setCursor(0, Px437_IBM_VGA_8x148pt7b.yAdvance);
}
