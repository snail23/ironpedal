// Part of Ironpedal
// https://github.com/snail23/ironpedal

#include <daisy_seed.h>
#include <daisysp.h>

#include <ssd1351.h>

#include "config.h"
#include "effect.h"
#include "terrarium.h"
#include "ironpedal.h"

#include "effects/autowah.h"
#include "effects/chorus.h"
#include "effects/compressor.h"
#include "effects/decimator.h"
#include "effects/looper.h"
#include "effects/master.h"
#include "effects/misc.h"
#include "effects/overdrive.h"
#include "effects/resonator.h"
#include "effects/reverb.h"
#include "effects/tremolo.h"

Effect::Autowah *Autowah;
Effect::Chorus *Chorus;
Effect::Compressor *Compressor;
Effect::Decimator *Decimator;
Effect::Looper *Looper;
Effect::Master *Master;
Effect::Misc *Misc;
Effect::Overdrive *Overdrive;
Effect::Resonator *Resonator;
Effect::Reverb *Reverb;
Effect::Tremolo *Tremolo;

Snailsoft::Ironpedal *Ironpedal;

bool Update()
{
    if (Ironpedal->current_effect.id == Effect::EFFECT_MISC && Ironpedal->display_enabled)
        return Misc->Update();

    return false;
}

int main()
{
    Ironpedal = new Snailsoft::Ironpedal;

    Autowah = new Effect::Autowah(Ironpedal);
    Chorus = new Effect::Chorus(Ironpedal);
    Compressor = new Effect::Compressor(Ironpedal);
    Decimator = new Effect::Decimator(Ironpedal);
    Looper = new Effect::Looper(Ironpedal);
    Master = new Effect::Master(Ironpedal);
    Misc = new Effect::Misc(Ironpedal);
    Overdrive = new Effect::Overdrive(Ironpedal);
    Resonator = new Effect::Resonator(Ironpedal);
    Reverb = new Effect::Reverb(Ironpedal);
    Tremolo = new Effect::Tremolo(Ironpedal);

    Ironpedal->Run();
}

void Draw()
{
    SSD1351_fill(COLOR_BLACK);
    SSD1351_set_cursor(1, 0);

    switch (Ironpedal->current_effect.id)
    {
    case Effect::EFFECT_AUTOWAH:
        Autowah->Draw();

        break;

    case Effect::EFFECT_CHORUS:
        Chorus->Draw();

        break;

    case Effect::EFFECT_COMPRESSOR:
        Compressor->Draw();

        break;

    case Effect::EFFECT_DECIMATOR:
        Decimator->Draw();

        break;

    case Effect::EFFECT_LOOPER:
        Looper->Draw();

        break;

    case Effect::EFFECT_MASTER:
        Master->Draw();

        break;

    case Effect::EFFECT_MISC:
        Misc->Draw();

        break;

    case Effect::EFFECT_OVERDRIVE:
        Overdrive->Draw();

        break;

    case Effect::EFFECT_RESONATOR:
        Resonator->Draw();

        break;

    case Effect::EFFECT_REVERB:
        Reverb->Draw();

        break;

    case Effect::EFFECT_TREMOLO:
        Tremolo->Draw();

        break;

    default:
        break;
    }

    SSD1351_update();
}

void OnAudio(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size)
{
    if (Ironpedal->current_effect.id == Effect::EFFECT_MISC && Ironpedal->display_enabled)
        Misc->OnAudio((float *)in[0], out[0], size);

    Master->OnAudio((float *)in[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_AUTOWAH).enabled)
        Autowah->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_DECIMATOR).enabled)
        Decimator->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_COMPRESSOR).enabled)
        Compressor->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_OVERDRIVE).enabled)
        Overdrive->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_RESONATOR).enabled)
        Resonator->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_CHORUS).enabled)
        Chorus->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_TREMOLO).enabled)
        Tremolo->OnAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_REVERB).enabled)
        Reverb->OnAudio(out[0], out[0], size);

    Misc->OnPostAudio(out[0], out[0], size);
    Master->OnPostAudio(out[0], out[0], size);

    if (Ironpedal->GetEffect(Effect::EFFECT_LOOPER).enabled)
        Looper->OnAudio(out[0], out[0], size);
}

void OnInput()
{
    Ironpedal->leds[PedalPCB::LED_1].Set(Ironpedal->GetEffect(Ironpedal->current_effect.id).enabled || Ironpedal->current_effect.id == Effect::EFFECT_MASTER || Ironpedal->current_effect.id == Effect::EFFECT_MISC);
    Ironpedal->leds[PedalPCB::LED_2].Set(Ironpedal->GetEffect(Ironpedal->current_effect.id).locked);

    switch (Ironpedal->current_effect.id)
    {
    case Effect::EFFECT_AUTOWAH:
        Autowah->OnInput();

        break;

    case Effect::EFFECT_CHORUS:
        Chorus->OnInput();

        break;

    case Effect::EFFECT_COMPRESSOR:
        Compressor->OnInput();

        break;

    case Effect::EFFECT_DECIMATOR:
        Decimator->OnInput();

        break;

    case Effect::EFFECT_LOOPER:
        Looper->OnInput();

        break;

    case Effect::EFFECT_MASTER:
        Master->OnInput();

        break;

    case Effect::EFFECT_MISC:
        Misc->OnInput();

        break;

    case Effect::EFFECT_OVERDRIVE:
        Overdrive->OnInput();

        break;

    case Effect::EFFECT_RESONATOR:
        Resonator->OnInput();

        break;

    case Effect::EFFECT_REVERB:
        Reverb->OnInput();

        break;

    case Effect::EFFECT_TREMOLO:
        Tremolo->OnInput();

        break;

    default:
        break;
    }

    Draw();
}

void OnInputAll()
{
    Ironpedal->leds[PedalPCB::LED_1].Set(Ironpedal->GetEffect(Ironpedal->current_effect.id).enabled || Ironpedal->current_effect.id == Effect::EFFECT_MASTER || Ironpedal->current_effect.id == Effect::EFFECT_MISC);
    Ironpedal->leds[PedalPCB::LED_2].Set(Ironpedal->GetEffect(Ironpedal->current_effect.id).locked);

    Misc->OnInput(true);
    Autowah->OnInput();
    Chorus->OnInput();
    Compressor->OnInput();
    Decimator->OnInput();
    Looper->OnInput();
    Master->OnInput();
    Overdrive->OnInput();
    Resonator->OnInput();
    Reverb->OnInput();
    Tremolo->OnInput();

    Draw();
}

void SSD1351_ClearCSPin()
{
    Ironpedal->spi_cs.Write(false);
}

void SSD1351_ClearDCPin()
{
    Ironpedal->spi_dc.Write(false);
}

void SSD1351_ClearResetPin()
{
    Ironpedal->spi_rst.Write(false);
}

void SSD1351_DelayMs(uint32_t ms)
{
    daisy::System::Delay(ms);
}

void SSD1351_SendBuffer(uint8_t *buffer, uint32_t size)
{
    Ironpedal->spi.BlockingTransmit(buffer, size);
}

void SSD1351_SendByte(uint8_t val)
{
    Ironpedal->spi.BlockingTransmit(&val, sizeof(val));
}

void SSD1351_SetCSPin()
{
    Ironpedal->spi_cs.Write(true);
}

void SSD1351_SetDCPin()
{
    Ironpedal->spi_dc.Write(true);
}

void SSD1351_SetResetPin()
{
    Ironpedal->spi_rst.Write(true);
}

// Shuts the compiler up

extern "C"
{
    int _close() { return -1; }
    int _fstat_r() { return -1; }
    int _getpid() { return -1; }
    int _isatty_r() { return -1; }
    int _kill() { return -1; }
    int _lseek() { return -1; }
    int _read() { return -1; }
    int _write() { return -1; }
}
