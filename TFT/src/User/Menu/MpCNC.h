#ifndef _MPCNC_H_
#define _MPCNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"  // for TFT70_V3_0

#ifdef TFT70_V3_0
  #include <stdint.h>

  void setPWMItemIndex(uint8_t index);
#endif

void menuMpCNC(void);

#ifdef __cplusplus
}
#endif

#endif
