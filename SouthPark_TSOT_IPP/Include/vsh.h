#pragma once
#include "cell_sys.h"
#define CallFunction(address, return_type, func_name, args) \
	uint32_t func_name##opd[2] = { address, TOC }; \
	using func_name##_t = return_type(*)args; \
	func_name##_t func_name = (func_name##_t)func_name##opd;

template<typename R, typename... Arguments> inline R Call(long long function, Arguments... args)
{
	int toc_t[2] = { function, 0x014CDAB0 };
	R(*temp)(Arguments...) = (R(*)(Arguments...)) & toc_t;
	return temp(args...);
}
static int(*vsh_sprintf)(char*, const char*, ...);
static int(*vsh_swprintf)(wchar_t*, size_t, const wchar_t*, ...);
static int(*vshtask_A02D46E7)(int, const char*);

void* getNIDfunc(const char* vsh_module, uint32_t fnid)
{
	// 0x10000 = ELF
	// 0x10080 = segment 2 start
	// 0x10200 = code start

	uint32_t table = (*(uint32_t*)0x1008C) + 0x984; // vsh table address

	while (((uint32_t) * (uint32_t*)table) != 0)
	{
		uint32_t* export_stru_ptr = (uint32_t*)*(uint32_t*)table; // ptr to export stub, size 2C, "sys_io" usually... Exports:0000000000635BC0 stru_635BC0:    ExportStub_s <0x1C00, 1, 9, 0x39, 0, 0x2000000, aSys_io, ExportFNIDTable_sys_io, ExportStubTable_sys_io>

		const char* lib_name_ptr = (const char*)*(uint32_t*)((char*)export_stru_ptr + 0x10);

		if (strncmp(vsh_module, lib_name_ptr, strlen(lib_name_ptr)) == 0)
		{
			// we got the proper export struct
			uint32_t lib_fnid_ptr = *(uint32_t*)((char*)export_stru_ptr + 0x14);
			uint32_t lib_func_ptr = *(uint32_t*)((char*)export_stru_ptr + 0x18);
			uint16_t count = *(uint16_t*)((char*)export_stru_ptr + 6); // amount of exports

			for (int i = 0; i < count; i++)
			{
				if (fnid == *(uint32_t*)((char*)lib_fnid_ptr + i * 4))
					return (void*&)*((uint32_t*)(lib_func_ptr)+i); // take adress from OPD
			}
		}

		table = table + 4;
	}

	return 0;
}

void notify(const char* format, int param1, int param2)
{
	char tmp[0x100];
	vsh_sprintf(tmp, format, param1, param2);
	(void*&)(vshtask_A02D46E7) = (void*)((int)getNIDfunc("vshtask", 0xA02D46E7)); // notification message func
	vshtask_A02D46E7(0, tmp);
}

void notify(const char* format, int param1, int param2, int param3)
{
	char tmp[0x100];
	vsh_sprintf(tmp, format, param1, param2, param3);
	(void*&)(vshtask_A02D46E7) = (void*)((int)getNIDfunc("vshtask", 0xA02D46E7)); // notification message func
	vshtask_A02D46E7(0, tmp);
}

void notify(const char* format, int param1)
{
	char tmp[0x100];
	vsh_sprintf(tmp, format, param1);

	(void*&)(vshtask_A02D46E7) = (void*)((int)getNIDfunc("vshtask", 0xA02D46E7)); // notification message func
	vshtask_A02D46E7(0, tmp);
}

void notify(char* param)
{
	(void*&)(vshtask_A02D46E7) = (void*)((int)getNIDfunc("vshtask", 0xA02D46E7)); // notification message func
	vshtask_A02D46E7(0, param);
}