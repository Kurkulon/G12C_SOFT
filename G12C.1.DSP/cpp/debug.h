
#ifndef DEBUG_H__INCLUDED
#define DEBUG_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

namespace Hardware
{
class Debug
{
private:

public:
	static void Init();
	static void inline Idle() {};
	static void TP(bool state);
};
} // namespace Hardware

#endif
