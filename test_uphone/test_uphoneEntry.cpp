// Application main file.
 
// Original file name: test_uphoneEntry.cpp
// Generated by TOPS Builder:Project wizard,Date:2010-7-26
 

 
#include "TG3.h"
#include "test_uphoneApp.h"

Int32 TG3AppMain(const TUChar * pAppID, UInt32 nCmd, void * pCmdParam)
{
	switch(nCmd)
	{
	case 0: // 主入口
		{
			// UI应用程序例子：
			Ttest_uphoneApp  *  pApp= new  Ttest_uphoneApp();
			pApp->Run();

			delete pApp;
			break;
		}
	}
	return 1;
}


