// Part of Ironpedal
// https://github.com/snail23/ironpedal

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

#include <DaisyDuino.h>

#include "Config.h"
#include "Effect.h"
#include "Res.h"
#include "Terrarium.h"
#include "Ironpedal.h"

#include "Util.h"

#include "Effects/Autowah.h"
#include "Effects/Chorus.h"
#include "Effects/Compressor.h"
#include "Effects/Looper.h"
#include "Effects/Master.h"
#include "Effects/Misc.h"
#include "Effects/Overdrive.h"
#include "Effects/Resonator.h"
#include "Effects/Reverb.h"
#include "Effects/Tremolo.h"

Effect::Autowah *Autowah;
Effect::Chorus *Chorus;
Effect::Compressor *Compressor;
Effect::Looper *Looper;
Effect::Master *Master;
Effect::Misc *Misc;
Effect::Overdrive *Overdrive;
Effect::Resonator *Resonator;
Effect::Reverb *Reverb;
Effect::Tremolo *Tremolo;

Ironpedal *Pedal;

void Draw()
{
    switch (Pedal->currentEffect.id)
    {
    case Effect::EFFECT_AUTOWAH:
        ::Autowah->Draw();

        break;
    case Effect::EFFECT_CHORUS:
        ::Chorus->Draw();

        break;

    case Effect::EFFECT_COMPRESSOR:
        ::Compressor->Draw();

        break;

    case Effect::EFFECT_LOOPER:
        ::Looper->Draw();

        break;

    case Effect::EFFECT_MASTER:
        ::Master->Draw();

        break;

    case Effect::EFFECT_MISC:
        ::Misc->Draw();

        break;

    case Effect::EFFECT_OVERDRIVE:
        ::Overdrive->Draw();

        break;

    case Effect::EFFECT_RESONATOR:
        ::Resonator->Draw();

        break;

    case Effect::EFFECT_REVERB:
        ::Reverb->Draw();

        break;

    case Effect::EFFECT_TREMOLO:
        ::Tremolo->Draw();

        break;
    }
}

void loop()
{
    // The purpose of having everything here is to force as much data on the stack as possible
    // Otherwise, the generated binary is too large

    Ironpedal pedal;
    Pedal = &pedal;

    Effect::Autowah autowah;
    ::Autowah = &autowah;
    ::Autowah->Init(Pedal);

    Effect::Chorus chorus;
    ::Chorus = &chorus;
    ::Chorus->Init(Pedal);

    Effect::Compressor compressor;
    ::Compressor = &compressor;
    ::Compressor->Init(Pedal);

    Effect::Looper looper;
    ::Looper = &looper;
    ::Looper->Init(Pedal);

    Effect::Master master;
    Master = &master;
    Master->Init(Pedal);

    Effect::Misc misc;
    Misc = &misc;
    Misc->Init(Pedal);

    Effect::Overdrive overdrive;
    ::Overdrive = &overdrive;
    ::Overdrive->Init(Pedal);

    Effect::Resonator resonator;
    ::Resonator = &resonator;
    ::Resonator->Init(Pedal);

    Effect::Reverb reverb;
    Reverb = &reverb;
    Reverb->Init(Pedal);

    Effect::Tremolo tremolo;
    ::Tremolo = &tremolo;
    ::Tremolo->Init(Pedal);

    Pedal->Splash();
    Pedal->Loop();
}

void OnAudio(float **in, float **out, size_t size)
{
    Master->OnAudio(in[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_AUTOWAH].enabled)
        ::Autowah->OnAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_COMPRESSOR].enabled)
        ::Compressor->OnAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_OVERDRIVE].enabled)
        ::Overdrive->OnAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_RESONATOR].enabled)
        ::Resonator->OnAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_CHORUS].enabled)
        ::Chorus->OnAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_TREMOLO].enabled)
        ::Tremolo->OnAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_REVERB].enabled)
        Reverb->OnAudio(out[0], out[0], size);

    Misc->OnPostAudio(out[0], out[0], size);
    Master->OnPostAudio(out[0], out[0], size);

    if (Pedal->storage->GetSettings().effects[Effect::EFFECT_LOOPER].enabled)
        ::Looper->OnAudio(out[0], out[0], size);
}

void OnInput()
{
    Pedal->leds[LED_1].Set(Pedal->storage->GetSettings().effects[Pedal->currentEffect.id].enabled || Pedal->currentEffect.id == Effect::EFFECT_MASTER || Pedal->currentEffect.id == Effect::EFFECT_MISC ? true : false);
    Pedal->leds[LED_2].Set(Pedal->storage->GetSettings().effects[Pedal->currentEffect.id].locked);

    switch (Pedal->currentEffect.id)
    {
    case Effect::EFFECT_AUTOWAH:
        ::Autowah->OnInput();

        break;

    case Effect::EFFECT_CHORUS:
        ::Chorus->OnInput();

        break;

    case Effect::EFFECT_COMPRESSOR:
        ::Compressor->OnInput();

        break;

    case Effect::EFFECT_LOOPER:
        ::Looper->OnInput();

        break;

    case Effect::EFFECT_MASTER:
        Master->OnInput();

        break;

    case Effect::EFFECT_MISC:
        Misc->OnInput();

        break;

    case Effect::EFFECT_OVERDRIVE:
        ::Overdrive->OnInput();

        break;

    case Effect::EFFECT_RESONATOR:
        ::Resonator->OnInput();

        break;

    case Effect::EFFECT_REVERB:
        Reverb->OnInput();

        break;

    case Effect::EFFECT_TREMOLO:
        ::Tremolo->OnInput();

        break;
    }

    Draw();
}

void OnInputAll()
{
    Pedal->leds[LED_1].Set(Pedal->storage->GetSettings().effects[Pedal->currentEffect.id].enabled || Pedal->currentEffect.id == Effect::EFFECT_MASTER || Pedal->currentEffect.id == Effect::EFFECT_MISC ? true : false);
    Pedal->leds[LED_2].Set(Pedal->storage->GetSettings().effects[Pedal->currentEffect.id].locked);

    ::Autowah->OnInput();
    ::Chorus->OnInput();
    ::Compressor->OnInput();
    ::Looper->OnInput();
    Master->OnInput();
    Misc->OnInput();
    ::Overdrive->OnInput();
    ::Resonator->OnInput();
    Reverb->OnInput();
    ::Tremolo->OnInput();

    Draw();
}

void setup() {}
