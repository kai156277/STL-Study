// STL-Study.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"

// memory leak detection
#define _CRTDBG_MAP_ALLOC // memory leak info add file name and line number
#include <cstdlib>
#include <crtdbg.h>

#include "tiny_unit_test.h"
#include "tiny_vector.h"
#include "tiny_list.h"
#include <vector>
#include <string>
//#include <list>

int main(int argc, char *argv[])
{
    // if detected memory leaks, before program quit will print info.
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1); 	// memory snapshot s1

	size_t len = 10;
	std::string value = "test string";
	tiny::vector<std::string> a(len, value);

	for (size_t i = 0; i < a.size(); ++i)
		std::cout << a[i] << std::endl;
    _CrtMemCheckpoint(&s2);		// memory snapshot s2

    if(_CrtMemDifference(&s3, &s1, &s2)) // diff snapshot
    {
        _RPTF0( _CRT_WARN ,"_CrtMemDumpStatistics: \n");
        _CrtMemDumpStatistics(&s3);
    }
	printf("press any key to continue...");
	getchar();
    return 0;
}

