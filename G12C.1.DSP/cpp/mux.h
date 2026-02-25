#ifndef MUX_H__INCLUDED
#define MUX_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#include "common.h"
#include "adc.h"

namespace Hardware
{
	class MUX // ADG739
	{
	private:
	public:
		static const unsigned int Frequency = 1000000;
		static const unsigned int Clock = 10000000;
		static const unsigned char Min = 0;
		static const unsigned char Max = 0x0F;
		static void Init();
		static void inline Idle() {};
		static void Set(unsigned char data);
	};
}

#endif
