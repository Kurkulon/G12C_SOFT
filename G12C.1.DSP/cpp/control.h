
#ifndef CONTROL_H__INCLUDED
#define CONTROL_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

namespace Device
{
	class Control
	{
	private:
		enum status_type
		{
			STATUS_WAIT,
			STATUS_START,
			STATUS_MEASURE,
			STATUS_MATH,
			STATUS_READY,
		};
		static status_type _status;
		static unsigned char _math;

	public:
		static void Set(unsigned char math);
		static void Start();
		static void Init();
		static void Idle();
		static unsigned short GetTemperature() { return 0; } // а его в этой плате нет
	};

}

#endif
