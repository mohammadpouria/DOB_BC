#include "Driver.h"

const uint8_t conf_code_LUT[75] = {0x80
, 0xC0
, 0xA0
, 0x10
, 0x50
, 0xB0
, 0xF0
, 0x48
, 0x28
, 0xE8
, 0x58
, 0x38
, 0xF8
, 0x84
, 0x24
, 0x64
, 0x94
, 0x34
, 0x74
, 0x8C
, 0xCC
, 0x6C
, 0x1C
, 0xDC
, 0xBC
, 0x02
, 0x82
, 0xC2
, 0x22
, 0xA2
, 0x62
, 0x12
, 0x92
, 0x52
, 0x32
, 0xB2
, 0x72
, 0xF2
, 0x8A
, 0x4A
, 0xCA
, 0x2A
, 0x6A
, 0xEA
, 0x1A
, 0x5A
, 0xDA
, 0x3A
, 0xBA
, 0xFA
, 0x06
, 0x86
, 0xC6
, 0x26
, 0xA6
, 0x66
, 0x16
, 0x96
, 0x56
, 0x36
, 0xB6
, 0x76
, 0xF6
, 0x8E
, 0x4E
, 0xCE
, 0x2E
, 0x6E
, 0xEE
, 0x1E
, 0x5E
, 0xDE
, 0x3E
, 0xBE
, 0xFE};

const int LUT_LED[75] = {57,	57,	58,	59,	60,	61,	61,	62,	63,	64,	64,	65,	66,	67,	67,	68,	69,	69,	70,	71,	71,	72,	72,	73,	74,	74,	75,	75,	76,	77,	77,	78,	78,	79,	79,	80,	81,	81,	82,	82,	83,	83,	84,	84,	85,	85,	86,	86,	87,	87,	88,	88,	89,	89,	90,	90,	91,	91,	91,	92,	92,	93,	93,	94,	94,	95,	95,	96,	96,	96,	97,	97,	98,	98,	99};

void inline SetCLK(void){
	__ASM("NOP");
	tlc_CLK_GPIO_Port->BSRR = tlc_CLK_Pin;
	__ASM("NOP");
}
void inline ClrCLK(void){
	__ASM("NOP");
	tlc_CLK_GPIO_Port->BSRR = tlc_CLK_Pin << 16U;
	__ASM("NOP");
}

void inline SetSDI(void){
	__ASM("NOP");
	tlc_SDI_GPIO_Port->BSRR = tlc_SDI_Pin;
	__ASM("NOP");
}
void inline ClrSDI(void){
	__ASM("NOP");
	tlc_SDI_GPIO_Port->BSRR = tlc_SDI_Pin << 16U;
	__ASM("NOP");
}

void inline SetLE(void){
	__ASM("NOP");
	tlc_LE_GPIO_Port->BSRR = tlc_LE_Pin;
	__ASM("NOP");
}
void inline ClrLE(void){
	__ASM("NOP");
	tlc_LE_GPIO_Port->BSRR = tlc_LE_Pin << 16U;
	__ASM("NOP");
}

void inline SetOE(void){
	__ASM("NOP");
	tlc_OE_GPIO_Port->BSRR = tlc_OE_Pin;
	__ASM("NOP");
}
void inline ClrOE(void){
	__ASM("NOP");
	tlc_OE_GPIO_Port->BSRR = tlc_OE_Pin << 16U;
	__ASM("NOP");
}

void Switch2Special(){
	ClrCLK();
	ClrLE();
	SetOE();
	SetCLK();	ClrCLK();
	ClrOE();
	SetCLK();	ClrCLK();
	SetOE();
	SetCLK();	ClrCLK();
	SetLE();
	SetCLK();	ClrCLK();
	ClrLE();
	SetCLK();	ClrCLK();
}

void Switch2Normal(){
	ClrCLK();
	ClrLE();
	SetOE();
	SetCLK();	ClrCLK();
	ClrOE();
	SetCLK();	ClrCLK();
	SetOE();
	SetCLK();	ClrCLK();
	SetCLK();	ClrCLK();
	SetCLK();	ClrCLK();
}

void LED_set(uint8_t data){
	ClrLE();
	ClrCLK();
	for(int i=0; i<8; i++){
		if(data & 0x0080) SetSDI(); else ClrSDI();
		data <<= 1;
		SetCLK(); ClrCLK();
	}
	ClrSDI();
	SetLE(); ClrLE();
}

void Conf_set(int Current_Gain){ // Current Gain from 25 to 99
	uint8_t data = conf_code_LUT[LUT_LED[Current_Gain - 25] - 25];
	ClrLE();
	SetOE();
	ClrCLK();
	for(int i=0; i<7; i++){
		if(data & 0x80) SetSDI(); else ClrSDI();
		data <<= 1;
		SetCLK(); ClrCLK();
	}
	if(data & 0x80) SetSDI(); else ClrSDI();
	SetLE();
	SetCLK(); ClrCLK();
	ClrLE();
	ClrSDI();
}

