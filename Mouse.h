#pragma once
#include <windows.h>
struct Mouse
{
	Mouse() {
		pos.x = 0;
		pos.y = 0;
		startpos.x = 0;
		startpos.y = 0;
	}

	POINT pos;
	POINT startpos;
};
