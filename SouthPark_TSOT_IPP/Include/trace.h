#pragma once
#include "cell_fs.h"
#include "cell/rtc.h"
namespace debug{
	
	char* fn = "/dev_hdd0/trace.log";
	void create()
	{
		createFile(fn);
		appendStringToFile("Create trace\n");
	}
	int clear() {
		return deleteFile(fn);

	}
	namespace out {
		char foofrm_mem[200];
		void write(char c) {
			char foo[1]{ c };
			appendToFile(fn, foo);
		}
		void write(char* strptr)
		{
			appendStringToFile(fn, strptr);
		}
		void writeL(char* strptr) {
			appendStringToFile(fn, strptr);
			write(10);
		}
		void writeif(bool expr, char* outp) {
			if (expr) {
				write(outp);
			}
		}
		void writeN(char* strfrmptr, int i) {
			if (_sys_strlen(strfrmptr) > 199) {
				writeL("[snprintf writing error; Too long. PointlesS?]");
			}
			else
			{
				clear_str(foofrm_mem);
				_sys_sprintf(foofrm_mem, strfrmptr, i);
				write(foofrm_mem);
			}
		}
	}
}