# Ironpedal
Guitar pedal firmware for Daisy Seed+Terrarrium boards.

## Features
- Small hardware accelerated OLED display that can be toggled off to reduce EMI when not in use
- 16 different effects
- Save/load your effect settings to external flash via QSPI
- Effects can be individually enabled/locked
- Record and play back up to 5 minutes of audio via the Looper effect
- 10 different profiles that you can use to save your settings
- Moog Ladder input filter for tone control

## Building from source
Make sure you have the [Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads) and [OpenOCD](https://github.com/openocd-org/openocd/releases) installed, as well as `make`. To build, run the following command in the main directory.

`make clean && make -j && make program`

## Supported effects
| Effect     | Switch position | Knob 1     | Knob 2   | Knob 3    | Knob 4    | Knob 5    | Knob 6     |
| ---------- | --------------- | ---------- | -------- | --------- | --------- | --------- | ---------- |
| Master     | 0000            |            |          | Profile   | LPF       | Resonance | Volume     |
| Overdrive  | 0001            | Blend      |          | Mode      |           | Drive     |            |
| Chorus     | 0010            | Delay      |          | Rate      | Feedback  |           | Depth      |
| Compressor | 0011            | Threshold  |          | Ratio     | Attack    |           | Release    |
| Reverb     | 0100            |            | Feedback |           |           | LPF       |            |
| Resonator  | 0101            | Decay      |          | Frequency | Stiffness |           | Brightness |
| Autowah    | 0110            |            | Blend    |           | Wah       |           |            |
| Tremolo    | 0111            | Depth      |          | Frequency |           | Shape     |            |
| Looper     | 1000            |            | Mode     |           |           | Volume    |            |
| Decimator  | 1001            | Downsample |          | Mode      |           | Crush     |            |
| Phaser     | 1010            | Frequency  |          | Rate      | Feedback  |           | Depth      |
| Delay      | 1011            |            | Delay    |           |           |           |            |
| Flanger    | 1100            | Delay      |          | Rate      | Feedback  |           | Depth      |
| Misc       | 1111            |            |          |           |           |           | Metronome  |

## Effect chain order
| 1                   | 2          | 3         | 4         | 5      | 6                        | 7      |
| ------------------- | ---------- | --------- | --------- | ------ | ------------------------ | ------ |
| Misc: Tuner         | Autowah    | Overdrive | Chorus    | Delay  | Misc: Metronome          | Looper |
| Master: Moog Filter | Decimator  | Resonator | Tremolo   | Reverb | Master: Volume           |        |
|                     | Compressor |           | Phaser    |        |                          |        |
|                     |            |           | Flanger   |        |                          |        |

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
| Effect     | Switch position | Knob 1    | Knob 2   | Knob 3       | Knob 4    | Knob 5        | Knob 6 |
| ---------- | --------------- | --------- | -------- | ------------ | --------- | ------------- | ------ |
| Master     | 0000            |           |          | 1            | 10.1 kHz  | 40            | 150    |
| Overdrive  | 0001            | 45        |          | Hard         |           | 45            |        |
| Chorus     | 0010            | 75        |          | 0.30         | 20        |               | 90     |
| Compressor | 0011            | -12 dB    |          | 2:1          | 0.118     |               | 0.118  |
| Reverb     | 0100            |           | 50       |              |           | 10.1 kHz      |        |
| Resonator  | 0101            | 20        |          | 78 Hz (Eb2)  | 25        |               | 35     |
| Autowah    | 0110            |           | 50       |              | 10        |               |        |
| Tremolo    | 0111            | 50        |          | 622 Hz (Eb5) |           | Triangle Wave |        |
| Looper     | 1000            |           | Playback |              |           | 100           |        |
| Decimator  | 1001            | 35        |          | Soft         |           | 50            |        |
| Phaser     | 1010            | 1.2 kHz   |          | 0.30         | 20        |               | 90     |
| Delay      | 1011            |           | 0 MS     |              |           |               |        |
| Flanger    | 1100            | 75        |          | 0.30         | 20        |               | 90     |
| Misc       | 1111            |           |          |              |           |               | Off    |

You may find this [chart](http://www.simonpaul.com/wp-content/uploads/downloads/2010/04/Notes-To-Frequencies.pdf) useful when setting certain effect frequencies.

## Build tutorial

### Preparation
1. Print off the [schematic](datasheets/Terrarium.pdf) with all scaling options turned off in your printer settings. This will make your life easier later and the last page is a physical template for drilling out the pedal case so this step is required.
2. Order all of the parts listed on page 2 of the [schematic](datasheets/Terrarium.pdf), I used [Tayda Electronics](https://www.taydaelectronics.com) for this.
3. Order a [pedal case](https://www.amazon.com/gp/product/B07VNCDSN7).
4. Order a [600:600 / 1:1 isolation transformer](https://www.amazon.com/gp/product/B073PXK2RX). This gets rid of digital noise on the output at the cost of some minor loss (a few dB when measured) on the low end frequencies. Noise is very noticable without one of these.
5. Order a [Terrarium](https://www.pedalpcb.com/product/pcb351) board from PedalPCB.
6. Order a [Daisy Seed](https://www.electro-smith.com/daisy/daisy) module from Electro-Smith, this is the heart of the pedal.
6. (Optional) If you want a screen, order a [1.5 inch 128x128 SSD1351 OLED display](https://www.amazon.com/1-5inch-Interface-Raspberry-Examples-Provided/dp/B07D9NVJPZ).
7. (Optional) If you want a very easy way of updating the firmware without having to open up the pedal every time to push the Seed BOOT/RESET combo, consider ordering a [STLINK-V3MINI](https://www.electro-smith.com/daisy/stlink-v3mini). Definitely worth it in my opinion to save your sanity.

### Drilling out the pedal case
1. Cut out the pedal case template from the last page of the [schematic](datasheets/Terrarium.pdf) and tape it in place. I used painter's tape for this but masking tape is also a good choice.

![Pedal case template](images/template.jpg)

2. Use a drill press if you have one but if not, hold the pedal case in vise and try and keep your drill as straight as possible. Use a scrap piece of wood on the underside to keep your holes clean.
3. A good tip to figure out the drill bit sizes to use is to use a digital caliper on each part that will be fitted through a hole. Going a little bigger is a good idea for fitting purposes later as things will not line up absolutely perfectly due to manufacturer defects and also just human error.
4. (Optional) File out a rectangle on the front face of the case for the screen. You want to do this by hand and check your work a lot, it's very easy to remove too much material here.

![Pedal case](images/case1.jpg)
![Pedal case](images/case2.jpg)

### (Optional) Paint the pedal case
1. This step is optional but feel free to paint your pedal case, I went with a punk pink color. You will want to use primer as paint falls off aluminum pretty easily without something to stick to.

![Painting the pedal case](images/paint.jpg)

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
