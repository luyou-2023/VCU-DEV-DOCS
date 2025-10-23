unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs,  StdCtrls, ControlCAN;

type
  TForm1 = class(TForm)
    ComboBox1: TComboBox;
    ComboBox2: TComboBox;
    Button1: TButton;
    Button2: TButton;
    GroupBox1: TGroupBox;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Button5: TButton;
    GroupBox2: TGroupBox;
    Label8: TLabel;
    Label9: TLabel;
    Button6: TButton;
    ComboBox5: TComboBox;
    ComboBox6: TComboBox;
    ComboBox7: TComboBox;
    Edit1: TEdit;
    Edit4: TEdit;
    GroupBox6: TGroupBox;
    ListBox1: TListBox;
    GroupBox3: TGroupBox;
    Label1: TLabel;
    Edit2: TEdit;
    Edit3: TEdit;
    Label2: TLabel;
    Edit5: TEdit;
    Label10: TLabel;
    Edit6: TEdit;
    Label11: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    ComboBox3: TComboBox;
    ComboBox4: TComboBox;
    Label14: TLabel;
    ComboBox_devtype: TComboBox;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button6Click(Sender: TObject);

    procedure FormClose(Sender: TObject; var Action: TCloseAction);

  private
    { Private declarations }
  public

  end;
  
PTListBox=^TListBox;
var
  Form1: TForm1;
var
    m_devtype : DWORD;
    m_devind : DWORD;
    m_cannum : DWORD;
    m_connect : DWORD;
    m_threadhandle : integer;

    m_arrdevtype:array[0..20] of integer;


implementation

{$R *.dfm}
function ReceiveThread(param : Pointer): integer;
var
receivedata : array[0..49] of VCI_CAN_OBJ;
len : integer;
j : integer;
i : integer;
str : AnsiString;
tmpstr :AnsiString;
box : PTListBox;
errinfo : VCI_ERR_INFO;
begin
  box:=param;
   while TRUE do
    begin
      if m_connect=0 then
        break;
      Sleep(1);
      len:=VCI_Receive(m_devtype,m_devind,m_cannum,@receivedata[0],50,200);
      if len<=0 then
        begin
          //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
    			//千万不能省略这一步（即使你可能不想知道错误码是什么）
		    	VCI_ReadErrInfo(m_devtype,m_devind,m_cannum,@errinfo);
          continue;
        end;
      for i:=0 to len-1 do
        begin
	    		str:='接收到数据帧:  ';
    			if receivedata[i].TimeFlag=0 then
		  		  tmpstr:='时间标识:无  '
    			else
    				tmpstr:='时间标识:0x'+IntToHex(receivedata[i].TimeStamp,8)+' ';
			    str:=str+tmpstr;
			    tmpstr:='帧ID:0x'+IntToHex(receivedata[i].ID,8)+' ';
			    str:=str+tmpstr;
			    str:=str+'帧格式:';
			    if receivedata[i].RemoteFlag=0 then
				    tmpstr:='数据帧 '
			    else
				    tmpstr:='远程帧 ';
    			str:=str+tmpstr;
		      str:=str+'帧类型:';
			    if receivedata[i].ExternFlag=0 then
    				tmpstr:='标准帧 '
    			else
		    		tmpstr:='扩展帧 ';
    			str:=str+tmpstr;
    			box.Items.Add(str);
	    		if receivedata[i].RemoteFlag=0 then
          begin
    				str:='数据:';
            if receivedata[i].DataLen>8 then
              receivedata[i].DataLen:=8;
    				for j:=0 to receivedata[i].DataLen-1 do
              begin
      					tmpstr:=IntToHex(receivedata[i].Data[j],2)+' ';
	      				str:=str+tmpstr;
              end;
   				box.Items.Add(str);
          end;
        end;
	  	box.ItemIndex:=box.Items.Count-1;
    end;

  EndThread(0);
  ReceiveThread:=0;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
index: integer;
cannum: integer;
threadid: LongWord;
initconfig : VCI_INIT_CONFIG;
begin


if m_connect=1 then
  begin
    m_connect:=0;
    Button1.Caption:='连接';
    VCI_CloseDevice(m_devtype,m_devind);
    Exit;
  end;

  index:=ComboBox1.ItemIndex;
cannum:=ComboBox2.ItemIndex;

m_devtype := m_arrdevtype[ComboBox_devtype. ItemIndex];

if (index>=0) and (cannum>=0)  then
  begin
    if VCI_OpenDevice(m_devtype,index,0)<>1 then
      ShowMessage('端口打开失败')
    else
      begin
        initconfig.AccCode:=StrToInt('0x'+Edit2.Text);
        initconfig.AccMask:=StrToInt('0x'+Edit3.Text);
        initconfig.Timing0:=StrToInt('0x'+Edit5.Text);
        initconfig.Timing1:=StrToInt('0x'+Edit6.Text);
        initconfig.Filter:=ComboBox3.ItemIndex;
        initconfig.Mode:=ComboBox4.ItemIndex;
        if VCI_InitCAN(m_devtype,m_devind,cannum,@initconfig)<>1 then
          begin
            Showmessage('初始化CAN失败');
            Exit;
          end;
        m_devind:=index;
        m_cannum:=cannum;
        m_connect:=1;
        Button1.Caption:='断开';
        threadid:=111;
        m_threadhandle:=BeginThread(0,0,ReceiveThread,@ListBox1,0,threadid);
      end;
  end
else
  ShowMessage('选项不能为空');
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  if m_connect=0 then
    Exit;
  if VCI_ResetCAN(m_devtype,m_devind,m_cannum)<>1 then
    ShowMessage('复位CAN失败')
  else
    ListBox1.Items.Add('复位CAN成功');

end;

procedure TForm1.FormCreate(Sender: TObject);
var
index: integer;
begin
  m_devtype := 4;//USBCAN2类型号
  m_devind := 0;
  m_cannum :=0;
  m_connect:= 0;
  ComboBox1.ItemIndex:=0;
  ComboBox2.ItemIndex:=0;
  ComboBox3.ItemIndex:=0;
  ComboBox4.ItemIndex:=0;
  ComboBox5.ItemIndex:=2;
  ComboBox6.ItemIndex:=0;
  ComboBox7.ItemIndex:=0;

  //ComboBox
  index:=0;
     ComboBox_devtype.Items.Clear();
  index := ComboBox_devtype.Items.add( 'VCI_PCI5121');
   m_arrdevtype[index] :=  VCI_PCI5121;

    index := ComboBox_devtype.Items.add( 'VCI_PCI9810');
   m_arrdevtype[index] :=  VCI_PCI9810;

    index := ComboBox_devtype.Items.add( 'VCI_USBCAN1(I+)');
   m_arrdevtype[index] :=  VCI_USBCAN1;

    index := ComboBox_devtype.Items.add( 'VCI_USBCAN2(II+)');
   m_arrdevtype[index] :=  VCI_USBCAN2;

    index := ComboBox_devtype.Items.add( 'VCI_USBCAN2A');
   m_arrdevtype[index] :=  VCI_USBCAN2A;

    index := ComboBox_devtype.Items.add( 'VCI_PCI9820');
   m_arrdevtype[index] :=  VCI_PCI9820;

    index := ComboBox_devtype.Items.add( 'VCI_PCI5110');
   m_arrdevtype[index] :=  VCI_PCI5110;

    index := ComboBox_devtype.Items.add( 'VCI_ISA9620');
   m_arrdevtype[index] :=  VCI_ISA9620;

    index := ComboBox_devtype.Items.add( 'VCI_ISA5420');
   m_arrdevtype[index] :=  VCI_ISA5420;

    index := ComboBox_devtype.Items.add( 'VCI_PC104CAN');
   m_arrdevtype[index] :=  VCI_PC104CAN;

    index := ComboBox_devtype.Items.add( 'VCI_DNP9810');
   m_arrdevtype[index] :=  VCI_DNP9810;

    index := ComboBox_devtype.Items.add( 'VCI_PCI9840');
   m_arrdevtype[index] :=  VCI_PCI9840;

    index := ComboBox_devtype.Items.add( 'VCI_PC104CAN2');
   m_arrdevtype[index] :=  VCI_PC104CAN2;

    index := ComboBox_devtype.Items.add( 'VCI_PCI9820I');
   m_arrdevtype[index] :=  VCI_PCI9820I;

    index := ComboBox_devtype.Items.add( 'VCI_PEC9920');
   m_arrdevtype[index] :=  VCI_PEC9920;

    index := ComboBox_devtype.Items.add( 'VCI_CANLITE');
   m_arrdevtype[index] :=  VCI_CANLITE;


  ComboBox_devtype.ItemIndex:=3;

end;

procedure TForm1.Button5Click(Sender: TObject);
var
sendtype,frametype,frameformat : BYTE;
id : DWORD;
data : array[0..7] of BYTE;
str : AnsiString;
strdata : AnsiString;
senddata : VCI_CAN_OBJ;
i : integer;
begin
  if m_connect=0 then
    Exit;
  sendtype:=ComboBox5.ItemIndex;
  frametype:=ComboBox6.ItemIndex;
  frameformat:=ComboBox7.ItemIndex;
  id:=StrToInt('0x'+Edit1.Text);
  str:=Edit4.Text;
  for i:=0 to 7 do
    begin
      strdata:=Copy(str,3*i+1,2);
      strdata:=Trim(strdata);
      if Length(strdata)=0 then
        break;
      data[i]:=StrToInt('0x'+strdata);
    end;
  senddata.SendType:=sendtype;
  senddata.ExternFlag:=frametype;
  senddata.RemoteFlag:=frameformat;
  senddata.ID:=id;
  senddata.DataLen:=i;
  Move(data,senddata.Data,i);
  if VCI_Transmit(m_devtype,m_devind,m_cannum,@senddata,1)=1 then
    ListBox1.Items.Add('发送成功')
  else
    ListBox1.Items.Add('发送失败');
end;

procedure TForm1.Button6Click(Sender: TObject);
begin
  if m_connect=0 then
    Exit;
  if VCI_StartCAN(m_devtype,m_devind,m_cannum)<>1 then
    ShowMessage('启动CAN失败')
  else
    ListBox1.Items.Add('启动CAN成功');

end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  if m_connect=1 then
  begin
    m_connect:=0;
    WaitForSingleObject(m_threadhandle,2000);
    m_threadhandle:=0;
    VCI_CloseDevice(m_devtype,m_devind);
  end
end;

end.
