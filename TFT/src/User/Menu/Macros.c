#include "Macros.h"
#include "includes.h"
#include "menu.h"
#include "Settings.h"

// Helper: force un LABEL à être une string (et pas un index langue)
#define LABEL_STR(s)  (LABEL){ .index = LABEL_NUM, .address = (s) }

void menuMacros(void)
{
  CUSTOM_GCODES customcodes;
  W25Qxx_ReadBuffer((uint8_t *)&customcodes, CUSTOM_GCODE_ADDR, sizeof(CUSTOM_GCODES));

  MENUITEMS macrosPageItems = {
    { .index = LABEL_MACROS },
    .items = {
      { ICON_HOME,              LABEL_STR(customcodes.name[0]) },
      { ICON_BABYSTEP,          LABEL_STR(customcodes.name[1]) },
      { ICON_RGB_WHITE,         LABEL_STR(customcodes.name[2]) },
      { ICON_RGB_OFF,           LABEL_STR(customcodes.name[3]) },
      { ICON_FAN,               LABEL_STR(customcodes.name[4]) },
      { ICON_DISABLE_STEPPERS,  { .index = LABEL_DISABLE_STEPPERS } },
      { ICON_GCODE,             { .index = LABEL_TERMINAL } },
      { ICON_BACK,              { .index = LABEL_BACK } },
    }
  };

  menuDrawPage(&macrosPageItems);

  while (MENU_IS(menuMacros))
  {
    KEY_VALUES key = menuKeyGetValue();

    switch (key)
    {
      case KEY_ICON_0: storeCmd(customcodes.gcode[0]); break;
      case KEY_ICON_1: storeCmd(customcodes.gcode[1]); break;
      case KEY_ICON_2: storeCmd(customcodes.gcode[2]); break;
      case KEY_ICON_3: storeCmd(customcodes.gcode[3]); break;
      case KEY_ICON_4: storeCmd(customcodes.gcode[4]); break;
      case KEY_ICON_5:
        storeCmd("M84\n");
        break;

      case KEY_ICON_6:
        OPEN_MENU(menuTerminal);
        break;


      case KEY_ICON_7:
        CLOSE_MENU();
        break;

      default:
        break;
    }

    loopProcess();
  }
}
