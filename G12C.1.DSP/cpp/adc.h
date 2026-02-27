#ifndef ADC_H__INCLUDED
#define ADC_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#define ADC16BIT

#include "common.h"

namespace Hardware
{
	class ADC // AD4020
	{
	public:

#ifdef ADC16BIT
		static const int			Max = 0xFFFF;
		static const int			Mid = 0x8000;
		static const int			Min = 0;
		static const int			Resolution = 16;
		static const unsigned int	Clock = 5000000; //Frequency * 40; // 20 bit + CNV
		static const unsigned int	ClockDiv = 20; //Frequency * 40; // 20 bit + CNV
		static const unsigned int	Freq = Clock/ClockDiv; 
		static const float			Frequency = 1.0 * Clock/ClockDiv; 
#else
		static const int Max = 0xFFFFF;
		static const int Mid = 0x80000;
		static const int Min = 0;
		static const int Resolution = 20;
		static const unsigned int Clock = 12500000;//Frequency * 40; // 20 bit + CNV
		static const unsigned int ClockDiv = 56; //Frequency * 40; // 20 bit + CNV
		static const unsigned int Frequency = Clock/ClockDiv;
#endif
		static void Init();
		static void Idle() {};
		static void Start(int *data, unsigned short size);
		static void Stop();
		static bool Ready();
		
	};
} // namespace Hardware

#endif
