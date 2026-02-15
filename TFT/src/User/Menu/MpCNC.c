#include "MpCNC.h"
#include "includes.h"

#define DUAL_VAL_FORMAT "%4d/%-4d"

static uint16_t pwm_value = 0;   // valeur courante envoyée (0..max)
static uint16_t pwm_max   = 100; // max selon mode (% ou PWM)
static uint16_t delta     = 1;   // pas incrément
static uint8_t  percentSteps_index = 0;

// ----------- UI redraw -----------
static void mpCNCReDraw(bool skipHeader)
{
  char tempstr[20];

  if (!skipHeader)
  {
    // displayExhibitHeader(LABEL_CNC_LASER, (infoSettings.cnc_percentage == 1) ? " % " : "PWM"); Marche pas, je ne sais pas pourquoi !!!!
    displayExhibitHeader(LABEL_MPCNC, (infoSettings.cnc_percentage == 1) ? " % " : "PWM");
  }

  snprintf(tempstr, sizeof(tempstr), DUAL_VAL_FORMAT, (int)pwm_value, (int)pwm_max);
  displayExhibitValue(tempstr);
}

// ----------- G-code send -----------
static void sendLaserOrSpindle(void)
{
  // ON avec valeur
  storeCmd(CNC_LASER_CMD_ON, pwm_value);
}

static void stopLaserOrSpindle(void)
{
  storeCmd(CNC_LASER_CMD_OFF);
}

// ----------- Menu -----------
void menuMpCNC(void)
{
  // Si on n'est pas en mode %, on utilise la valeur max PWM configurée
  if (infoSettings.cnc_percentage != 1)
    pwm_max = infoSettings.cnc_laser_max;
  else
    pwm_max = 100;

  // Sécurité : éviter delta=0 si max petit
  delta = 1;

  MENUITEMS mpCNCItems = {
    LABEL_MPCNC,  // Si tu as ajouté ce label. Sinon mets LABEL_CNC_LASER_SPINDLE ou un autre existant.
    {
      { ICON_DEC,         LABEL_DEC },
      { ICON_NULL,        LABEL_NULL },
      { ICON_NULL,        LABEL_NULL },
      { ICON_INC,         LABEL_INC },
      { ICON_SHUT_DOWN,   LABEL_ON },
      { ICON_E_1_PERCENT, LABEL_1_PERCENT },
      { ICON_STOP,        LABEL_STOP },
      { ICON_BACK,        LABEL_BACK },
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&mpCNCItems);
  mpCNCReDraw(false);

  while (MENU_IS(menuMpCNC))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
      case KEY_DECREASE:
        if (pwm_value > delta) pwm_value -= delta;
        else pwm_value = 0;
        mpCNCReDraw(false);
        sendLaserOrSpindle();
        break;

      case KEY_ICON_1:
      case KEY_ICON_2:
      case KEY_INFOBOX:
      {
        int16_t val = editIntValue(0, pwm_max, 0, pwm_value);
        if (val != (int16_t)pwm_value)
          pwm_value = (uint16_t)val;

        // Redessiner proprement (comme ton ancien code)
        menuDrawPage(&mpCNCItems);
        mpCNCReDraw(true);
        sendLaserOrSpindle();
        break;
      }

      case KEY_ICON_3:
      case KEY_INCREASE:
        if (pwm_value + delta < pwm_max) pwm_value += delta;
        else pwm_value = pwm_max;
        mpCNCReDraw(false);
        sendLaserOrSpindle();
        break;

      case KEY_ICON_4:
        // Bouton ON : dans ton ancien code tu mettais max/100 (≈ 1%)
        // Je garde pareil. Si tu veux "plein pot", mets pwm_value = pwm_max;
        pwm_value = (pwm_max / 100);
        mpCNCReDraw(false);
        sendLaserOrSpindle();
        break;

      case KEY_ICON_5:
        // Changement du pas (1%, 5%, 10% etc.)
        percentSteps_index = (percentSteps_index + 1) % ITEM_PERCENT_STEPS_NUM;

        // Si tu veux afficher l'icône du pas, il faut que itemPercent[] existe encore dans ta version.
        // Si ça ne compile pas, commente les 2 lignes suivantes.
        mpCNCItems.items[key_num] = itemPercent[percentSteps_index];
        menuDrawItem(&mpCNCItems.items[key_num], key_num);

        // Calcul du delta en valeur absolue
        delta = (uint16_t)((pwm_max * percentSteps[percentSteps_index]) / 100);
        if (delta < 1) delta = 1;
        break;

      case KEY_ICON_6:
        // STOP
        stopLaserOrSpindle();
        pwm_value = 0;
        mpCNCReDraw(false);
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
