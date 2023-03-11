#ifndef ADC_SERIAL_H
#define ADC_SERIAL_H

#include	"main.h"

struct Register {
	char Address;
	uint16_t Data;
};	

//struct ADC_registers {
//	struct Register Configuration = {0b000, 0b001010000};
//	struct Register MUX = {0b001, 0b000010000};
//	struct Register Blue_PGA = {0b100, 0b000000000};
//	struct Register Blue_offset = {0b111, 0b000000000}
//};

		
void SetSDATA(void);
void ClrSDATA(void);
void SetSCLK(void);
void ClrSCLK(void);
void SetSLOAD(void);
void ClrSLOAD(void);
void Serial_Write(char add, uint16_t data);
struct Register Serial_Read(char add, uint16_t data);
void ADC_init(void);



#endif
