#include "common.h"
#include "main.h"
#include "debug.h"
#include "twi.h"
#include "mux.h"
#include "adc.h"
#include "rs485.h"
#include "options.h"
#include "telemetry.h"
#include "control.h"
#include "calculation.h"
#include "data.h"
		
void main()
{
	Hardware::Main::Init();
	Hardware::Debug::Init();
	Hardware::TWI::Init();
	Hardware::MUX::Init();
	Hardware::ADC::Init();
	Hardware::RS485::Init();
	Software::Calculation::Init();
	Software::Data::Init();
	Device::Options::Init();
	Device::Telemetry::Init();
	Device::Control::Init();


	for (;;)
	{
		Hardware::Main::Idle(); 
		Hardware::Debug::Idle();
		Hardware::TWI::Idle();
		Hardware::MUX::Idle();
		Hardware::ADC::Idle();
		Hardware::RS485::Idle();
		Software::Calculation::Idle();
		Software::Data::Idle();
		Device::Options::Idle();
		Device::Telemetry::Idle();
		Device::Control::Idle();
		
	}
}
