
#ifndef DATA_H__INCLUDED
#define DATA_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

#include "calculation.h"

namespace Software
{
	class Data
	{
	private:
#pragma pack(1)
		typedef struct
		{
			unsigned int counter;
		} special_data_type;
		typedef struct
		{
			unsigned short amplitude_max;
			unsigned short amplitude_mid;
		} measure_data_type;
		typedef struct
		{
			struct
			{
				unsigned short amp;
				unsigned short step;
				unsigned short length;
				unsigned short filter;
			} options;
			int wave[Software::Calculation::Length+16]; // без упаковки, 20 бит
		} wave_data_type;
		typedef struct
		{
			struct
			{
				unsigned short channels;
				float partition;
				unsigned short overlapp;
			} options;
			unsigned short spectrum[Software::Calculation::MaxChannels / 2]; // упаковано  по 2 ushort
		} spectrum_data_type;
		typedef struct
		{
			special_data_type special;
			measure_data_type measure;
			wave_data_type wave;
			spectrum_data_type spectrum;
		} data_type;
#pragma pack()
		static data_type _data;

	public:
		static void Init();
		static void Idle();
		static inline unsigned int GetCounter() { return _data.special.counter; }
		static void SetCounter(unsigned int counter) { _data.special.counter = counter; }
		static inline unsigned short GetWaveAmp() { return _data.wave.options.amp; }
		static inline void SetWaveAmp(unsigned short amp) { _data.wave.options.amp = amp; }
		static inline unsigned short GetWaveStep() { return _data.wave.options.step; }
		static inline void SetWaveStep(unsigned short step) { _data.wave.options.step = step; }
		static inline unsigned short GetWaveLength() { return _data.wave.options.length; }
		static inline void SetWaveLength(unsigned short length) { _data.wave.options.length = length; }
		static inline unsigned short GetWaveFilter() { return _data.wave.options.filter; }
		static inline void SetWaveFilter(unsigned short filter) { _data.wave.options.filter = filter; }
		static inline unsigned short GetSpectrumChannels() { return _data.spectrum.options.channels; }
		static inline void SetSpectrumChannels(unsigned short channels) { _data.spectrum.options.channels = channels; }
		static inline float GetSpectrumPartition() { return _data.spectrum.options.partition; }
		static inline void SetSpectrumPartition(float partition) { _data.spectrum.options.partition = partition; }
		static inline unsigned short GetSpectrumOverlapp() { return _data.spectrum.options.overlapp; }
		static inline void SetSpectrumOverlapp(unsigned short overlapp) { _data.spectrum.options.overlapp = overlapp; }
		static inline unsigned short GetMeasureAmplitudeMax() { return _data.measure.amplitude_max; }
		static inline void SetMeasureAmplitudeMax(unsigned short amplitude) { _data.measure.amplitude_max = amplitude; }
		static inline unsigned short GetMeasureAmplitudeMid() { return _data.measure.amplitude_mid; }
		static inline void SetMeasureAmplitudeMid(unsigned short amplitude) { _data.measure.amplitude_mid = amplitude; }
		static inline int *GetWave() { return (int *)((unsigned int)(_data.wave.wave)); }
		static inline int *GetWaveBuffer() { return (int *)(((unsigned int)(_data.wave.wave))); }
		static inline unsigned short *GetSpectrum() { return (unsigned short *)(((unsigned int)(_data.spectrum.spectrum) << 1)); }
		static inline unsigned int *GetSpectrumBuffer() { return (unsigned int *)(((unsigned int)(_data.spectrum.spectrum))); }
	};

}

#endif
