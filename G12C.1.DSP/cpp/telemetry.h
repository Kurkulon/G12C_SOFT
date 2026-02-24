
#ifndef TELEMETRY_H__INCLUDED
#define TELEMETRY_H__INCLUDED

#include "rs485.h"

namespace Device
{
	class Telemetry
	{
	private:
		static inline void TimerStart(unsigned int mks)
		{
			sysreg_bit_clr(sysreg_IRPTL, TMZHI);
			timer_set(mks * (CCLK / 1000000), mks * (CCLK / 1000000));
			timer_on();
		}
		static inline void TimerStop()
		{
			timer_off();
			sysreg_bit_clr(sysreg_IRPTL, TMZHI);
		}
		static inline bool TimerEvent()
		{
			return sysreg_bit_tst(sysreg_IRPTL, TMZHI);
		}

		static unsigned short Parse(unsigned short *receive_pdata, unsigned short receive_count, unsigned short *transceive_pdata);
		static const unsigned short _receive_data_size = 16;	  // words
		static const unsigned short _transceive_data_size = 2100; // words
		static const unsigned short _receive_pause_before = Hardware::RS485::ByteLength * 2;
		static const unsigned short _receive_pause_after = Hardware::RS485::ByteLength * 2;

#pragma pack(1)

		typedef struct
		{
			unsigned short request;
		} receive_type;
		typedef struct
		{
			unsigned short request;
			unsigned short data[2];
		} receive_extended_type;
		typedef struct
		{
			unsigned short response;
		} transceive_type;
		typedef struct
		{
			unsigned short response;
			unsigned short number;
			unsigned short version;
		} transceive_identification_type;
		typedef struct
		{
			unsigned short response;
			unsigned short amp;
			unsigned short step;
			unsigned short length;
			unsigned short filter;
			unsigned short channels;
			unsigned short overlapp;
		} transceive_options_type;
		typedef struct
		{
			unsigned short response;
			struct
			{
				unsigned short amp;
				unsigned short step;
				unsigned short length;
				unsigned short filter;
				unsigned short channels;
				unsigned short partition[2];
				unsigned short overlapp;
			} options;
			struct
			{
				unsigned short amplitude_max;
				unsigned short amplitude_mid;
				unsigned short temperature;
			} measure;
			unsigned short spectrum[]; // упакуется
		} transceive_measure_type;
#pragma pack()

	public:
		static void Init() {};
		static void Idle();
	};
}

#endif
