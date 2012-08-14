Function RecoveryFunction2(Object, Method, Arguments, retVal)
    Class_Templates = Pathfinder.Locate("Class Templates.VBS")
	Engines_Path = Pathfinder.Locate("Engines.VBS")
	Filters_Path = Pathfinder.Locate("Filters.VBS")
	ReporterManager_Path = Pathfinder.Locate("ReporterManager.VBS")
	
	ExecuteFile Class_Templates
	ExecuteFile Engines_Path
	ExecuteFile Filters_Path
	ExecuteFile ReporterManager_Path

	If Dialog("TestCpp.exe").Exist(3) Then
		Dialog("TestCpp.exe").Activate
		Dialog("TestCpp.exe").Move 872,177
		Dialog("TestCpp.exe").WinObject("DirectUIHWND").Click 21,235
	End If

	Dim FileName ,TimeNow, ResPath
	ResPath = "D:\"
	TestNameNow=environment.Value("TestName") 
	FileName = ResPath & ""&TestNameNow & ".png"
	desktop.CaptureBitmap  filename,True
	systemutil.closedescendentprocesses


	If Dialog("TestCpp.exe").Exist(3) Then
   'Call oReporterManager.Report ("Fail", "Step 3" ,"Window Should Open" ,"App. Crashed", "Please check Error-datetime.png file")
		Dialog("TestCpp.exe").WinObject("关闭程序").Click
	Else
		Call oReporterManager.Report ("Fail", "Step 3" ,"Window Should Open" ,"App. Crashed", "App close by accident")
	End If
End Function 
