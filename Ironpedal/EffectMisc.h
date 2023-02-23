// Part of Ironpedal
// https://github.com/snail23/ironpedal

namespace Effect {
namespace Misc {

void onDraw() {
  printHeader();

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("IRONPEDAL");

  Display.setTextColor(COLOR);
  printlnCentered("VER " VERSION);
  printlnCentered(0);

  Display.setTextColor(COLOR_LIGHT);
  printlnCentered("TUNER");

  Display.setTextColor(COLOR);
  printlnCentered("   G+++");

  printFooter("MISC");
}

}
}
