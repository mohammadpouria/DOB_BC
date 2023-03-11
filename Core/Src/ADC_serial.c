#include "ADC_serial.h"

void inline SetSDATA(void){
	__ASM("NOP");
	AD_SDATA_GPIO_Port->BSRR = AD_SDATA_Pin;
	__ASM("NOP");
}

void inline ClrSDATA(void){
	__ASM("NOP");
	AD_SDATA_GPIO_Port->BSRR = AD_SDATA_Pin << 16U;
	__ASM("NOP");
}

void inline SetSCLK(void){
	__ASM("NOP");
	AD_SCLK_GPIO_Port->BSRR = AD_SCLK_Pin;
	__ASM("NOP");
}

void inline ClrSCLK(void){
	__ASM("NOP");
	AD_SCLK_GPIO_Port->BSRR = AD_SCLK_Pin << 16U;
	__ASM("NOP");
}

void inline SetSLOAD(void){
	__ASM("NOP");
	AD_SLOAD_GPIO_Port->BSRR = AD_SLOAD_Pin;
	__ASM("NOP");
}

void inline ClrSLOAD(void){
	__ASM("NOP");
	AD_SLOAD_GPIO_Port->BSRR = AD_SLOAD_Pin << 16U;
	__ASM("NOP");
}

void Serial_Write(char add, uint16_t data){
	SetSLOAD();
	ClrSCLK();
//	for (int i=0; i<10; i++); // delay
	ClrSLOAD();
	ClrSDATA();
	SetSCLK();	ClrSCLK();
	for (int i=2; i>=0; i--){
		if (add & 0x04) SetSDATA(); else ClrSDATA();
		SetSCLK();	ClrSCLK();
		add <<= 1;
	}
	ClrSDATA();
	for (int i=0; i<3; i++){
		SetSCLK();	ClrSCLK();
	}
	for (int i=8; i>=0; i--){
		if (data & 0x0100) SetSDATA(); else ClrSDATA();
		SetSCLK();	ClrSCLK();
		data <<= 1;
	}
	ClrSCLK();
	SetSLOAD();
}

void ADC_init(void){
	Serial_Write(0x01, 0x0010); // MUX register setting
	for (int i=0; i<50; i++); // delay
	Serial_Write(0x00, 0x0051);// Configuration register settings
	for (int i=0; i<50; i++); // delay
//	Serial_Write(0x04, 0x0003);
}
