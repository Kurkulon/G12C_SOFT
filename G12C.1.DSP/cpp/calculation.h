
#ifndef CALCULATION_H__INCLUDED
#define CALCULATION_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#include "adc.h"

namespace Software
{
	class Calculation
	{
	public:
		enum math_type
		{
			MATH_AVERAGE,
			MATH_MAXIMUM,
			MATH_UNKNOWN,
		};
		static const unsigned int Step = (1000000 / Hardware::ADC::Frequency);
		static const unsigned int Length = (1 << 14); 
		static const unsigned int MaxChannels = (1 << 10);
		static const unsigned int MinChannels = 2;
		static const unsigned int MaxOverlapp = 1000;
		static const unsigned int MinOverlapp = 0;
	private:
		static bool FFT(complex_float *data, unsigned int *size, bool inverse);
		static bool Partition(math_type math, complex_float *data, unsigned int size, unsigned short *out, unsigned int *out_size, float *partition, float overlapping);
		static complex_float _data[Length];
		static complex_float _coef[16];
		static const float _partition[MaxChannels];
	public:
		static bool Calculate(math_type math, unsigned short *in, unsigned int in_size, unsigned short *out, unsigned int *out_size, float *partition, float overlapping);
		static void Init();
		static void Idle() {};
	};
	
	
}

#endif
