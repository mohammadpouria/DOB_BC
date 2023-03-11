#ifndef __DRIVER_H
#define __DRIVER_H

#include	"main.h"

//#define SetCLK()  (tlc_CLK_GPIO_Port->BSRR = tlc_CLK_Pin)
//#define ClrCLK()  (tlc_CLK_GPIO_Port->BSRR  = (uint32_t)tlc_CLK_Pin << 16U)
//#define SetLE()  (tlc_LE_GPIO_Port->BSRR = tlc_LE_Pin)
//#define ClrLE()  (tlc_LE_GPIO_Port->BSRR  = (uint32_t)tlc_LE_Pin << 16U)
//#define SetOE()  (tlc_OE_GPIO_Port->BSRR = tlc_OE_Pin)
//#define ClrOE()  (tlc_OE_GPIO_Port->BSRR  = (uint32_t)tlc_OE_Pin << 16U)
//#define SetSDI()  (tlc_SDI_GPIO_Port->BSRR = tlc_SDI_Pin)
//#define ClrSDI()  (tlc_SDI_GPIO_Port->BSRR  = (uint32_t)tlc_SDI_Pin << 16U)

void SetCLK(void);
void ClrCLK(void);
void SetLE(void);
void ClrLE(void);
void SetOE(void);
void ClrOE(void);
void SetSDI(void);
void ClrSDI(void);
void Switch2Normal(void);
void Switch2Special(void);
void LED_set(uint8_t data);
void Conf_set(int Current_Gain);
void inline output_en(void) {ClrOE(); }
#endif
