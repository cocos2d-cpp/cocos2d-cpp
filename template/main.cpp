// Application main file.
 
// Original file name: cocosTemplateEntry.cpp
// Generated by TOPS Builder:Project wizard,Date:2010-09-27
 
#include  "TG3.h"
#include  "AppDelegate.h"
#include  "cocostemplate_res_c.h"

const  ResourceRegisterEntry		ResRegList_cocosTemplate[]  =
{
	TG_RESOURCE_DEFINE
};

extern const  AppResourceEntry		cocosTemplateResourceEntry  =
{
	(ResourceRegisterEntry*)ResRegList_cocosTemplate,                    //  res  list  in  this  app
		sizeof(ResRegList_cocosTemplate) /  sizeof(ResourceRegisterEntry),    //number  of  item  in  res
};



Int32 TG3AppMain(const TUChar * pAppID, UInt32 nCmd, void * pCmdParam)
{
	AppDelegate app;
	app.WM_SetResourceEntry(&cocosTemplateResourceEntry);
	app.Run();

	return 1;
}


