
#ifndef TWI_H__INCLUDED
#define TWI_H__INCLUDED

#ifndef __cplusplus
#error C++ compilation required.
#endif

namespace Hardware
{
	class TWI
	{
	private:
		typedef struct
		{
			unsigned char dimersion; // 1byte..4byte
			unsigned short count; // in bytes
			unsigned int *data;
		} descriptor_subtype;
	public:
		struct Descriptor
		{
			Descriptor *next;
			unsigned char address;
			volatile bool ready;
			volatile bool error;
			descriptor_subtype command;
			descriptor_subtype write;
			descriptor_subtype read;
		};

	private:
		static Descriptor *_descriptor;
		static Descriptor *_descriptor_last;

	public:
		static void Init();
		static void Idle();
		static bool Handle(Descriptor *dsc);
	};
} // namespace Hardware

#endif
