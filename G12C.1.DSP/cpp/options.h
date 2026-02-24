
#ifndef OPTIONS_H__INCLUDED
#define OPTIONS_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

namespace Device
{
	class Options
	{
	private:
#pragma pack(1)
		typedef struct
		{
			unsigned short number;
		} special_options_type;
		typedef struct
		{
			unsigned short amp;
			unsigned short step;
			unsigned short length;
			unsigned short filter;
		} wave_options_type;
		typedef struct
		{
				unsigned short channels;
				unsigned short overlapp;
		} spectrum_options_type;
		typedef struct
		{
			special_options_type special;
			wave_options_type wave;
			spectrum_options_type spectrum;
		} options_type;

#pragma pack()
		static unsigned int _options_buffer[(sizeof(options_type) + 1) / 2]; // pack16
		static options_type *_options;
		static bool _save;
		static bool _ready;

	public:
		static void Init();
		static void Idle();
		static unsigned short GetNumber();
		static void SetNumber(unsigned short number);
		static unsigned short GetWaveAmp();
		static void SetWaveAmp(unsigned short amp);
		static unsigned short GetWaveStep();
		static void SetWaveStep(unsigned short step);
		static unsigned short GetWaveLength();
		static void SetWaveLength(unsigned short length);
		static unsigned short GetWaveFilter();
		static void SetWaveFilter(unsigned short filter);
		static unsigned short GetSpectrumChannels();
		static void SetSpectrumChannels(unsigned short channels);
		static unsigned short GetSpectrumOverlapp();
		static void SetSpectrumOverlapp(unsigned short overlapp);
		static void Save() { _save = true; }
		static bool Ready() { return _ready; }
	};

}

#endif
