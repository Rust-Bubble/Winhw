
// EmergencyGateway.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// AppEG:
// 有关此类的实现，请参阅 EmergencyGateway.cpp
//

class AppEG : public CWinApp{
public:
	bool hasSearchingStarted;

public:
	AppEG();

protected:
	virtual BOOL InitInstance() override;
	DECLARE_MESSAGE_MAP()
public:
};

extern AppEG appInstance;