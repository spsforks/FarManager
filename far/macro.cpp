/*
macro.cpp

������

*/

/* Revision: 1.31 29.04.2001 $ */

/*
Modify:
  29.04.2001 ��
    + ����७�� NWZ �� ����类��
  25.04.2001 SVS
    + MFLAGS_SELECTION - 䫠� �஢�ન �뤥�����:
      ��� ������� - ����� ������ �뤥������� ��ꥪ�,
      ��� ।���� - �� �����.
    ! ��� �஢�ન 䫠��� ��� ���� ����ᮢ �뭥ᥭ � �㭪樨 Check* -
      ᫨誮� ����� �������饣��� ���� :-(
    ! ���� ������ ����ன�� ����� - �ᯮ������� 3-� ����樮��� 祪�����,
      �� ��������� "㦠��" ������ � ࠧ����.
  05.04.2001 VVM
    + 3 �������⥫��� ������ ����ᮢ - "Info", "QView", "Tree"
  21.03.2001 SVS
    + ��ࠡ�⪠ �ᮡ�� ������: F1 & Enter
  11.03.2001 SVS
    ! �������� MFLAGS_RUNAFTERSTART �������� �� MFLAGS_RUNAFTERFARSTART
    + �㭪�� MkTextSequence - �ନ஢���� ��ப����� �।�⠢����� Sequence
    + �஢�ઠ �� "������� ⮦� ᠬ��"
    ! ������� ������ - ����﫨 �� ���� ��ப� ����� :-)
    ! �������� ��⨬����� ����.
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  22.02.2001 SVS
    + MFLAGS_DISABLEMACRO - ���� ����� ����ᠡ���!
    ! ��⥬, �� ᨬ��� '~' � ��砫� �������� ����� - �� ����ᠡ�����
      �����
    + � �������� �� 㤠�����, ��७����祭�� ��������� ��⪠� ��� ��
      ⥪�騩 Sequence
  19.02.2001 SVS
    - ���࠭�� ������� ��ࠬ��஢ ����� (�� ���������� �������)
  30.01.2001 SVS
    - ���� ᤥ���� �஢��� �� ��� ������ �� ������� �����祭��
  21.01.2001 SVS
    - �訡�� � �����祭�� ������ - ���뢠��� ⥪�騩 ���⥪�� �����, �
      �� ���⮢� �� ����砭�� ����� �����.
  19.01.2001 SVS
    + ��१�ࢨ஢���: MFLAGS_REUSEMACRO - ����୮� �ᯮ�짮����� ����ᮢ
      �� ���� �����, ����� �㤥� ��堭��� ����ਧ�樨...
      � ���� ����� �㤥� :-)
  18.01.2001 SVS
    ! ������� ��⨬���樨 ����
    + �㭪樨 ���᪠ ���ப����� � ��।������ ࠧ��� �⮩ ������
    + ��⥫�� - �뤠���� �।�०�����, �� ⠪�� ������ �������
      ��� ���⢥ত���� ������� 㤠���� ���ப������.
  09.01.2001 SVS
    - ���� � �ய������� ������� �����祭�� �� �६� ��������� 䠩�����
      �������� � �������.
  09.01.2001 SVS
    + ��⥬ �ࠢ��� Opt.ShiftsKeyRules (WaitInFastFind)
  04.01.2001 SVS
    ! �����⭠� ��।���� �������� ����᭥�쪨� �㭪権 :-)
    ! ���� ������ �����祭�� ������
    ! �������⥫�� ���� - �� ���
    + �᪫�祭�� "NoFolders" � "NoFiles"
  28.12.2000 SVS
    - ���� � �᪫�祭�ﬨ �� ������.
  26.12.2000 SVS
    + KeyMacroToText()
    ! ������㥬 �� END_FARKEY_BASE
    + ��ࠡ�⪠ ᯥ�-���ப�����.
  25.12.2000 SVS
    ! MFLAGS_ ���㫨�� �� plugin.hpp
  23.12.2000 SVS
    ! MFLAGS_ ��॥堫� � plugin.hpp
    + int KeyMacro::ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr)
    + int KeyMacro::PlayKeyMacro(struct MacroRecord *MRec)
    + int KeyMacro::PlayKeyMacro(char *KeyBuffer)
  22.12.2000 SVS
    - �⢫���� - ���� ����� ���� 2 䫠�� :-(
  22.12.2000 SVS
    - �� ���ࠢ��쭮 ��࠭��� ��������� ��稭����� �㬠�� :-(
      ��᫥ 333 ���� ����⠫� ࠡ���� ������ �����!
  21.12.2000 SVS
    ! 3-� ���ﭨ� ��� ⨯� �������.
    + LoadMacros(), InitVars(), ReleaseTempBuffer()
    ! ReadMacros - �����頥� TRUE ��� FALSE (�� ������ �� �뤥����� �����)
    + TempMacroType, TempMacro - ���� �ᯮ�짮������ ��� ������
      MCMD_PLAYRECORD, MCMD_PLAYSTRING.
  21.12.2000 SVS
    - ����୮ ࠡ�⠫� ���뢠��� ��ࠬ��஢ �� ���� ���砬
      FilePanels � PluginPanels
  21.12.2000 SVS
    + "㡨ࠥ� ���� �����  (LockScr)
    ! �㭪�� KeyToText 㤠���� �� �������������
    ! ������� MacroRecord "ᦠ�"
    + 2 ०��� ࠡ��� ����ᮢ � �������:
       MFLAGS_PLUGINPANEL - ࠡ�⠥� �� ������ ��������
       MFLAGS_FILEPANEL - ࠡ�⠥� �� 䠩����� ������
      �� 㬮�砭�� ��� ����祭�.
  27.09.2000 SKV
    - Don't redraw editor after macro finished if it is hidden.
  10.09.2000 SVS
    ! ��ࠢ�� ����� � ����ᠬ� � �裡 � ���ࠡ�⠭묨 ������ ������
    ! �㭪�� ReadMacros ����� �������⥫�� ��㬥���
  10.08.2000 skv
    ! ��������� � GetKey ��� ��ࠡ�⪨ ����砭�� ���� � �����.
  25.07.2000 SVS
    ! �㭪�� KeyToText ᤥ���� ᠬ����⥫쭮� - ��諠 � ��⠢
      FSF
  23.07.2000 SVS
    + ������:
       Ctrl- Shift- Alt- CtrlShift- AltShift- CtrlAlt- Apps :-)
       KEY_LWIN (VK_LWIN), KEY_RWIN (VK_RWIN)
  13.07.2000 SVS
    ! ������� ���४樨 �� �ᯮ�짮����� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ���������� �������樨 ��� BC & VC
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

#define MFLAGS_MODEMASK            0x0000FFFF
#define MFLAGS_DISABLEOUTPUT       0x00010000
#define MFLAGS_RUNAFTERFARSTART    0x00020000
#define MFLAGS_EMPTYCOMMANDLINE    0x00040000
#define MFLAGS_NOTEMPTYCOMMANDLINE 0x00080000
#define MFLAGS_NOFILEPANELS        0x00100000
#define MFLAGS_NOPLUGINPANELS      0x00200000
#define MFLAGS_NOFOLDERS           0x00400000
#define MFLAGS_NOFILES             0x00800000
#define MFLAGS_REUSEMACRO          0x01000000
#define MFLAGS_SELECTION           0x02000000
#define MFLAGS_NOSELECTION         0x04000000
#define MFLAGS_DISABLEMACRO        0x80000000


static const char *MacroModeName[]={
  "Shell", "Viewer", "Editor", "Dialog", "Search",
  "Disks", "MainMenu", "Help",
  "Info", "QView", "Tree"
};
static const char *MacroModeNameOther="Other";

static struct TMacroFlagsName {
  char *Name;
  DWORD Flag;
} MacroFlagsName[]={
  {"DisableOutput",       MFLAGS_DISABLEOUTPUT},
  {"RunAfterFARStart",    MFLAGS_RUNAFTERFARSTART},
  {"EmptyCommandLine",    MFLAGS_EMPTYCOMMANDLINE},
  {"NotEmptyCommandLine", MFLAGS_NOTEMPTYCOMMANDLINE},
  {"NoFilePanels",        MFLAGS_NOFILEPANELS},
  {"NoPluginPanels",      MFLAGS_NOPLUGINPANELS},
  {"NoFolders",           MFLAGS_NOFOLDERS},
  {"NoFiles",             MFLAGS_NOFILES},
  {"ReuseMacro",          MFLAGS_REUSEMACRO},
  {"Selection",           MFLAGS_SELECTION},
  {"NoSelection",         MFLAGS_NOSELECTION},
};

// ⨯ �६������ ����
enum MacroTempType{
  MTEMP_POINTER,  // ��।��� ��㤠 �
  MTEMP_DYNAMIC,  // �ᯮ�짮������ �뤥����� �����
};

// ��� ������� �����祭�� ������
struct DlgParam{
  KeyMacro *Handle;
  DWORD Key;
  int Mode;
};

// ���� �����������
enum{
  KEY_MACROSTOP=KEY_MACROSPEC_BASE,
  KEY_MACROMODE,
};

// �࠭᫨����� ⠡��� - ��� <-> ��� ���ப�����
static struct TKeyCodeName{
  int Key;
  int Len;
  char *Name;
} KeyMacroCodes[]={
   { KEY_MACRODAY,                           4, "$DAY"   },
   { KEY_MACROMONTH,                         6, "$MONTH" },
   { KEY_MACROYEAR,                          5, "$YEAR"  },
   { KEY_MACROHOUR,                          5, "$HOUR"  },
   { KEY_MACROMIN,                           4, "$MIN"   },
   { KEY_MACROSEC,                           4, "$SEC"   },
   { KEY_MACROSTOP,                          5, "$STOP"  },
   { KEY_MACROMODE,                          6, "$MMODE" },
};

// �㭪�� �८�ࠧ������ ���� ���ப����� � ⥪��
BOOL WINAPI KeyMacroToText(int Key,char *KeyText0,int Size)
{
  if(!KeyText0)
     return FALSE;

  char KeyText[32]="";

  for (int I=0;I<sizeof(KeyMacroCodes)/sizeof(KeyMacroCodes[0]);I++)
    if (Key==KeyMacroCodes[I].Key)
    {
      strcpy(KeyText,KeyMacroCodes[I].Name);
      break;
    }

  if(!KeyText[0])
  {
    *KeyText0='\0';
    return FALSE;
  }
  if(Size > 0)
    strncpy(KeyText0,KeyText,Size);
  else
    strcpy(KeyText0,KeyText);

  return TRUE;
}

KeyMacro::KeyMacro()
{
  TempMacroType=MTEMP_POINTER;
  TempMacro=NULL;
  LockScr=NULL;
  Macros=NULL;
  RecBuffer=NULL;
  LoadMacros();
}

KeyMacro::~KeyMacro()
{
  InitVars();
}

// ���樠������ ��� ��६�����
void KeyMacro::InitVars()
{
  if(Macros)
  {
    for (int I=0;I<MacrosNumber;I++)
      if(Macros[I].Buffer)
        free(Macros[I].Buffer);
    free(Macros);
  }
  if(RecBuffer) delete[] RecBuffer;

  if(LockScr)
  {
    delete LockScr;
    LockScr=NULL;
  }

  ReleaseTempBuffer();

  MacrosNumber=0;
  StartMacroPos=0;
  Recording=FALSE;
  Executing=FALSE;
  Macros=NULL;
  RecBuffer=NULL;
  RecBufferSize=0;
  InternalInput=FALSE;
}

// 㤠����� �६������ ����, �᫨ �� ᮧ������� �������᪨
// (�������᪨ - ����� � PlayMacros ��।��� ��ப�.
void KeyMacro::ReleaseTempBuffer()
{
  if(TempMacroType == MTEMP_DYNAMIC && TempMacro)
  {
    if(TempMacro->Buffer)
      free(TempMacro->Buffer);
    free(TempMacro);
  }
  TempMacro=NULL;
  TempMacroType=MTEMP_POINTER;
}

// ����㧪� ���� ����ᮢ �� ॥���
int KeyMacro::LoadMacros()
{
  int Ret=FALSE;
  InitVars();

  #define TEMP_BUFFER_SIZE 32768
  char *Buffer=new char[TEMP_BUFFER_SIZE];

  if(Buffer)
  {
    Ret+=ReadMacros(MACRO_SHELL,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_VIEWER,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_EDITOR,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_DIALOG,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_SEARCH,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_DISKS,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_MAINMENU,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_HELP,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_INFOPANEL,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_QVIEWPANEL,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_TREEPANEL,Buffer,TEMP_BUFFER_SIZE);
    Ret+=ReadMacros(MACRO_OTHER,Buffer,TEMP_BUFFER_SIZE);

    // ���⠢�� ��� ������ - �᫨ �� �� ��� ����㧨����, �
    // �㤥� FALSE
    Ret=(Ret < 12)?FALSE:TRUE;
    delete Buffer;
  }
  Mode=MACRO_SHELL;
  return Ret;
}

// �㭪�� �८�ࠧ������ �������� � ��� ���ப�����
// ��୥� -1, �᫨ ��� ����������!
int WINAPI KeyNameMacroToKey(char *Name)
{
  // �ன����� �� �ᥬ ����䨪��ࠬ
  for(int I=0; I < sizeof(KeyMacroCodes)/sizeof(KeyMacroCodes[0]); ++I)
    if(!memicmp(Name,KeyMacroCodes[I].Name,KeyMacroCodes[I].Len))
      return KeyMacroCodes[I].Key;
  return -1;
}

int KeyMacro::ProcessKey(int Key)
{
  if (InternalInput || Key==KEY_IDLE || Key==KEY_NONE)
    return(FALSE);

  if (Recording) // ���� ������?
  {
    if (Key==KEY_CTRLSHIFTDOT || Key==KEY_CTRLDOT) // �ਧ��� ���� �����?
    {
      DWORD MacroKey;
      int WaitInMainLoop0=WaitInMainLoop;
      InternalInput=TRUE;
      WaitInMainLoop=FALSE;
      MacroKey=AssignMacroKey();

      DWORD Flags=MFLAGS_DISABLEOUTPUT;

      // ������� �஢��� �� 㤠�����
      // �᫨ 㤠�塞, � �� �㦭� �뤠���� ������ ����ன��.
      if (MacroKey != KEY_ESC && (Key==KEY_CTRLSHIFTDOT || Recording==2) && RecBufferSize)
      {
        if (!GetMacroSettings(MacroKey,Flags))
          MacroKey=KEY_ESC;
      }
      WaitInMainLoop=WaitInMainLoop0;
      InternalInput=FALSE;

      if (MacroKey==KEY_ESC)
        delete RecBuffer;
      else
      {
        int Pos;
        for (Pos=0;Pos<MacrosNumber;Pos++)
          if (Macros[Pos].Key==MacroKey && (Macros[Pos].Flags&MFLAGS_MODEMASK)==StartMode)
            break;
        if (Pos==MacrosNumber)
        {
          Macros=(struct MacroRecord *)realloc(Macros,sizeof(*Macros)*(MacrosNumber+1));
          if (Macros==NULL)
          {
            MacrosNumber=0;
            WaitInFastFind++;
            return(FALSE);
          }
          MacrosNumber++;
        }
        else
          delete Macros[Pos].Buffer;
        Macros[Pos].Key=MacroKey;
        Macros[Pos].Buffer=(DWORD*)RecBuffer;
        Macros[Pos].BufferSize=RecBufferSize;
        Macros[Pos].Flags=Flags|(StartMode&MFLAGS_MODEMASK);
      }

      Recording=FALSE;
      RecBuffer=NULL;
      RecBufferSize=0;
      if (Opt.AutoSaveSetup)
        SaveMacros();
      ScrBuf.RestoreMacroChar();
      WaitInFastFind++;
      return(TRUE);
    }
    else // ����� ����� �த��������.
    {
      if (Key>=KEY_NONE && Key<=KEY_END_SKEY) // ᯥ樠��� ������ �ப����
        return(FALSE);
      RecBuffer=(int *)realloc(RecBuffer,sizeof(*RecBuffer)*(RecBufferSize+1));
      if (RecBuffer==NULL)
        return(FALSE);
      RecBuffer[RecBufferSize++]=Key;
      return(FALSE);
    }
  }
  else if (Key==KEY_CTRLSHIFTDOT || Key==KEY_CTRLDOT) // ��砫� �����?
  {
    if(LockScr) delete LockScr;
    LockScr=NULL;

    // ��� ��?
    StartMode=(Mode==MACRO_SHELL && !WaitInMainLoop)?MACRO_OTHER:Mode;
    // ⨯ ����� - � �맮��� ������� ����஥� ���...
    Recording=(Key==KEY_CTRLSHIFTDOT) ? 2:1;
    delete RecBuffer;
    RecBuffer=NULL;
    RecBufferSize=0;
    ScrBuf.ResetShadow();
    ScrBuf.Flush();
    WaitInFastFind--;
    return(TRUE);
  }
  else
  {
    if (!Executing) // �� �� �� ०�� �ᯮ������?
    {
      DWORD CurFlags;
      int I=GetIndex(LocalUpper(Key),
                    (Mode==MACRO_SHELL && !WaitInMainLoop) ? MACRO_OTHER:Mode);
      if(I != -1 && !((CurFlags=Macros[I].Flags)&MFLAGS_DISABLEMACRO) && CtrlObject)
      {
//SysLog("KeyMacro: %d (I=%d Key=0x%08X)",__LINE__,I,Key);
        if(!CheckAll(CurFlags))
          return FALSE;

        // ��������� �뢮�?
        if (CurFlags&MFLAGS_DISABLEOUTPUT)
        {
          if(LockScr) delete LockScr;
          LockScr=new LockScreen;
        }
        Executing=TRUE;
        ExecMacroPos=I;
        ExecKeyPos=0;
        return(TRUE);
      }
    }
    return(FALSE);
  }
}

// ������� ��।��� ��� ������ �� �����
int KeyMacro::GetKey()
{
  struct MacroRecord *MR;

  if (InternalInput || !Executing)
    return(FALSE);

initial:
  MR=!TempMacro?Macros+ExecMacroPos:TempMacro;

begin:
  if (ExecKeyPos>=MR->BufferSize || MR->Buffer==NULL)
  {
done:
    /*$ 10.08.2000 skv
      If we are in editor mode, and CurEditor defined,
      we need to call this events.
      EE_REDRAW 2 - to notify that text changed.
      EE_REDRAW 0 - to notify that whole screen updated
      ->Show() to actually update screen.

      This duplication take place since ShowEditor method
      will NOT send this event while screen is locked.
    */
    if(Mode==MACRO_EDITOR)
    {
      if(CtrlObject->Plugins.CurEditor)
      {
        /*$ 27.09.2000 skv
          Don't redraw editor if it is hidden.
        */
        if(CtrlObject->Plugins.CurEditor->IsVisible())
        {

          CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE);
          CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
          CtrlObject->Plugins.CurEditor->Show();
        }
        /* skv$*/
      }
    }
    /* skv$*/
    if(LockScr) delete LockScr;
    LockScr=NULL;
    Executing=FALSE;
    ReleaseTempBuffer();
    return(FALSE);
  }

  DWORD Key=MR->Buffer[ExecKeyPos++];
  switch(Key)
  {
    case KEY_MACROSTOP:
      goto done;

    case KEY_MACROMODE:
      if (ExecKeyPos<MR->BufferSize)
      {
        Key=MR->Buffer[ExecKeyPos++];
        if(Key == '1')
        {
          DWORD Flags=MR->Flags;
          if(Flags&MFLAGS_DISABLEOUTPUT) // �᫨ �� - 㤠���
          {
            if(LockScr) delete LockScr;
            LockScr=NULL;
          }

          SwitchFlags(MR->Flags,MFLAGS_DISABLEOUTPUT);

          if(MR->Flags&MFLAGS_DISABLEOUTPUT) // �᫨ �⠫ - ����稬
          {
            if(LockScr) delete LockScr;
            LockScr=new LockScreen;
          }
        }
        goto begin;
      }
  }
//SysLog("Key=0x%08X ExecMacroPos=%d", Key,ExecMacroPos);
  return(Key);
}

// �஢���� - ���� �� �� ������?
int KeyMacro::PeekKey()
{
  struct MacroRecord *MR=!TempMacro?Macros+ExecMacroPos:TempMacro;
  if (InternalInput || !Executing || ExecKeyPos >= MR->BufferSize)
    return(0);
  int Key=MR->Buffer[ExecKeyPos];
  return(Key);
}

DWORD KeyMacro::SwitchFlags(DWORD& Flags,DWORD Value)
{
  if(Flags&Value) Flags&=~Value;
  else Flags|=Value;
  return Flags;
}


char *KeyMacro::MkRegKeyName(int IdxMacro,char *RegKeyName)
{
  char KeyText[50];
  ::KeyToText(Macros[IdxMacro].Key,KeyText);
  sprintf(RegKeyName,"KeyMacros\\%s\\%s%s",
     GetSubKey(Macros[IdxMacro].Flags&MFLAGS_MODEMASK),
     (Macros[IdxMacro].Flags&MFLAGS_DISABLEMACRO?"~":""),
     KeyText);
  return RegKeyName;
}

// ��᫥ �맮�� �⮩ �㭪樨 �㦭� 㤠���� ������!!!
char *KeyMacro::MkTextSequence(DWORD *Buffer,int BufferSize)
{
  char MacroKeyText[50], *TextBuffer;

  // �뤥�塞 �������� ����让 ��᮪
  if((TextBuffer=(char*)malloc(BufferSize*50+1)) == NULL)
    return NULL;

  TextBuffer[0]=0;

  for (int J=0; J < BufferSize; J++)
    if(KeyToText(Buffer[J],MacroKeyText))
    {
      if(J)
        strcat(TextBuffer," ");
      strcat(TextBuffer,MacroKeyText);
    }

  return TextBuffer;
}

// ���࠭���� ���� ����ᮢ
void KeyMacro::SaveMacros()
{
  char *TextBuffer;
  char RegKeyName[150];
  for (int I=0;I<MacrosNumber;I++)
  {
    MkRegKeyName(I,RegKeyName);

    if (Macros[I].BufferSize==0)
    {
      DeleteRegKey(RegKeyName);
      continue;
    }

    if((TextBuffer=MkTextSequence(Macros[I].Buffer,Macros[I].BufferSize)) == NULL)
      continue;

    SetRegKey(RegKeyName,"Sequence",TextBuffer);

    if(TextBuffer)
      free(TextBuffer);

    // ���᮪�⨬ ����...
    for(int J=0; J < sizeof(MacroFlagsName)/sizeof(MacroFlagsName[0]); ++J)
    {
      if (Macros[I].Flags & MacroFlagsName[J].Flag)
        SetRegKey(RegKeyName,MacroFlagsName[J].Name,1);
      else
        DeleteRegValue(RegKeyName,MacroFlagsName[J].Name);
    }
  }
}


/* $ 10.09.2000 SVS
  ! ��ࠢ�� ����� � ����ᠬ� � �裡 � ���ࠡ�⠭묨 ������ ������
  ! �㭪�� ReadMacros ����� �������⥫�� ��㬥���
*/
int KeyMacro::ReadMacros(int ReadMode,char *Buffer,int BufferSize)
{
  int I, J;

  for (I=0;;I++)
  {
    char RegKeyName[150],KeyText[50];
    char UpKeyName[100];
    DWORD MFlags=0;

    sprintf(UpKeyName,"KeyMacros\\%s",GetSubKey(ReadMode));
    if (!EnumRegKey(UpKeyName,I,RegKeyName,sizeof(RegKeyName)))
      break;
    char *KeyNamePtr=strrchr(RegKeyName,'\\');
    if (KeyNamePtr!=NULL)
    {
      strcpy(KeyText,KeyNamePtr+1);
      // ������! �� �������� �����, ��稭��饥�� �� ᨬ��� ~ - ��
      // �����஢���� �����!!!
      if(*KeyText == '~' && KeyText[1])
      {
        memmove(KeyText,KeyText+1,sizeof(KeyText)-1);
        MFlags|=MFLAGS_DISABLEMACRO;
      }
    }
    else
      *KeyText=0;
    int KeyCode=KeyNameToKey(KeyText);
    if (KeyCode==-1)
      continue;
    Macros=(struct MacroRecord *)realloc(Macros,sizeof(*Macros)*(MacrosNumber+1));
    if (Macros==NULL)
    {
      MacrosNumber=0;
      return FALSE;
    }
    struct MacroRecord *CurMacro=&Macros[MacrosNumber];
    CurMacro->Key=KeyCode;
    CurMacro->Buffer=NULL;
    CurMacro->BufferSize=0;
    CurMacro->Flags=MFlags|(ReadMode&MFLAGS_MODEMASK);
    GetRegKey(RegKeyName,"Sequence",Buffer,"",BufferSize);

    for(J=0; J < sizeof(MacroFlagsName)/sizeof(MacroFlagsName[0]); ++J)
      CurMacro->Flags|=GetRegKey(RegKeyName,MacroFlagsName[J].Name,0)?MacroFlagsName[J].Flag:0;

    if(!ParseMacroString(CurMacro,Buffer))
      return FALSE;
    MacrosNumber++;
  }
  return TRUE;
}
/* SVS $ */

// �㭪��, ����᪠��� ������ �� ���� ����
// �᫨ � ��⠢���� �।�०����� � �������⨬��� �믮�����
// �������� ����ᮢ, � ������ ���!
void KeyMacro::RunStartMacro()
{
  if (StartMacroPos==-1)
    return;

  DWORD CurFlags;
  while (StartMacroPos<MacrosNumber)
  {

    int CurPos=StartMacroPos++;
    if (((CurFlags=Macros[CurPos].Flags)&MFLAGS_MODEMASK)==MACRO_SHELL &&
        Macros[CurPos].BufferSize>0 &&
        // �ᯮ��塞 �� ����ᠡ����� ������
        !(CurFlags&MFLAGS_DISABLEMACRO) &&
        (CurFlags&MFLAGS_RUNAFTERFARSTART) && CtrlObject)
    {
      if(!CheckAll(CurFlags))
        return;

      if (CurFlags&MFLAGS_DISABLEOUTPUT)
      {
        if(LockScr) delete LockScr;
        LockScr=new LockScreen;
      }
      Executing=TRUE;
      ExecMacroPos=CurPos;
      ExecKeyPos=0;
      return;
    }
  }
  StartMacroPos=-1;
}

// ��ࠡ��稪 ����������� ���� �����祭�� ������
long WINAPI KeyMacro::AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  char KeyText[50];
  static int LastKey;
  static struct DlgParam *KMParam=NULL;
  int Index, I;

  if(Msg == DN_INITDIALOG)
  {
    KMParam=(struct DlgParam *)Param2;
    LastKey=0;
  }
  else if(Msg == DM_KEY)
  {
    // <��ࠡ�⪠ �ᮡ�� ������: F1 & Enter>
    // Esc & (Enter � �।��騩 Enter) - �� ��ࠡ��뢠��
    if(Param2 == KEY_ESC ||
       Param2 == KEY_ENTER && LastKey == KEY_ENTER)
      return FALSE;

    // F1 - �ᮡ� ��砩 - �㦭� ���� 2 ࠧ�
    // ���� ࠧ �㤥� �뢥��� 奫�,
    // � ��ன ࠧ - ��ன ࠧ 㦥 �����祭��
    if(Param2 == KEY_F1 && LastKey!=KEY_F1)
    {
      LastKey=KEY_F1;
      return FALSE;
    }

    // �뫮 ��-� 㦥 ����� � Enter`�� ���⢥ত���
    if(Param2 == KEY_ENTER && LastKey && LastKey != KEY_ENTER)
      return FALSE;
    // </��ࠡ�⪠ �ᮡ�� ������: F1 & Enter>

    KeyMacro *MacroDlg=KMParam->Handle;

    KMParam->Key=(DWORD)Param2;
    KeyToText(Param2,KeyText);

    // �᫨ ��� ���� ⠪�� �����...
    if((Index=MacroDlg->GetIndex(Param2,KMParam->Mode)) != -1)
    {
      DWORD DisFlags=MacroDlg->Macros[Index].Flags&MFLAGS_DISABLEMACRO;
      char Buf[256];
      char BufKey[64];
      char RegKeyName[150];
      char *TextBuffer;

      MacroDlg->MkRegKeyName(Index,RegKeyName);
      // ��६ �� �����.
      if((TextBuffer=MacroDlg->MkTextSequence(MacroDlg->Macros[Index].Buffer,
                                  MacroDlg->Macros[Index].BufferSize)) != NULL)
      {
        int F=0;
        I=strlen(TextBuffer);
        if(I > 45) { I=45; F++; }
        sprintf(Buf,"\"%*.*s%s\"",I,I,TextBuffer,(F?"...":""));
        strcpy(BufKey,Buf);
        free(TextBuffer);
      }
      else
        BufKey[0]=0;

      sprintf(Buf,
        MSG(!MacroDlg->RecBufferSize?
           (DisFlags?MMacroDeleteAssign:MMacroDeleteKey):
           MMacroReDefinedKey),
        KeyText);

      // �஢�ਬ "� �� ᮢ������ �� ���?"
      if(!DisFlags &&
         MacroDlg->Macros[Index].Buffer &&
         MacroDlg->RecBuffer &&
         MacroDlg->Macros[Index].BufferSize == MacroDlg->RecBufferSize &&
         !memcmp(MacroDlg->Macros[Index].Buffer,MacroDlg->RecBuffer,
         MacroDlg->RecBufferSize))
        I=0;
      else
        I=Message(MSG_WARNING,2,MSG(MWarning),
            Buf,
            BufKey,
            MSG(!MacroDlg->RecBufferSize?
                  MMacroDeleteKey2:
                  (DisFlags?MMacroDisDisabledKey:MMacroReDefinedKey2)),
            MSG(DisFlags && MacroDlg->RecBufferSize?MMacroDisOverwrite:MYes),
            MSG(DisFlags && MacroDlg->RecBufferSize?MMacroDisAnotherKey:MNo));

      if(!I)
      {
        if(DisFlags)
        {
          if (Opt.AutoSaveSetup) // 㤠�塞 �� ॥��� ⮫쪮 � ��砥
          {                      // ����� ����祭 ���ᥩ�
            // 㤠��� ����� ������ �� ॥���
            DeleteRegKey(RegKeyName);
          }
          // ࠧ��ᠡ���
          MacroDlg->Macros[Index].Flags&=~MFLAGS_DISABLEMACRO;
        }
        // � �� ��砥 - �뢠��������
        Dialog::SendDlgMessage(hDlg,DM_CLOSE,1,0);
        return TRUE;
      }
      // ����� - ����� �� �������� "���", �� � �� ��� � �㤠 ���
      //  � ����� ���⨬ ���� �����.
      KeyText[0]=0;
    }
    Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,2,(long)KeyText);
//    if(Param2 == KEY_F1 && LastKey == KEY_F1)
//      LastKey=-1;
//    else
      LastKey=Param2;
    return TRUE;
  }
  else if(Msg == DN_CTLCOLORDLGITEM) // ��ᨬ Unchanged
  {
    Param2&=0xFF00FFFFU;      // Unchanged � ��� ᨤ�� � ����襬 ���� ���襣� ᫮��
    Param2|=(Param2&0xFF)<<16;
    return Param2;
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

DWORD KeyMacro::AssignMacroKey()
{
/*
  ������� Define macro �����ͻ
  � Press the desired key    �
  � ________________________ �
  ��������������������������ͼ
*/

  static struct DialogData MacroAssignDlgData[]={
  /* 00 */ DI_DOUBLEBOX,3,1,30,4,0,0,0,0,(char *)MDefineMacroTitle,
  /* 01 */ DI_TEXT,-1,2,0,0,0,0,DIF_BOXCOLOR|DIF_READONLY,0,(char *)MDefineMacro,
  /* 02 */ DI_EDIT,5,3,28,3,1,0,0,1,"",
  };
  MakeDialogItems(MacroAssignDlgData,MacroAssignDlg);
  struct DlgParam Param={this,0,StartMode};

  Dialog Dlg(MacroAssignDlg,sizeof(MacroAssignDlg)/sizeof(MacroAssignDlg[0]),AssignMacroDlgProc,(long)&Param);
  Dlg.SetPosition(-1,-1,34,6);
  Dlg.SetHelp("KeyMacro");
  Dlg.Process();
  /* $ 30.01.2001 SVS
     ���� ᤥ���� �஢��� �� ��� ������ �� ������� �����祭��
  */
  if(Dlg.GetExitCode() == -1)
    return KEY_ESC;
  /* SVS $ */
  return Param.Key;
}

static int Set3State(DWORD Flags,DWORD Chk1,DWORD Chk2)
{
  DWORD Chk12=Chk1|Chk2, FlagsChk12=Flags&Chk12;
  if(FlagsChk12 == Chk12 || FlagsChk12 == 0)
    return (2);
  else
    return (Flags&Chk1?1:0);
}

int KeyMacro::GetMacroSettings(int Key,DWORD &Flags)
{

  static struct DialogData MacroSettingsDlgData[]={
  /* 00 */ DI_DOUBLEBOX,3,1,62,11,0,0,0,0,"",
  /* 01 */ DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MMacroSettingsDisableOutput,
  /* 02 */ DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MMacroSettingsRunAfterStart,
  /* 03 */ DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */ DI_CHECKBOX,5,5,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsCommandLine,
  /* 05 */ DI_CHECKBOX,5,6,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsPluginPanel,
  /* 06 */ DI_CHECKBOX,5,7,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsFolders,
  /* 07 */ DI_CHECKBOX,5,8,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsSelectionPresent,
  /* 08 */ DI_TEXT,3,9,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 09 */ DI_BUTTON,0,10,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 10 */ DI_BUTTON,0,10,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(MacroSettingsDlgData,MacroSettingsDlg);

  char KeyText[66];
  KeyToText(Key,KeyText);
  sprintf(MacroSettingsDlg[0].Data,MSG(MMacroSettingsTitle),KeyText);
  if(!(Key&0x7F000000))
    MacroSettingsDlg[3].Flags|=DIF_DISABLE;

  MacroSettingsDlg[1].Selected=Flags&MFLAGS_DISABLEOUTPUT?1:0;
  MacroSettingsDlg[2].Selected=Flags&MFLAGS_RUNAFTERFARSTART?1:0;

  MacroSettingsDlg[4].Selected=Set3State(Flags,MFLAGS_EMPTYCOMMANDLINE,MFLAGS_NOTEMPTYCOMMANDLINE);
  MacroSettingsDlg[5].Selected=Set3State(Flags,MFLAGS_NOFILEPANELS,MFLAGS_NOPLUGINPANELS);
  MacroSettingsDlg[6].Selected=Set3State(Flags,MFLAGS_NOFILES,MFLAGS_NOFOLDERS);
  MacroSettingsDlg[7].Selected=Set3State(Flags,MFLAGS_SELECTION,MFLAGS_NOSELECTION);

  Dialog Dlg(MacroSettingsDlg,sizeof(MacroSettingsDlg)/sizeof(MacroSettingsDlg[0]));
  Dlg.SetPosition(-1,-1,66,13);
  Dlg.SetHelp("KeyMacroSetting");
  Dlg.Process();
  if (Dlg.GetExitCode()!=9)
    return(FALSE);

  Flags=MacroSettingsDlg[1].Selected?MFLAGS_DISABLEOUTPUT:0;
  Flags|=MacroSettingsDlg[2].Selected?MFLAGS_RUNAFTERFARSTART:0;
  Flags|=MacroSettingsDlg[4].Selected==2?0:
          (MacroSettingsDlg[4].Selected==0?MFLAGS_NOTEMPTYCOMMANDLINE:MFLAGS_EMPTYCOMMANDLINE);
  Flags|=MacroSettingsDlg[5].Selected==2?0:
          (MacroSettingsDlg[5].Selected==0?MFLAGS_NOPLUGINPANELS:MFLAGS_NOFILEPANELS);
  Flags|=MacroSettingsDlg[6].Selected==2?0:
          (MacroSettingsDlg[6].Selected==0?MFLAGS_NOFOLDERS:MFLAGS_NOFILES);
  Flags|=MacroSettingsDlg[7].Selected==2?0:
          (MacroSettingsDlg[7].Selected==0?MFLAGS_NOSELECTION:MFLAGS_SELECTION);

  return(TRUE);
}

int KeyMacro::PlayKeyMacro(char *KeyBuffer)
{
  ReleaseTempBuffer();

  if((TempMacro=(struct MacroRecord *)malloc(sizeof(MacroRecord))) == NULL)
    return FALSE;
  TempMacro->Buffer=NULL;
  TempMacro->Flags=0;
  TempMacro->Key=0;
  TempMacro->BufferSize=0;

  if(!ParseMacroString(TempMacro,KeyBuffer))
  {
    ReleaseTempBuffer();
    return FALSE;
  }

  TempMacroType=MTEMP_DYNAMIC;
  if (TempMacro->Flags&MFLAGS_DISABLEOUTPUT)
  {
    if(LockScr) delete LockScr;
    LockScr=new LockScreen;
  }
  Executing=TRUE;
  ExecKeyPos=0;
  return TRUE;
}

int KeyMacro::PlayKeyMacro(struct MacroRecord *MRec)
{
  ReleaseTempBuffer();

  TempMacro=MRec;

  if(!TempMacro)
    return FALSE;

  if (TempMacro->Flags&MFLAGS_DISABLEOUTPUT)
  {
    if(LockScr) delete LockScr;
    LockScr=new LockScreen;
  }
  Executing=TRUE;
  ExecKeyPos=0;
  return TRUE;
}

// ����� ��ப���� ���������⮢ � ���� ������
int KeyMacro::ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr)
{
  int J;
  if(!CurMacro || !BufPtr || !*BufPtr)
    return FALSE;
  // ����� ������� ��ନ஢���, ��稭��� ࠧ��� ��᫥����⥫쭮��,
  // ����� ��室���� � Buffer
  while (1)
  {
    // �ய�᪠�� ����騥 �஡���� ᨬ����
    while (isspace(*BufPtr))
      BufPtr++;
    if (*BufPtr==0)
      break;

    char *CurBufPtr=BufPtr;

    // �饬 ����� ��।���� �������� ������
    while (*BufPtr && !isspace(*BufPtr))
      BufPtr++;
    int Length=BufPtr-CurBufPtr;
    char CurKeyText[50];
    memcpy(CurKeyText,CurBufPtr,Length);
    CurKeyText[Length]=0;

    // � CurKeyText - �������� ������. ���஡㥬 ���᪠�� �� ���...
    DWORD KeyCode;
    KeyCode=KeyNameToKey(CurKeyText);
    // ��� ������, ������� ��� ��� � ���� ��᫥����⥫쭮��.
    if (KeyCode!=-1)
    {
      CurMacro->Buffer=(DWORD *)realloc(CurMacro->Buffer,sizeof(*CurMacro->Buffer)*(CurMacro->BufferSize+1));
      if (CurMacro->Buffer==NULL)
      {
        return FALSE;
      }
      CurMacro->Buffer[CurMacro->BufferSize]=KeyCode;
      CurMacro->BufferSize++;
    }
  }
  return TRUE;
}


// �㭪�� ����祭�� ������ �㦭��� ����� � ���ᨢ�
// Ret=-1 - �� ������ ⠪����.
// �᫨ CheckMode=-1 - ����� ��䨣� � ����� ०���, �.�. ���� �����訩��
int KeyMacro::GetIndex(int Key, int ChechMode)
{
  int Pos;
  if(Macros)
    for(Pos=0; Pos < MacrosNumber; ++Pos)
//      if (Macros[Pos].Key==Key && Macros[Pos].BufferSize)
      if (LocalUpper(Macros[Pos].Key)==LocalUpper(Key) && Macros[Pos].BufferSize > 0)
        if((Macros[Pos].Flags&MFLAGS_MODEMASK) == ChechMode || ChechMode == -1)
          return Pos;
  return -1;
}

// ����祭�� ࠧ���, ����������� 㪠����� ����ᮬ
// Ret= 0 - �� ������ ⠪����.
// �᫨ CheckMode=-1 - ����� ��䨣� � ����� ०���, �.�. ���� �����訩��
int KeyMacro::GetRecordSize(int Key, int CheckMode)
{
  int Pos=GetIndex(Key,CheckMode);
  if(Pos == -1)
    return 0;
  return sizeof(struct MacroRecord)+Macros[Pos].BufferSize;
}

/* $ 21.12.2000 SVS
   ���᮪�⨬ ���.
*/
// ������� �������� ���� �� ����
char* KeyMacro::GetSubKey(int Mode)
{
  return (char *)((Mode >= MACRO_SHELL && Mode < MACRO_LAST)?
            MacroModeName[Mode]:
            (Mode == MACRO_OTHER?MacroModeNameOther:""));
}

// ������� ��� ���� �� �����
int KeyMacro::GetSubKey(char *Mode)
{
  if(!stricmp(MacroModeNameOther,Mode))
    return MACRO_OTHER;

  int I;
  for(I=MACRO_SHELL; I < MACRO_LAST; ++I)
    if(!stricmp(MacroModeName[I],Mode))
      return I;
  return -1;
}

BOOL KeyMacro::CheckEditSelected(DWORD CurFlags)
{
  if(Mode==MACRO_EDITOR)
  {
    char Type[200],Name[NM];
    Modal* CurModal=CtrlObject->ModalManager.ActiveModal;
    if (CurModal && CurModal->GetTypeAndName(Type,Name)==MODALTYPE_EDITOR)
    {
      int CurSelected=CurModal->ProcessKey(KEY_MEDIT_ISSELECTED);
      if((CurFlags&MFLAGS_SELECTION) && !CurSelected ||
         (CurFlags&MFLAGS_NOSELECTION) && CurSelected)
          return FALSE;
    }
  }
  return TRUE;
}

BOOL KeyMacro::CheckPanel(int PanelMode,DWORD CurFlags)
{
  if(PanelMode == PLUGIN_PANEL && (CurFlags&MFLAGS_NOPLUGINPANELS) ||
     PanelMode == NORMAL_PANEL && (CurFlags&MFLAGS_NOFILEPANELS))
    return FALSE;
  return TRUE;
}

BOOL KeyMacro::CheckCmdLine(int CmdLength,DWORD CurFlags)
{
 if ((CurFlags&MFLAGS_EMPTYCOMMANDLINE) && CmdLength!=0 ||
     (CurFlags&MFLAGS_NOTEMPTYCOMMANDLINE) && CmdLength==0)
      return FALSE;
  return TRUE;
}

BOOL KeyMacro::CheckFileFolder(Panel *ActivePanel,DWORD CurFlags)
{
  char FileName[NM*2];
  int FileAttr=-1;
  ActivePanel->GetFileName(FileName,ActivePanel->GetCurrentPos(),FileAttr);
  if(FileAttr != -1)
  {
    if((FileAttr&FA_DIREC) && (CurFlags&MFLAGS_NOFOLDERS) ||
      !(FileAttr&FA_DIREC) && (CurFlags&MFLAGS_NOFILES))
      return FALSE;
  }
  return TRUE;
}

BOOL KeyMacro::CheckAll(DWORD CurFlags)
{
  // �஢�ઠ �� ����/�� ���� � ���.��ப� (� � ।����? :-)
  if(!CheckCmdLine(CtrlObject->CmdLine->GetLength(),CurFlags))
    return FALSE;

  // �஢�ન ������ � ⨯� 䠩��
  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  if(ActivePanel!=NULL)// && (CurFlags&MFLAGS_MODEMASK)==MACRO_SHELL)
  {
    if(!CheckPanel(ActivePanel->GetMode(),CurFlags))
      return FALSE;

    if(!CheckFileFolder(ActivePanel,CurFlags))
      return FALSE;

    int SelCount=ActivePanel->GetRealSelCount();
    if(Mode!=MACRO_EDITOR) // ??? ������ �� ���� �������� !!!
    {
      if((CurFlags&MFLAGS_SELECTION) && SelCount < 1 ||
         (CurFlags&MFLAGS_NOSELECTION) && SelCount >= 1)
        return FALSE;
    }
  }

  if(!CheckEditSelected(CurFlags))
    return FALSE;

  return TRUE;
}
