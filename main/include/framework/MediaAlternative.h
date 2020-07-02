#pragma once
typedef long LONG;
#include"framework/BaseMedia.h"

#ifdef GFW_WIN32
#include"framework/Media.h"
#else
#include"framework/MediaToFile.h"
#endif
