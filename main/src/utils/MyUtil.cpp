#include"utils/MyUtil.h"
#include<stdio.h>
#include<stdlib.h>

#ifdef GFW_WIN32
#define myfopen(p, q, r) fopen_s(&(p), (q), (r))
#else
#define myfopen(p, q, r) (p) = fopen((q), (r))
#endif

bool resetLog()
{
	FILE *dousalog = NULL;
	myfopen(dousalog, "log.txt", "w");
	if (dousalog)
	{
		fputs("MKFJ log\n", dousalog);
		fputs("Built on ", dousalog);
		fputs(__DATE__ " " __TIME__, dousalog);
		fputs("\n", dousalog);
		fclose(dousalog);
		dousalog = NULL;
		return 1;
	}
	else return 0;
}

bool outputLog(const std::string& str)
{
	FILE *dousalog = NULL;
	myfopen(dousalog, "log.txt", "a");
	if (dousalog)
	{
		fputs((str + "\n").c_str(), dousalog);
		fclose(dousalog);
		dousalog = NULL;
		return 1;
	}
	else return 0;
}