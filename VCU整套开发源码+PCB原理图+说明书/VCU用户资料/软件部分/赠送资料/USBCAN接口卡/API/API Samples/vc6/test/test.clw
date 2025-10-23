; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "test.h"

ClassCount=3
Class1=CTestApp
Class2=CTestDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TEST_DIALOG
Resource4=IDR_MENU1

[CLS:CTestApp]
Type=0
HeaderFile=test.h
ImplementationFile=test.cpp
Filter=N

[CLS:CTestDlg]
Type=0
HeaderFile=testDlg.h
ImplementationFile=testDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_COMBO_devtype

[CLS:CAboutDlg]
Type=0
HeaderFile=testDlg.h
ImplementationFile=testDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TEST_DIALOG]
Type=1
Class=CTestDlg
ControlCount=46
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_INDEX,combobox,1344339971
Control4=IDC_BUTTON_CONNECT,button,1342242817
Control5=IDC_BUTTON_STARTCAN,button,1342242816
Control6=IDC_BUTTON_RESETCAN,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_SENDTYPE,combobox,1344339971
Control9=IDC_STATIC,static,1342308352
Control10=IDC_COMBO_SENDFRAMETYPE,combobox,1344339971
Control11=IDC_STATIC,static,1342308352
Control12=IDC_COMBO_SENDFRAMEFORMAT,combobox,1344339971
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_SENDFRAMEID,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT_SENDDATA,edit,1350631552
Control17=IDC_BUTTON_SEND,button,1342242816
Control18=IDC_STATIC,button,1342177287
Control19=IDC_LIST_INFO,listbox,1352728835
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308352
Control22=IDC_COMBO_CANIND,combobox,1344339971
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT_CODE,edit,1350631552
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_MASK,edit,1350631552
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT_TIMING0,edit,1484849280
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_TIMING1,edit,1484849280
Control32=IDC_STATIC,static,1342308352
Control33=IDC_COMBO_FILTERTYPE,combobox,1344339971
Control34=IDC_STATIC,static,1342308352
Control35=IDC_COMBO_MODE,combobox,1344339971
Control36=IDC_BUTTON_recvsend,button,1073807360
Control37=IDC_COMBO_BAUD1,combobox,1344339971
Control38=IDC_STATIC,static,1342308352
Control39=IDC_BUTTON_clear,button,1342242816
Control40=IDC_STATIC,static,1342308352
Control41=IDC_COMBO_devtype,combobox,1344339971
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_BUTTON_clearcnt,button,1342242816
Control45=IDC_STATIC_recv,static,1342308352
Control46=IDC_STATIC_send,static,1342308352

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_MENU_REFRESH
CommandCount=1

