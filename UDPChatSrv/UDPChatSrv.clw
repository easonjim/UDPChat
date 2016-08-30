; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUDPChatSrvDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "UDPChatSrv.h"

ClassCount=3
Class1=CUDPChatSrvApp
Class2=CUDPChatSrvDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_UDPCHATSRV_DIALOG

[CLS:CUDPChatSrvApp]
Type=0
HeaderFile=UDPChatSrv.h
ImplementationFile=UDPChatSrv.cpp
Filter=N

[CLS:CUDPChatSrvDlg]
Type=0
HeaderFile=UDPChatSrvDlg.h
ImplementationFile=UDPChatSrvDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_PORT

[CLS:CAboutDlg]
Type=0
HeaderFile=UDPChatSrvDlg.h
ImplementationFile=UDPChatSrvDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_UDPCHATSRV_DIALOG]
Type=1
Class=CUDPChatSrvDlg
ControlCount=7
Control1=IDC_BUTTON_START,button,1342275585
Control2=IDC_BUTTON_CANCEL,button,1342275584
Control3=IDC_EDIT_RECV,edit,1353777348
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_EDIT_PORT,edit,1350639744
Control7=IDC_STATIC,static,1342308352

