# Ironpedal
Guitar pedal firmware for Daisy Seed+Terrarrium boards.

# Features
- 16 different effects
- Save/load your effect settings to external flash via QSPI
- Effects can individually be enabled/locked

# Supported Effects
| Effect     | Switch positions |
| ---------- | ---------------- |
| Master     | 0000             |
| Overdrive  | 0001             |
| Chorus     | 0010             |
| Compressor | 0011             |

# Effect order
| Effects              | Effects   | Effects | Effects        |
| -------------------- | --------- | ------- | -------------- |
| Master: EQ & Pregain | Overdrive | Chorus  | Master: Volume |
| Compressor           |           |         |                |

# Usage
| Operation                               | Instructions                                         |
| --------------------------------------- | ---------------------------------------------------- |
| Change the current effect screen        | Move the toggle switches into their correct position |
| Enabling & disabling the current effect | Single tap the left foot switch                      |
| Locking & unlocking the current effect  | Single tap the right foot switch                     |
| Load saved effect settings              | Hold the left foot switch for 3 seconds              |
| Save effect settings                    | Hold the right foot switch for 3 seconds             |

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
