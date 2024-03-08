#pragma once
#include "sysutil/sysutil_oskdialog.h"
#include "sysutil/sysutil_oskdialog_ext.h"
#include "sysutil/sysutil_msgdialog.h"
namespace diag
{
	void show(int t, char* str)
	{
		cellMsgDialogOpen(t, str, 0, 0, 0);
	}
	void show_auto(int t, char* str, float delay = 10.000) {

		cellMsgDialogOpen(t, str, 0, 0, 0);
		cellMsgDialogClose(delay);
	}
	void close() {
		cellMsgDialogClose(0);
	}
}