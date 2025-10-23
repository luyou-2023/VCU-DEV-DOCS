//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

#define VCI_PCI5121		1
#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN2A	4
#define VCI_PCI9820		5
#define VCI_CAN232		6
#define VCI_PCI5110		7
#define VCI_CANLITE		8
#define VCI_ISA9620		9
#define VCI_ISA5420		10
#define VCI_PC104CAN	11
#define VCI_CANETUDP	12
#define VCI_CANETE		12
#define VCI_DNP9810		13
#define VCI_PCI9840		14
#define VCI_PC104CAN2	15
#define VCI_PCI9820I	16
#define VCI_CANETTCP	17
#define VCI_PEC9920		18
#define VCI_PCI5010U	19
#define VCI_USBCAN_E_U	20
#define VCI_USBCAN_2E_U 21
#define VCI_PCI5020U	22
#define VCI_EG2OT_CAN	23

/*
下面是具体的调用动态库的做法，采用的是动态加载的方法，主要就是在程序初始化的
时候从动态库中取得各个函数的地址并保存起来，然后在需要的时候调用就可以了，最后在
程序退出的时候释放掉打开的动态库句柄就行。
*/

//首先定义需要用到的数据结构
//1.ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
		USHORT	hw_Version;
		USHORT	fw_Version;
		USHORT	dr_Version;
		USHORT	in_Version;
		USHORT	irq_Num;
		BYTE	can_Num;
		CHAR	str_Serial_Num[20];
		CHAR	str_hw_Type[40];
		USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;
	BYTE	TimeFlag;
	BYTE	SendType;
	BYTE	RemoteFlag;//是否是远程帧
	BYTE	ExternFlag;//是否是扩展帧
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _ERR_INFO{
		UINT	ErrCode;
		BYTE	Passive_ErrData[3];
		BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//接下来定义要导入的函数类型
//根据ControlCAN.h中的函数声明定义函数指针类型
//////////////////////////////////////////////////////////////////////////
typedef DWORD (CALLBACK*  LPVCI_OpenDevice)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_CloseDevice)(DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_InitCan)(DWORD,DWORD,DWORD,PVCI_INIT_CONFIG);

typedef DWORD (CALLBACK*  LPVCI_ReadBoardInfo)(DWORD,DWORD,PVCI_BOARD_INFO);
typedef DWORD (CALLBACK*  LPVCI_ReadErrInfo)(DWORD,DWORD,DWORD,PVCI_ERR_INFO);
typedef DWORD (CALLBACK*  LPVCI_ReadCanStatus)(DWORD,DWORD,DWORD,PVCI_CAN_STATUS);

typedef DWORD (CALLBACK*  LPVCI_GetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);
typedef DWORD (CALLBACK*  LPVCI_SetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);

typedef ULONG (CALLBACK*  LPVCI_GetReceiveNum)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_ClearBuffer)(DWORD,DWORD,DWORD);

typedef DWORD (CALLBACK*  LPVCI_StartCAN)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_ResetCAN)(DWORD,DWORD,DWORD);

typedef ULONG (CALLBACK*  LPVCI_Transmit)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ,ULONG);
typedef ULONG (CALLBACK*  LPVCI_Receive)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ,ULONG,INT);
//////////////////////////////////////////////////////////////////////////

HANDLE m_hDLL;//用来保存打开的动态库句柄

//接口函数指针
LPVCI_OpenDevice VCI_OpenDevice;
LPVCI_CloseDevice VCI_CloseDevice;
LPVCI_InitCan VCI_InitCAN;
LPVCI_ReadBoardInfo VCI_ReadBoardInfo;
LPVCI_ReadErrInfo VCI_ReadErrInfo;
LPVCI_ReadCanStatus VCI_ReadCanStatus;
LPVCI_GetReference VCI_GetReference;
LPVCI_SetReference VCI_SetReference;
LPVCI_GetReceiveNum VCI_GetReceiveNum;
LPVCI_ClearBuffer VCI_ClearBuffer;
LPVCI_StartCAN VCI_StartCAN;
LPVCI_ResetCAN VCI_ResetCAN;
LPVCI_Transmit VCI_Transmit;
LPVCI_Receive VCI_Receive;


////////////////////////////////////////////////////////////////////////////
DWORD m_devtype=4;//USBCAN2类型号
DWORD m_devind=0;
DWORD m_cannum=0;
int m_connect=0;

int m_arrdevtype[20];

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        m_hDLL = LoadLibrary("ControlCAN.dll");//打开动态库

        //取得函数地址
        VCI_OpenDevice=(LPVCI_OpenDevice)GetProcAddress(m_hDLL,"VCI_OpenDevice");
 	VCI_CloseDevice=(LPVCI_CloseDevice)GetProcAddress(m_hDLL,"VCI_CloseDevice");
   	VCI_InitCAN=(LPVCI_InitCan)GetProcAddress(m_hDLL,"VCI_InitCAN");
   	VCI_ReadBoardInfo=(LPVCI_ReadBoardInfo)GetProcAddress(m_hDLL,"VCI_ReadBoardInfo");
    	VCI_ReadErrInfo=(LPVCI_ReadErrInfo)GetProcAddress(m_hDLL,"VCI_ReadErrInfo");
    	VCI_ReadCanStatus=(LPVCI_ReadCanStatus)GetProcAddress(m_hDLL,"VCI_ReadCANStatus");
  	VCI_GetReference=(LPVCI_GetReference)GetProcAddress(m_hDLL,"VCI_GetReference");
    	VCI_SetReference=(LPVCI_SetReference)GetProcAddress(m_hDLL,"VCI_SetReference");
  	VCI_GetReceiveNum=(LPVCI_GetReceiveNum)GetProcAddress(m_hDLL,"VCI_GetReceiveNum");
 	VCI_ClearBuffer=(LPVCI_ClearBuffer)GetProcAddress(m_hDLL,"VCI_ClearBuffer");
 	VCI_StartCAN=(LPVCI_StartCAN)GetProcAddress(m_hDLL,"VCI_StartCAN");
  	VCI_ResetCAN=(LPVCI_ResetCAN)GetProcAddress(m_hDLL,"VCI_ResetCAN");
    	VCI_Transmit=(LPVCI_Transmit)GetProcAddress(m_hDLL,"VCI_Transmit");
    	VCI_Receive=(LPVCI_Receive)GetProcAddress(m_hDLL,"VCI_Receive");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        if(m_connect==1)
        {
                m_connect=0;
                WaitForSingleObject(m_readhandle,2000);
                m_readhandle=NULL;
                VCI_CloseDevice(m_devtype,m_devind);
        }

        FreeLibrary(m_hDLL);//释放动态库句柄
}
//---------------------------------------------------------------------------




void ReceiveThread(void *param)
{
   TListBox *box=(TListBox*)param;
   VCI_CAN_OBJ receivedata[50];
   VCI_ERR_INFO errinfo;
   int len,i;
   AnsiString str,tmpstr;
   while(1)
   {
        if(m_connect==0)
                break;
        Sleep(1);
        len=VCI_Receive(m_devtype,m_devind,m_cannum,receivedata,50,200);
        if(len<=0)
        {
                //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
                //千万不能省略这一步（即使你可能不想知道错误码是什么）
                VCI_ReadErrInfo(m_devtype,m_devind,m_cannum,&errinfo);
        }
        else
        {
	        for(i=0;i<len;i++)
		{
			str="接收到数据帧:  ";
			if(receivedata[i].TimeFlag==0)
				tmpstr="时间标识:无  ";
			else
				tmpstr="时间标识:0x"+IntToHex((int)receivedata[i].TimeStamp,8)+" ";
			str+=tmpstr;
			tmpstr="帧ID:0x"+IntToHex((int)receivedata[i].ID,8)+" ";
			str+=tmpstr;
			str+="帧格式:";
			if(receivedata[i].RemoteFlag==0)
				tmpstr="数据帧 ";
			else
				tmpstr="远程帧 ";
			str+=tmpstr;
			str+="帧类型:";
			if(receivedata[i].ExternFlag==0)
				tmpstr="标准帧 ";
			else
				tmpstr="扩展帧 ";
			str+=tmpstr;
			box->Items->Add(str);
			if(receivedata[i].RemoteFlag==0)
			{
				str="数据:";
                                if(receivedata[i].DataLen>8)
                                        receivedata[i].DataLen=8;
				for(int j=0;j<receivedata[i].DataLen;j++)
				{
					tmpstr=IntToHex((int)receivedata[i].Data[j],2)+" ";
					str+=tmpstr;
				}
				box->Items->Add(str);
			}
		}
		box->ItemIndex=box->Items->Count-1;
        }
   }

   _endthread();
}


void __fastcall TForm1::Button1Click(TObject *Sender)
{
        if(m_connect==1)
        {
                Button1->Caption ="连接";
                m_connect=0;
                WaitForSingleObject(m_readhandle,2000);
                m_readhandle=NULL;
                VCI_CloseDevice(m_devtype,m_devind);
                return;
        }
         m_devtype = m_arrdevtype[ ComboBox_devtype->ItemIndex];
        int index = ComboBox1->ItemIndex;
        int cannum = ComboBox2->ItemIndex;
        VCI_INIT_CONFIG initconfig;
        initconfig.AccCode=StrToInt("0x"+Edit2->Text);
        initconfig.AccMask=StrToInt("0x"+Edit3->Text);
        initconfig.Timing0=StrToInt("0x"+Edit5->Text);
        initconfig.Timing1=StrToInt("0x"+Edit6->Text);
        initconfig.Filter=ComboBox3->ItemIndex;
        initconfig.Mode=ComboBox4->ItemIndex;

        if(index>=0&&cannum>=0)
        {
                if(VCI_OpenDevice(m_devtype,index,0)==1)
                {
                        if(VCI_InitCAN(m_devtype,index,cannum,&initconfig)==1)
                        {
                                Button1->Caption ="断开";
                                m_connect=1;
                                m_devind=index;
                                m_cannum=cannum;
                                m_readhandle=(HANDLE)_beginthread(ReceiveThread,0,(void*)ListBox1);
                        }
                        else
                        {
                                ShowMessage("初始化CAN错误");
                        }
                }
                else
                {
                        ShowMessage("打开端口错误");
                }

        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
        if(m_connect==0)
        {
                ShowMessage("请先打开端口");
                return;
        }
        if(VCI_ResetCAN(m_devtype,m_devind,m_cannum)==1)
        {
                ListBox1->Items->Add("复位CAN成功");
        }
        else
        {
                ListBox1->Items->Add("复位CAN失败");
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
        if(m_connect==0)
        {
                ShowMessage("请先打开端口");
                return;
        }
        if(VCI_StartCAN(m_devtype,m_devind,m_cannum)==1)
        {
                ListBox1->Items->Add("启动CAN成功");
        }
        else
        {
                ListBox1->Items->Add("启动CAN失败");
        }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button6Click(TObject *Sender)
{
        if(m_connect==0)
        {
                ShowMessage("请先打开端口");
                return;
        }
        DWORD i= ComboBox3->ItemIndex;
        if(VCI_SetReference(m_devtype,m_devind,0,1,(PVOID)&i)==1)
        {
                ListBox1->Items->Add("更改成功");
        }
        else
        {
                ListBox1->Items->Add("更改失败");
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
      ComboBox1->ItemIndex = 0;
      ComboBox2->ItemIndex = 0;
      ComboBox3->ItemIndex = 0;
      ComboBox4->ItemIndex = 0;
      ComboBox5->ItemIndex = 2;
      ComboBox6->ItemIndex =0;
      ComboBox7->ItemIndex =0;

      //
      //ComboBox_devtype->Items("VCI_PCI5121");
      int index=0;
      ComboBox_devtype->Items->Clear();

      index= ComboBox_devtype->Items-> Add("VCI_PCI5121");
      m_arrdevtype[index]=VCI_PCI5121;

      index= ComboBox_devtype->Items-> Add("VCI_PCI9810");
      m_arrdevtype[index]=VCI_PCI9810;

      index= ComboBox_devtype->Items-> Add("VCI_USBCAN1(I+)");
      m_arrdevtype[index]=VCI_USBCAN1;

      index= ComboBox_devtype->Items-> Add("VCI_USBCAN2(II+)");
      m_arrdevtype[index]=VCI_USBCAN2;

      index= ComboBox_devtype->Items-> Add("VCI_USBCAN2A");
      m_arrdevtype[index]=VCI_USBCAN2A;

      index= ComboBox_devtype->Items-> Add("VCI_PCI9820");
      m_arrdevtype[index]=VCI_PCI9820;

      index= ComboBox_devtype->Items-> Add("VCI_PCI5110");
      m_arrdevtype[index]=VCI_PCI5110;

      index= ComboBox_devtype->Items-> Add("VCI_CANLITE");
      m_arrdevtype[index]=VCI_CANLITE;

      index= ComboBox_devtype->Items-> Add("VCI_ISA9620");
      m_arrdevtype[index]=VCI_ISA9620;

      index= ComboBox_devtype->Items-> Add("VCI_ISA5420");
      m_arrdevtype[index]=VCI_ISA5420;

      index= ComboBox_devtype->Items-> Add("VCI_PC104CAN");
      m_arrdevtype[index]=VCI_PC104CAN;

      index= ComboBox_devtype->Items-> Add("VCI_DNP9810");
      m_arrdevtype[index]=VCI_DNP9810;

      index= ComboBox_devtype->Items-> Add("VCI_PCI9840");
      m_arrdevtype[index]=VCI_PCI9840;

      index= ComboBox_devtype->Items-> Add("VCI_PC104CAN2");
      m_arrdevtype[index]=VCI_PC104CAN2;

      index= ComboBox_devtype->Items-> Add("VCI_PCI9820I");
      m_arrdevtype[index]=VCI_PCI9820I;

      index= ComboBox_devtype->Items-> Add("VCI_PEC9920");
      m_arrdevtype[index]=VCI_PEC9920;

      ComboBox_devtype->ItemIndex=3;
      ComboBox_devtype->DropDownCount = ComboBox_devtype->Items-> Count;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
        if(m_connect==0)
        {
                ShowMessage("请先打开端口");
                return;
        }

        BYTE sendtype,frametype,frameformat;
        DWORD id;
        BYTE data[8];

        sendtype=ComboBox5->ItemIndex;
        frametype=ComboBox6->ItemIndex;
        frameformat=ComboBox7->ItemIndex;
        id=StrToInt("0x"+Edit1->Text);

        AnsiString str=Edit4->Text;
        AnsiString strdata;
        int i,kkk;
        for(i=0;i<8;i++)
        {
                strdata=str.SubString(3*i+1,2);
                strdata=strdata.Trim();
                kkk=strdata.Length();
                if(kkk==0)
                {
                        goto exit;
                }
                data[i]=StrToInt(strdata);
                //sscanf(strdata.c_str(),"%x",data+i);
        }

exit:
        VCI_CAN_OBJ senddata;
        senddata.SendType=sendtype;
        senddata.ExternFlag=frametype;
        senddata.RemoteFlag=frameformat;
        senddata.ID=id;
        senddata.DataLen=i;
        memcpy(senddata.Data,data,i);

        if(VCI_Transmit(m_devtype,m_devind,m_cannum,&senddata,1)==1)
        {
                ListBox1->Items->Add("发送成功");
        }
        else
        {
                ListBox1->Items->Add("发送失败");
        }

}


//---------------------------------------------------------------------------

