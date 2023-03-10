// Part of Ironpedal
// https://github.com/snail23/ironpedal

void MessageBox(Snailsoft::Ironpedal *ironpedal, const char *text, uint32_t event)
{
    char buf[16];
    sprintf(buf, text, event);

    size_t length = strlen(buf);
    SSD1351_set_cursor(64 - (length * ironpedal->font.width) / 2, ironpedal->font.height * 2);

    SSD1351_draw_round_rect(SSD1351_cursor.x - 8, SSD1351_cursor.y - 8, ironpedal->font.width * length + 16, ironpedal->font.height + 16 - 1, 4, COLOR_BLACK);
    SSD1351_draw_round_rect(SSD1351_cursor.x - 7, SSD1351_cursor.y - 7, ironpedal->font.width * length + 14, ironpedal->font.height + 14 - 1, 4, COLOR_BLACK);
    SSD1351_draw_filled_rect(SSD1351_cursor.x - 6, SSD1351_cursor.y - 6, ironpedal->font.width * length + 12, ironpedal->font.height + 12 - 1, COLOR_BLACK);
    SSD1351_draw_round_rect(SSD1351_cursor.x - 6, SSD1351_cursor.y - 6, ironpedal->font.width * length + 12, ironpedal->font.height + 12 - 1, 4, COLOR_DARK);
    SSD1351_draw_round_rect(SSD1351_cursor.x - 5, SSD1351_cursor.y - 5, ironpedal->font.width * length + 10, ironpedal->font.height + 10 - 1, 4, COLOR_DARK);

    SSD1351_write_string(COLOR_DARK, ironpedal->font, buf);
    SSD1351_update();
}

void PrintFooter(Snailsoft::Ironpedal *ironpedal, const char *effect_name)
{
    char buf[16];
    SSD1351_write_string(COLOR, ironpedal->font, "\n");

    uint8_t y = SSD1351_cursor.y - ironpedal->font.height / 2 - 1;
    SSD1351_draw_line(0, y, COLUMNS, y, COLOR_LIGHT);
    sprintf(buf, "%c  %c  %c  %c\n", ironpedal->current_effect.switch1 ? '1' : '0', ironpedal->current_effect.switch2 ? '1' : '0', ironpedal->current_effect.switch3 ? '1' : '0', ironpedal->current_effect.switch4 ? '1' : '0');
    SSD1351_write_string(COLOR, ironpedal->font, buf, ALIGN_CENTER);
    SSD1351_write_string(COLOR_LIGHT, ironpedal->font, effect_name, ALIGN_CENTER);

    if (ironpedal->current_effect.id == Effect::EFFECT_MASTER || ironpedal->current_effect.id == Effect::EFFECT_MISC)
    {
        if (ironpedal->storage->GetSettings().effects[ironpedal->current_effect.id].locked)
            SSD1351_write_string(COLOR_DARK, ironpedal->font, "LOCKED\n", ALIGN_CENTER);
    }
    else
    {
        sprintf(buf, "%s%s", ironpedal->storage->GetSettings().effects[ironpedal->current_effect.id].enabled ? "ON" : "OFF", ironpedal->storage->GetSettings().effects[ironpedal->current_effect.id].locked ? " / LOCKED\n" : "\n");
        SSD1351_write_string(COLOR_DARK, ironpedal->font, buf, ALIGN_CENTER);
    }
}
