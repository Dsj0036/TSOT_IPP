#pragma once
#include "System.h"
#include "../PS3_CELL_UTIL/FileSystem.h"

namespace Debug {
	#define PATH(x) ("/dev_hdd0/tmp/TSoT/" x)


	char* getOutputPath() {
		return PATH("output.html");
	}

	void Initialize() {
		FS_MKDIR(PATH());

		char* outputPath = getOutputPath();

		FS_RECREATE(outputPath);
		char* head = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\"";
		const char* tags[]{
			"<meta charset=\"UTF-8\">", 
			"<meta http-equiv=\"refresh\" content=\"5\">",
			"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		};	
		char* head2 = "</head><body>";
		FS_APPEND_STRING(outputPath, head);
		for (byte i = 0; i < 3 ; i++) {
			FS_APPEND_STRING(outputPath, (char*)tags[i]);
		}

	}
	// Without line ending.
	void cout(char* buffer) {
		FS_APPEND_STRING(getOutputPath(), buffer);
	}
	void coutl(char* buffer) {
		int buffLen = strlen(buffer);
		if ((!buffer) || buffLen <= 0) {
			return;
		}
		
		auto buff = (char*)malloc(buffLen);
		_sys_strncat(buff, buffer, buffLen);
		_sys_strncat(buff, "\n", 2);
		cout(buffer);
		free(buff);
	}
	template <typename ...Arguments>
	void coutf(char* format, Arguments ...s) {
		int len = strlen(format);
		int buffLen = len * 1.55;
		auto buff = (char*)malloc(buffLen);
		_sys_snprintf(buff, buffLen, format, s...);
		cout(buff);
		free(buff);
	}


}


#ifdef DEBUG
#define __printf Debug::coutf
#define __printl Debug::coutl
#define __print Debug::cout
#else 
#define __printf __nop();
#define __printl __nop();
#define __print  __nop();
#endif 
#define ONCE(tracedName) \
namespace { \
    bool traced_##tracedName = false; \
} \
void traced_##tracedName##_func() { \
    if (!traced_##tracedName) { \
        __printl("(ONCE) " #tracedName " is getting called."); \
        traced_##tracedName = true; \
    } \
}
