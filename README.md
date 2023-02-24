# Ironpedal
Guitar pedal firmware for Daisy Seed+Terrarrium boards.

# Features
- Retro looking small OLED display
- 16 different effects
- Save/load your effect settings to external flash via QSPI
- Effects can individually be enabled/locked

# Supported Effects
| Effect     | Switch positions | Knob 1    | Knob 2   | Knob 3   | Knob 4   | Knob 5   | Knob 6    |
| ---------- | ---------------- | --------- | -------- | -------- | -------- | -------- | --------- |
| Master     | 0000             | Pre HPF   | Pre LPF  | Pre Gain | Post HPF | Post LPF | Volume    |
| Overdrive  | 0001             |           | Blend    |          |          | Drive    |           |
| Chorus     | 0010             | Delay     |          | Rate     | Feedback |          | Depth     |
| Compressor | 0011             | Threshold |          | Ratio    | Attack   |          | Release   |
| Reverb     | 0100             |           | Feedback |          |          | LPF      |           |
| Misc       | 1111             |           |          |          |          |          | Metronome |

# Effect chain order
| 1                     | 2         | 3         | 4      | 5                        |
| --------------------- | --------- | --------- | ------ | ------------------------ |
| Master: Pre EQ & Gain | Overdrive | Resonator | Reverb | Misc: Metronome          |
| Compressor            |           | Chorus    |        | Master: Post EQ & Volume |

# Usage
| Operation                               | Instructions                                                       |
| --------------------------------------- | ------------------------------------------------------------------ |
| Change the current effect screen        | Move the toggle switches into their correct position               |
| Enabling & disabling the current effect | Single tap the left foot switch (has no effect on Master and Misc) |
| Locking & unlocking the current effect  | Single tap the right foot switch (has no effect on Misc)           |
| Load saved effect settings              | Hold the left foot switch for 3 seconds                            |
| Save effect settings                    | Hold the right foot switch for 3 seconds                           |

# Pictures
Todo.

# Physical build tutorial
Todo.

# Reporting bugs
If you encounter a bug and wish to report it or have feature suggestions and other topics, please [open an issue](https://github.com/snail23/ironpedal/issues).

# License
```
MIT License

Copyright (c) 2023 Snail

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
