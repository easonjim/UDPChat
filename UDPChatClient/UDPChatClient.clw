; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUDPChatClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "UDPChatClient.h"

ClassCount=3
Class1=CUDPChatClientApp
Class2=CUDPChatClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_UDPCHATCLIENT_DIALOG

[CLS:CUDPChatClientApp]
Type=0
HeaderFile=UDPChatClient.h
ImplementationFile=UDPChatClient.cpp
Filter=N

[CLS:CUDPChatClientDlg]
Type=0
HeaderFile=UDPChatClientDlg.h
ImplementationFile=UDPChatClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_IPADDRESS_SRVIP

[CLS:CAboutDlg]
Type=0
HeaderFile=UDPChatClientDlg.h
ImplementationFile=UDPChatClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_UDPCHATCLIENT_DIALOG]
Type=1
Class=CUDPChatClientDlg
ControlCount=12
Control1=IDC_EDIT_SEND,edit,1350631552
Control2=IDC_BUTTON_SEND,button,1342275585
Control3=IDC_IPADDRESS_SRVIP,SysIPAddress32,1342242816
Control4=IDC_EDIT_SRVPORT,edit,1350639744
Control5=IDC_BUTTON_LINK,button,1342275584
Control6=IDC_BUTTON_CANCEL,button,1342275584
Control7=IDC_STATIC,button,1342178055
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177543
Control11=IDC_EDIT_RECV,edit,1353777348
Control12=IDC_STATIC,button,1342177543

