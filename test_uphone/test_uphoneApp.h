// Application application header file.
 
// Original file name: test_uphoneApp.h
// Generated by TOPS Builder:Project wizard,Date:2010-7-26
 

#ifndef  __test_uphone_App_H__
#define  __test_uphone_App_H__
// #include "CCXApplication.h"
// #include "CCXEGLView.h"
#include "cocos2d.h"

class  Ttest_uphoneApp  :  public  cocos2d::CCXApplication
{
public:
	Ttest_uphoneApp();
	~Ttest_uphoneApp();

    virtual bool initCocos2d();

public:
	virtual Boolean EventHandler(EventType * pEvent);

    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

protected:
    cocos2d::CCXEGLView * m_pMainWnd;

private:
    Int32 m_nTimer;
};
 

#endif

