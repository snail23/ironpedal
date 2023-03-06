// Part of Ironpedal
// https://github.com/snail23/ironpedal

void MessageBox(Ironpedal *pedal, char *text, uint32_t event)
{
    char buf[16];

    pedal->display->setCursor(0, Px437_IBM_VGA_8x148pt7b.yAdvance * 3);
    pedal->display->setTextColor(COLOR_DARK);

    sprintf(buf, text, event);
    auto length = strlen(text);

    for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
        pedal->display->print(" ");

    int16_t x;
    int16_t y;

    uint16_t h;
    uint16_t w;

    pedal->display->getTextBounds(buf, pedal->display->getCursorX(), pedal->display->getCursorY(), &x, &y, &w, &h);
    pedal->display->fillRect(x - 10, y - 10, w + 20, h + 20, 0);

    pedal->display->drawRoundRect(x - 6, y - 6, w + 12, h + 12, 4, COLOR_DARK);
    pedal->display->drawRoundRect(x - 5, y - 5, w + 10, h + 10, 4, COLOR_DARK);

    pedal->display->println(buf);
}

void PrintlnCentered(Ironpedal *pedal, char *text)
{
    auto length = strlen(text);

    for (auto i = 0; i < SSD1351WIDTH / FONT_WIDTH / 2 - length / 2; ++i)
        pedal->display->print(" ");

    pedal->display->println(text);
}

void PrintFooter(Ironpedal *pedal, char *effectName)
{
    char buf[16];

    PrintlnCentered(pedal, 0);
    pedal->display->drawFastHLine(0, pedal->display->getCursorY() - Px437_IBM_VGA_8x148pt7b.yAdvance - Px437_IBM_VGA_8x148pt7b.yAdvance / 3, SSD1351WIDTH, COLOR_LIGHT);

    sprintf(buf, "%c  %c  %c  %c", pedal->currentEffect.switch1 ? '1' : '0', pedal->currentEffect.switch2 ? '1' : '0', pedal->currentEffect.switch3 ? '1' : '0', pedal->currentEffect.switch4 ? '1' : '0');
    PrintlnCentered(pedal, buf);

    pedal->display->setTextColor(COLOR_LIGHT);
    PrintlnCentered(pedal, effectName);

    pedal->display->setTextColor(COLOR_DARK);

    if (pedal->currentEffect.id == Effect::EFFECT_MASTER || pedal->currentEffect.id == Effect::EFFECT_MISC)
    {
        if (pedal->storage->GetSettings().effects[pedal->currentEffect.id].locked)
        {
            pedal->display->setTextColor(COLOR_DARK);
            PrintlnCentered(pedal, "LOCKED");
        }
    }
    else
    {
        pedal->display->setTextColor(COLOR_DARK);
        sprintf(buf, "%s%s", pedal->storage->GetSettings().effects[pedal->currentEffect.id].enabled ? "ON" : "OFF", pedal->storage->GetSettings().effects[pedal->currentEffect.id].locked ? " / LOCKED" : "");
        PrintlnCentered(pedal, buf);
    }
}

void PrintHeader(Ironpedal *pedal)
{
    pedal->display->fillScreen(0);
    pedal->display->setCursor(0, Px437_IBM_VGA_8x148pt7b.yAdvance);
}
