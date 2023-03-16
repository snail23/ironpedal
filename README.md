# Ironpedal
Guitar pedal firmware for Daisy Seed+Terrarrium boards.

## Features
- Small hardware accelerated OLED display that can be toggled off to reduce EMI when not in use
- 16 different effects
- Save/load your effect settings to external flash via QSPI
- Effects can be individually enabled/locked
- Record and play back up to 5 minutes of audio via the Looper effect

## Building from source
Make sure you have the [Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads) and [OpenOCD](https://github.com/openocd-org/openocd/releases) installed, as well as `make`. To build, run the following command in the main directory.

`make clean && make -j && make program`

## Supported effects
| Effect     | Switch position | Knob 1    | Knob 2   | Knob 3    | Knob 4    | Knob 5   | Knob 6     |
| ---------- | --------------- | --------- | -------- | --------- | --------- | -------- | ---------- |
| Master     | 0000            | Pre HPF   | Pre LPF  | Pre Gain  | Post HPF  | Post LPF | Volume     |
| Overdrive  | 0001            |           | Blend    |           |           | Drive    |            |
| Chorus     | 0010            | Delay     |          | Rate      | Feedback  |          | Depth      |
| Compressor | 0011            | Threshold |          | Ratio     | Attack    |          | Release    |
| Reverb     | 0100            |           | Feedback |           |           | LPF      |            |
| Resonator  | 0101            | Decay     |          | Frequency | Stiffness |          | Brightness |
| Autowah    | 0110            |           | Blend    |           | Wah       |          |            |
| Tremolo    | 0111            | Depth     |          | Frequency |           | Shape    |            |
| Looper     | 1000            |           | Mode     |           |           | Volume   |            |
| Misc       | 1111            |           |          |           |           |          | Metronome  |

## Effect chain order
| 1                     | 2          | 3         | 4         | 5      | 6                        | 7      |
| --------------------- | ---------- | --------- | --------- | ------ | ------------------------ | ------ |
| Master: Pre EQ & Gain | Autowah    | Overdrive | Resonator | Reverb | Misc: Metronome          | Looper |
|                       | Compressor |           | Chorus    |        | Master: Post EQ & Volume |        |
|                       |            |           | Tremolo   |        |                          |        |

## Usage
| Operation                               | Instructions                                                       |
| --------------------------------------- | ------------------------------------------------------------------ |
| Turn the OLED display on/off            | Single tap both foot switches                                      |
| Change the current effect screen        | Move the toggle switches into their correct position               |
| Enabling & disabling the current effect | Single tap the left foot switch (has no effect on Master and Misc) |
| Locking & unlocking the current effect  | Single tap the right foot switch                                   |
| Load saved effect settings              | Hold the left foot switch for 3 seconds                            |
| Save effect settings                    | Hold the right foot switch for 3 seconds                           |

## My settings, feel free to use these as a starting point
| Effect     | Switch position | Knob 1    | Knob 2   | Knob 3       | Knob 4    | Knob 5        | Knob 6     |
| ---------- | --------------- | --------- | -------- | ------------ | --------- | ------------- | ---------- |
| Master     | 0000            | 60 Hz     | 10.1 kHz | 0            | 60 Hz     | 10.1 kHz      | 100        |
| Overdrive  | 0001            |           | 45       |              |           | 45            |            |
| Chorus     | 0010            | 75        |          | 0.30         | 20        |               | 90         |
| Compressor | 0011            | -12 dB    |          | 2:1          | 0.118     |               | 0.118      |
| Reverb     | 0100            |           | 50       |              |           | 10.1 kHz      |            |
| Resonator  | 0101            | 20        |          | 78 Hz (Eb2)  | 25        |               | 35         |
| Autowah    | 0110            |           | 50       |              | 10        |               |            |
| Tremolo    | 0111            | 50        |          | 622 Hz (Eb5) |           | Triangle Wave |            |
| Looper     | 1000            |           | Playback |              |           | 100           |            |
| Misc       | 1111            |           |          |              |           |               | Off        |

You may find this [chart](http://www.simonpaul.com/wp-content/uploads/downloads/2010/04/Notes-To-Frequencies.pdf) useful when setting certain effect frequencies.

## Pictures
Todo.

## Physical build tutorial
Todo.

## Reporting bugs
If you encounter a bug and wish to report it or have feature suggestions and other topics, please [open an issue](https://github.com/snail23/ironpedal/issues).

## License
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
