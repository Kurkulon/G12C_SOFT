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
		static const unsigned int Frequency = 500000;
		static const int Max = 0xFFFF;
		static const int Mid = 0x8000;
		static const int Min = 0;
		static const int Resolution = 16;
		static const unsigned int Clock = Frequency * 50; // 20 bit + CNV
#else
		static const unsigned int Frequency = 250000;
		static const int Max = 0xFFFFF;
		static const int Mid = 0x80000;
		static const int Min = 0;
		static const int Resolution = 20;
		static const unsigned int Clock = Frequency * 25; // 20 bit + CNV
#endif
		static void Init();
		static void Idle() {};
		static void Start(int *data, unsigned short size);
		static void Stop();
		static bool Ready();
		
	};
} // namespace Hardware

#endif
