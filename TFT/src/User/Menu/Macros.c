#include "Macros.h"
#include "includes.h"

void menuMacros(void)
{
  // 1 title, ITEM_PER_PAGE items (icon + label)
  MENUITEMS macrosPageItems = {
    // title
    LABEL_MACROS,
    // icon                          label
    {
      {ICON_HOME,                    LABEL_TERMINAL},
      {ICON_MOVE,                    LABEL_TERMINAL},
      {ICON_NOZZLE,                  LABEL_TERMINAL},
      {ICON_STOP,                    LABEL_TERMINAL},
      {ICON_GCODE,                   LABEL_TERMINAL},
      {ICON_NULL,                    LABEL_NULL}, 
      {ICON_DISABLE_STEPPERS,        LABEL_DISABLE_STEPPERS},
      {ICON_BACK,                    LABEL_BACK },
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  if (infoMachineSettings.firmwareType == FW_REPRAPFW)
    macrosPageItems.items[5].label.index = LABEL_MACROS;

  if (infoSettings.status_screen != 1)
  {
    macrosPageItems.items[3].icon = ICON_PRINT;
    macrosPageItems.items[3].label.index = LABEL_PRINT;
  }

  menuDrawPage(&macrosPageItems);
  menuDrawItemStr(KEY_ICON_0, ICON_HOME,  "xx");


  while (MENU_IS(menuMacros))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
        OPEN_MENU(menuHome);
        break;

      case KEY_ICON_1:
        OPEN_MENU(menuMove);
        break;
      
      case KEY_ICON_2:
        OPEN_MENU(menuMpCNC);
        break;

      case KEY_ICON_3:
        // Emergency Stop : Used for emergency stopping, a reset is required to return to operational mode.
        // it may need to wait for a space to open up in the command queue.
        // Enable EMERGENCY_PARSER in Marlin Firmware for an instantaneous M112 command.
        sendEmergencyCmd("M112\n");
        break;

      case KEY_ICON_4:
        OPEN_MENU(menuTerminal);
        break;

      case KEY_ICON_5:
        
        break;

      case KEY_ICON_6:
        storeCmd("M84\n");
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
