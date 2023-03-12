// Part of Ironpedal
// https://github.com/snail23/ironpedal

void MessageBox(Snailsoft::Ironpedal *ironpedal, const char *text, uint32_t event)
{
    char buf[16];

    SSD1351_set_cursor(0, ironpedal->font.height * 2);
    sprintf(buf, text, event);

    auto length = strlen(text);

    for (auto i = 0u; i < COLUMNS / ironpedal->font.width / 2 - length / 2; ++i)
        SSD1351_write_string(COLOR_DARK, ironpedal->font, " ");

    SSD1351_draw_round_rect(SSD1351_cursor.x - 8, SSD1351_cursor.y - 8, ironpedal->font.width * (length - 1) + 16, ironpedal->font.height + 16 - 1, 4, COLOR_BLACK);
    SSD1351_draw_round_rect(SSD1351_cursor.x - 7, SSD1351_cursor.y - 7, ironpedal->font.width * (length - 1) + 14, ironpedal->font.height + 14 - 1, 4, COLOR_BLACK);
    SSD1351_draw_filled_rect(SSD1351_cursor.x - 6, SSD1351_cursor.y - 6, ironpedal->font.width * (length - 1) + 12, ironpedal->font.height + 12 - 1, COLOR_BLACK);
    SSD1351_draw_round_rect(SSD1351_cursor.x - 6, SSD1351_cursor.y - 6, ironpedal->font.width * (length - 1) + 12, ironpedal->font.height + 12 - 1, 4, COLOR_DARK);
    SSD1351_draw_round_rect(SSD1351_cursor.x - 5, SSD1351_cursor.y - 5, ironpedal->font.width * (length - 1) + 10, ironpedal->font.height + 10 - 1, 4, COLOR_DARK);

    SSD1351_write_string(COLOR_DARK, ironpedal->font, buf);
    SSD1351_update();
}

void PrintlnCentered(Snailsoft::Ironpedal *ironpedal, const char *text, uint16_t color)
{
    auto length = strlen(text);

    for (auto i = 0u; i < COLUMNS / ironpedal->font.width / 2 - length / 2; ++i)
        SSD1351_write_string(color, ironpedal->font, " ");

    SSD1351_write_string(color, ironpedal->font, text);
    SSD1351_write_string(color, ironpedal->font, "\n");
}

void PrintFooter(Snailsoft::Ironpedal *ironpedal, const char *effect_name)
{
    char buf[16];
    PrintlnCentered(ironpedal, "", COLOR);

    auto y = SSD1351_cursor.y - ironpedal->font.height / 2 - 1;
    SSD1351_draw_line(0, y, COLUMNS, y, COLOR_LIGHT);
    sprintf(buf, "%c  %c  %c  %c", ironpedal->current_effect.switch1 ? '1' : '0', ironpedal->current_effect.switch2 ? '1' : '0', ironpedal->current_effect.switch3 ? '1' : '0', ironpedal->current_effect.switch4 ? '1' : '0');
    PrintlnCentered(ironpedal, buf, COLOR);
    PrintlnCentered(ironpedal, effect_name, COLOR_LIGHT);

    if (ironpedal->current_effect.id == Effect::EFFECT_MASTER || ironpedal->current_effect.id == Effect::EFFECT_MISC)
    {
        if (ironpedal->storage->GetSettings().effects[ironpedal->current_effect.id].locked)
            PrintlnCentered(ironpedal, "LOCKED", COLOR_DARK);
    }
    else
    {
        sprintf(buf, "%s%s", ironpedal->storage->GetSettings().effects[ironpedal->current_effect.id].enabled ? "ON" : "OFF", ironpedal->storage->GetSettings().effects[ironpedal->current_effect.id].locked ? " / LOCKED" : "");
        PrintlnCentered(ironpedal, buf, COLOR_DARK);
    }
}
