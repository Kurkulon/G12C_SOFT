#ifndef RS485_H__INCLUDED
#define RS485_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#include "common.h"

namespace Hardware
{
	class RS485 
	{
	public:
		static const unsigned int Frequency = 250000;
		static const unsigned int ByteLength = (1000000 * 12) / Frequency;
		
		static void Init();
		static void inline Idle() {};
		static void StartTX(unsigned char *data, unsigned short size);
		static void StartRX(unsigned char *data, unsigned short size);
		static void StopTX();
		static void StopRX();
		static bool ReadyTX();
		static bool ReadyRX();
		static unsigned short Pack(unsigned short *data, unsigned short size);
		static unsigned short UnPack(unsigned short *data, unsigned short size);
		static unsigned short GetTX();
		static unsigned short GetRX();
		
	};
} // namespace Hardware

#endif
