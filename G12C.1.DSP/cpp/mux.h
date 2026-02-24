#ifndef MUX_H__INCLUDED
#define MUX_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#include "common.h"
#include "adc.h"

namespace Hardware
{
	class MUX // NXP74HC595
	{
	private:
	public:
		static const unsigned int Frequency = 1000000;
		static const unsigned int Clk = 10000000;
		static void Init();
		static void inline Idle() {};
		static void Set(unsigned char data);
	};
}

#endif
