// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect
{
    enum Id
    {
        EFFECT_MASTER,     // 0000
        EFFECT_OVERDRIVE,  // 0001
        EFFECT_CHORUS,     // 0010
        EFFECT_COMPRESSOR, // 0011
        EFFECT_REVERB,     // 0100
        EFFECT_RESONATOR,  // 0101
        EFFECT_AUTOWAH,    // 0110
        EFFECT_TREMOLO,    // 0111
        EFFECT_LOOPER,     // 1000
        EFFECT_DECIMATOR,  // 1001
        EFFECT_PHASER,     // 1010
        EFFECT_DELAY,      // 1011
        EFFECT_FLANGER,    // 1100
        EFFECT_PITCH,      // 1101
        EFFECT_NYI,        // 1110
        EFFECT_MISC,       // 1111
        EFFECT_LAST
    };

    union Type
    {
        Id id;

        struct
        {
            bool switch4 : 1;
            bool switch3 : 1;
            bool switch2 : 1;
            bool switch1 : 1;
        };
    };

    struct Effect
    {
        bool enabled;
        bool locked;

        float values[6]; // KNOB_LAST
    };
}
