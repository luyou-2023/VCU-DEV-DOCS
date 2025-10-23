unit ControlCAN;

interface

uses
  WinTypes;



const
  DLL_NAME  = 'ControlCAN.dll';//动态库名称

VCI_PCI5121	=	1  ;
VCI_PCI9810	=	2  ;
VCI_USBCAN1	=	3   ;
VCI_USBCAN2	=	4  ;
VCI_USBCAN2A =	4  ;
VCI_PCI9820	=	5 ;
VCI_CAN232	=	6 ;
VCI_PCI5110	=	7  ;
VCI_CANLITE	=	8  ;
VCI_ISA9620	=	9  ;
VCI_ISA5420	=	10 ;
VCI_PC104CAN =	11  ;
VCI_CANETUDP =	12 ;
VCI_CANETE	=	12 ;
VCI_DNP9810	=	13  ;
VCI_PCI9840	=	14  ;
VCI_PC104CAN2=	15 ;
VCI_PCI9820I=	16   ;
VCI_CANETTCP =	17 ;
VCI_PEC9920	=	18    ;
VCI_PCI5010U =	19  ;
VCI_USBCAN_E_U =	20  ;
VCI_USBCAN_2E_U= 21  ;
VCI_PCI5020U =	22  ;
VCI_EG2OT_CAN=	23  ;

type

//声明各个数据结构

//1.ZLGCAN系列接口卡信息的数据类型。
VCI_BOARD_INFO = Record
		hw_Version : WORD;
		fw_Version : WORD;
		dr_Version : WORD;
		in_Version : WORD;
		irq_Num : WORD;
		can_Num : BYTE;
		str_Serial_Num : array[0..19] of CHAR;
		str_hw_Type : array[0..39] of CHAR;
		Reserved : array[0..3] of WORD;
END;

PVCI_BOARD_INFO=^VCI_BOARD_INFO;

//2.定义CAN信息帧的数据类型。
VCI_CAN_OBJ = Record
	ID : UINT;
	TimeStamp : UINT;
	TimeFlag : BYTE;
	SendType : BYTE;
	RemoteFlag : BYTE;//是否是远程帧
	ExternFlag : BYTE;//是否是扩展帧
	DataLen : BYTE;
	Data : array[0..7] of BYTE;
	Reserved : array[0..2] of BYTE;
END;

PVCI_CAN_OBJ = ^VCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
VCI_CAN_STATUS = Record
	ErrInterrupt : UCHAR;
	regMode : UCHAR;
	regStatus : UCHAR;
	regALCapture : UCHAR;
	regECCapture : UCHAR;
	regEWLimit : UCHAR;
	regRECounter : UCHAR;
	regTECounter : UCHAR;
	Reserved : DWORD;
END;

PVCI_CAN_STATUS = ^VCI_CAN_STATUS;

//4.定义错误信息的数据类型。
VCI_ERR_INFO = Record
		ErrCode : UINT;
		Passive_ErrData : array[0..2] of BYTE;
		ArLost_ErrData : BYTE;
END;

PVCI_ERR_INFO = ^VCI_ERR_INFO;

//5.定义初始化CAN的数据类型
VCI_INIT_CONFIG = Record
	AccCode : DWORD;
	AccMask : DWORD;
	Reserved : DWORD;
	Filter : UCHAR;
	Timing0 : UCHAR;
	Timing1 : UCHAR;
	Mode : UCHAR;
END;

PVCI_INIT_CONFIG = ^VCI_INIT_CONFIG;
                    
//导入动态库函数

function VCI_OpenDevice ( DeviceType  : DWORD;
                          DeviceInd : DWORD;
                          Reserved : DWORD) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_CloseDevice ( DeviceType  : DWORD;
                           DeviceInd : DWORD) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_InitCAN ( DeviceType  : DWORD;
                       DeviceInd : DWORD;
                       CANInd : DWORD;
                       pInitConfig : PVCI_INIT_CONFIG) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_ReadBoardInfo ( DeviceType  : DWORD;
                             DeviceInd : DWORD;
                             pInfo : PVCI_BOARD_INFO) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_ReadErrInfo ( DeviceType  : DWORD;
                           DeviceInd : DWORD;
                           CANInd : DWORD;
                           pErrInfo : PVCI_ERR_INFO) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_ReadCANStatus ( DeviceType  : DWORD;
                             DeviceInd : DWORD;
                             CANInd : DWORD;
                             pCANStatus : PVCI_CAN_STATUS) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_GetReference ( DeviceType  : DWORD;
                            DeviceInd : DWORD;
                            CANInd : DWORD;
                            RefType : DWORD;
                            pData : Pointer) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_SetReference ( DeviceType  : DWORD;
                            DeviceInd : DWORD;
                            CANInd : DWORD;
                            RefType : DWORD;
                            pData : Pointer) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_GetReceiveNum ( DeviceType  : DWORD;
                             DeviceInd : DWORD;
                             CANInd : DWORD) : ULONG;
  stdcall;
  external DLL_NAME;

function VCI_ClearBuffer ( DeviceType  : DWORD;
                           DeviceInd : DWORD;
                           CANInd : DWORD) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_StartCAN ( DeviceType  : DWORD;
                        DeviceInd : DWORD;
                        CANInd : DWORD) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_ResetCAN ( DeviceType  : DWORD;
                        DeviceInd : DWORD;
                        CANInd : DWORD) : DWORD;
  stdcall;
  external DLL_NAME;

function VCI_Transmit ( DeviceType  : DWORD;
                        DeviceInd : DWORD;
                        CANInd : DWORD;
                        pSend : PVCI_CAN_OBJ;
                        Len : ULONG) : ULONG;
  stdcall;
  external DLL_NAME;

function VCI_Receive ( DeviceType  : DWORD;
                        DeviceInd : DWORD;
                        CANInd : DWORD;
                        pReceive : PVCI_CAN_OBJ;
                        Len : ULONG;
                        WaitTime : integer) : ULONG;
  stdcall;
  external DLL_NAME;


implementation

end.
