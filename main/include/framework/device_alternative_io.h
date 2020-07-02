#pragma once

#ifdef GFW_WIN32
#define myfopen(p, q, r) fopen_s(&(p), (q), (r))
#else
#define myfopen(p, q, r) (p) = fopen((q), (r))
#endif


#ifdef GFW_WIN32
#define my_strtok(p, q, r) strtok_s((p), (q), (r))
#else
#define my_strtok(p, q, r) strtok((p), (q))
#endif