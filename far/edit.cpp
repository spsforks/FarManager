/*
edit.cpp

��������� �����筮� ��ப� ।���஢����

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
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

static const char *WordDiv="!%^&*()+|{}:\"<>?`-=\\[];',./";

static int EditOutDisabled=0;
static int EditEncodeDisabled=0;
static int Recurse=0;


Edit::Edit()
{
  ConvertTabs=0;
  Str=new char[1];
  StrSize=0;
  *Str=0;
  CurPos=0;
  CursorPos=0;
  ClearFlag=0;
  Overtype=0;
  TableSet=NULL;
  LeftPos=0;
  PasswordMode=0;
  MaxLength=-1;
  SelStart=-1;
  SelEnd=0;
  EditBeyondEnd=TRUE;
  Color=F_LIGHTGRAY|B_BLACK;
  SelColor=F_WHITE|B_BLACK;
  EndType=EOL_NONE;
  MarkingBlock=FALSE;
  EditorMode=FALSE;
  ColorList=NULL;
  ColorCount=0;
}


Edit::~Edit()
{
  if (ColorList!=NULL)
    delete ColorList;

  delete Str;
}


void Edit::DisplayObject()
{
  if (EditOutDisabled)
    return;
  FastShow();
  if (Overtype)
    SetCursorType(1,99);
  else
    SetCursorType(1,-1);
  MoveCursor(X1+CursorPos-LeftPos,Y1);
}


void Edit::FastShow()
{
  int EditLength;
  if (!EditBeyondEnd && CurPos>StrSize && StrSize>=0)
    CurPos=StrSize;
  EditLength=ObjWidth;
  if (MaxLength!=-1)
  {
    if (StrSize>MaxLength)
    {
      Str[MaxLength]=0;
      StrSize=MaxLength;
    }
    if (CurPos>MaxLength-1)
      CurPos=MaxLength>0 ? (MaxLength-1):0;
  }
  int TabCurPos=GetTabCurPos();
  if (TabCurPos-LeftPos>EditLength-1)
    LeftPos=TabCurPos-EditLength+1;
  if (TabCurPos<LeftPos)
    LeftPos=TabCurPos;
  GotoXY(X1,Y1);
  int TabSelStart=(SelStart==-1) ? -1:RealPosToTab(SelStart);
  int TabSelEnd=(SelEnd<0) ? -1:RealPosToTab(SelEnd);

  if (!ConvertTabs && memchr(Str,'\t',StrSize)!=NULL)
  {
    char *SaveStr;
    int SaveStrSize=StrSize,SaveCurPos=CurPos;
    if ((SaveStr=new char[StrSize+1])==NULL)
      return;
    memcpy(SaveStr,Str,StrSize);
    ReplaceTabs();
    CursorPos=CurPos;
    if (CurPos-LeftPos>EditLength-1)
      LeftPos=CurPos-EditLength+1;
    ShowString(Str,TabSelStart,TabSelEnd);
    memcpy(Str,SaveStr,SaveStrSize);
    Str[SaveStrSize]=0;
    delete SaveStr;
    StrSize=SaveStrSize;
    CurPos=SaveCurPos;
    Str=(char *)realloc(Str,StrSize+1);
  }
  else
  {
    ShowString(Str,TabSelStart,TabSelEnd);
    CursorPos=CurPos;
  }
  ApplyColor();
}


void Edit::ShowString(char *ShowStr,int TabSelStart,int TabSelEnd)
{
  int EditLength=ObjWidth;
  if (PasswordMode)
  {
    char *PswStr=new char[StrSize+1];
    if (PswStr==NULL)
      return;
    memset(PswStr,'*',StrSize);
    PswStr[StrSize]=0;
    PasswordMode=0;
    ShowString(PswStr,TabSelStart,TabSelEnd);
    PasswordMode=1;
    delete PswStr;
    return;
  }
  char *SaveStr=NULL;
  if (TableSet)
  {
    SaveStr=new char[StrSize+1];
    if (SaveStr==NULL)
      return;
    memcpy(SaveStr,Str,StrSize);
    DecodeString(ShowStr,TableSet->DecodeTable,StrSize);
  }
  if (memchr(Str,0,StrSize)!=0)
  {
    if (SaveStr==NULL)
    {
      SaveStr=new char[StrSize+1];
      if (SaveStr==NULL)
        return;
      memcpy(SaveStr,Str,StrSize);
    }
    for (int I=0;I<StrSize;I++)
      if (Str[I]==0)
        Str[I]=' ';
  }

  SetColor(Color);

  if (TabSelStart==-1)
  {
    if (ClearFlag && LeftPos<StrSize)
    {
      SetColor(COL_DIALOGEDITUNCHANGED);
      Text(&ShowStr[LeftPos]);
      SetColor(Color);
      int BlankLength=EditLength-(StrSize-LeftPos);
      if (BlankLength>0)
        mprintf("%*s",BlankLength,"");
    }
    else
      mprintf("%-*.*s",EditLength,EditLength,LeftPos>StrSize ? "":&ShowStr[LeftPos]);
  }
  else
  {
    char *OutStr=new char[EditLength+1];
    if (OutStr==NULL)
      return;
    if ((TabSelStart-=LeftPos)<0)
      TabSelStart=0;
    int AllString=(TabSelEnd==-1);
    if (AllString)
      TabSelEnd=EditLength;
    else
      if ((TabSelEnd-=LeftPos)<0)
        TabSelEnd=0;
    sprintf(OutStr,"%-*.*s",EditLength,EditLength,LeftPos>StrSize ? "":&ShowStr[LeftPos]);
    if (TabSelStart>=EditLength || !AllString && TabSelStart>=StrSize ||
        TabSelEnd<TabSelStart)
      Text(OutStr);
    else
    {
      mprintf("%.*s",TabSelStart,OutStr);
      SetColor(SelColor);
      mprintf("%.*s",TabSelEnd-TabSelStart,OutStr+TabSelStart);
      if (TabSelEnd<EditLength)
      {
        SetColor(ClearFlag ? SelColor:Color);
        Text(OutStr+TabSelEnd);
      }
    }
    delete OutStr;
  }
  if (SaveStr)
  {
    memcpy(Str,SaveStr,StrSize);
    delete SaveStr;
  }
}


int Edit::RecurseProcessKey(int Key)
{
  Recurse++;
  int RetCode=ProcessKey(Key);
  Recurse--;
  return(RetCode);
}


int Edit::ProcessKey(int Key)
{
  switch(Key)
  {
    case KEY_ADD:
      Key='+';
      break;
    case KEY_SUBTRACT:
      Key='-';
      break;
    case KEY_MULTIPLY:
      Key='*';
      break;
    case KEY_DIVIDE:
      Key='/';
      break;
    case KEY_CTRLC:
      Key=KEY_CTRLINS;
      break;
    case KEY_CTRLV:
      Key=KEY_SHIFTINS;
      break;
    case KEY_CTRLX:
      Key=KEY_SHIFTDEL;
      break;
  }

  int PrevSelStart=-1,PrevSelEnd=0;

  if ((Key==KEY_DEL && Opt.EditorDelRemovesBlocks || Key==KEY_CTRLD) &&
      !EditorMode && SelStart!=-1 && SelStart<SelEnd)
  {
    DeleteBlock();
    Show();
    return(TRUE);
  }

  if (!ShiftPressed && !Editor::IsShiftKey(Key) && !Recurse &&
      Key!=KEY_SHIFT && Key!=KEY_CTRL && Key!=KEY_ALT && Key!=KEY_RCTRL &&
      Key!=KEY_RALT && Key!=KEY_NONE)
  {
    MarkingBlock=FALSE;
    if (!Opt.EditorPersistentBlocks && Key!=KEY_CTRLINS && !EditorMode)
    {
      PrevSelStart=SelStart;
      PrevSelEnd=SelEnd;
      Select(-1,0);
      Show();
    }

  }
  if (!EditEncodeDisabled && Key<256 && TableSet)
    Key=TableSet->EncodeTable[Key];

  if (Key==KEY_DEL && ClearFlag && CurPos>=StrSize)
    Key=KEY_CTRLY;

  if (ClearFlag && (Key<256 && Key>=31 || Key==KEY_CTRLBRACKET ||
      Key==KEY_CTRLBACKBRACKET || Key==KEY_CTRLSHIFTBRACKET ||
      Key==KEY_CTRLSHIFTBACKBRACKET || Key==KEY_SHIFTENTER))
  {
    LeftPos=0;
    SetString("");
    Show();
  }

  if (Key>=KEY_RCTRL0 && Key<=KEY_RCTRL9)
  {
    char ShortcutFolder[NM],PluginModule[NM],PluginFile[NM],PluginData[8192];
    if (GetShortcutFolder(Key,ShortcutFolder,PluginModule,PluginFile,PluginData))
    {
      if (ClearFlag)
      {
        LeftPos=0;
        SetString("");
      }
      InsertString(ShortcutFolder);
      Show();
      return(TRUE);
    }
  }

  if (Key!=KEY_NONE && Key!=KEY_IDLE && Key!=KEY_SHIFTINS &&
      (Key<KEY_F1 || Key>KEY_F12) && Key!=KEY_ALT && Key!=KEY_SHIFT &&
      Key!=KEY_CTRL && Key!=KEY_RALT && Key!=KEY_RCTRL &&
      (Key<KEY_ALT_BASE || Key>=KEY_ALT_BASE+256))
  {
    ClearFlag=0;
    Show();
  }

  switch(Key)
  {
    case KEY_SHIFTENTER:
      {
        char FileName[NM],ShortFileName[NM];
        if (CtrlObject->ActivePanel!=NULL && CtrlObject->ActivePanel->GetCurName(FileName,ShortFileName))
        {
          InsertString(FileName);
          Show();
        }
      }
      return(TRUE);
    case KEY_CTRLBRACKET:
    case KEY_CTRLBACKBRACKET:
    case KEY_CTRLSHIFTBRACKET:
    case KEY_CTRLSHIFTBACKBRACKET:
      {
        Panel *SrcPanel;
        switch(Key)
        {
          case KEY_CTRLBRACKET:
            SrcPanel=CtrlObject->LeftPanel;
            break;
          case KEY_CTRLBACKBRACKET:
            SrcPanel=CtrlObject->RightPanel;
            break;
          case KEY_CTRLSHIFTBRACKET:
            SrcPanel=CtrlObject->ActivePanel;
            break;
          case KEY_CTRLSHIFTBACKBRACKET:
            SrcPanel=CtrlObject->GetAnotherPanel(CtrlObject->ActivePanel);
            break;
        }
        if (SrcPanel!=NULL)
        {
          char PanelDir[NM];
          SrcPanel->GetCurDir(PanelDir);
          if (SrcPanel->GetShowShortNamesMode())
            ConvertNameToShort(PanelDir,PanelDir);
          AddEndSlash(PanelDir);
          for (int I=0;PanelDir[I]!=0;I++)
            InsertKey(PanelDir[I]);
          Show();
        }
      }
      return(TRUE);
    case KEY_SHIFTLEFT:
      if (CurPos>0)
      {
        RecurseProcessKey(KEY_LEFT);
        if (!MarkingBlock)
        {
          Select(-1,0);
          MarkingBlock=TRUE;
        }
        if (SelStart!=-1 && SelStart<=CurPos)
          Select(SelStart,CurPos);
        else
        {
          int EndPos=CurPos+1;
          int NewStartPos=CurPos;
          if (EndPos>StrSize)
            EndPos=StrSize;
          if (NewStartPos>StrSize)
            NewStartPos=StrSize;
          AddSelect(NewStartPos,EndPos);
        }
        Show();
      }
      return(TRUE);
    case KEY_SHIFTRIGHT:
      if (!MarkingBlock)
      {
        Select(-1,0);
        MarkingBlock=TRUE;
      }
      if (SelStart!=-1 && SelEnd==-1 || SelEnd>CurPos)
      {
        if (CurPos+1==SelEnd)
          Select(-1,0);
        else
          Select(CurPos+1,SelEnd);
      }
      else
        AddSelect(CurPos,CurPos+1);
      RecurseProcessKey(KEY_RIGHT);
      return(TRUE);
    case KEY_CTRLSHIFTLEFT:
      if (CurPos>StrSize)
        CurPos=StrSize;
      if (CurPos>0)
        RecurseProcessKey(KEY_SHIFTLEFT);
      while (CurPos>0 && !(strchr(WordDiv,Str[CurPos])==NULL &&
             strchr(WordDiv,Str[CurPos-1])!=NULL && !isspace(Str[CurPos])))
      {
        if (!isspace(Str[CurPos]) && isspace(Str[CurPos-1]))
          break;
        RecurseProcessKey(KEY_SHIFTLEFT);
      }
      Show();
      return(TRUE);
    case KEY_CTRLSHIFTRIGHT:
      if (CurPos>=StrSize)
        return(FALSE);
      RecurseProcessKey(KEY_SHIFTRIGHT);
      while (CurPos<StrSize && !(strchr(WordDiv,Str[CurPos])!=NULL &&
             strchr(WordDiv,Str[CurPos-1])==NULL))
      {
        if (!isspace(Str[CurPos]) && isspace(Str[CurPos-1]))
          break;
        RecurseProcessKey(KEY_SHIFTRIGHT);
        if (MaxLength!=-1 && CurPos==MaxLength-1)
          break;
      }
      Show();
      return(TRUE);
    case KEY_SHIFTHOME:
      DisableEditOut(TRUE);
      while (CurPos>0)
        RecurseProcessKey(KEY_SHIFTLEFT);
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    case KEY_SHIFTEND:
      DisableEditOut(TRUE);
      while (CurPos<StrSize)
        RecurseProcessKey(KEY_SHIFTRIGHT);
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    case KEY_BS:
      if (CurPos<=0)
        return(FALSE);
      CurPos--;
      if (CurPos<=LeftPos)
      {
        LeftPos-=15;
        if (LeftPos<0)
          LeftPos=0;
      }
      if (!RecurseProcessKey(KEY_DEL))
        Show();
      return(TRUE);
    case KEY_CTRLBS:
      if (CurPos>StrSize)
        CurPos=StrSize;
      DisableEditOut(TRUE);
//      while (CurPos>0 && isspace(Str[CurPos-1]))
//        RecurseProcessKey(KEY_BS);
      while (1)
      {
        int StopDelete=FALSE;
        if (CurPos>1 && isspace(Str[CurPos-1])!=isspace(Str[CurPos-2]))
          StopDelete=TRUE;
        RecurseProcessKey(KEY_BS);
        if (CurPos==0 || StopDelete)
          break;
        if (strchr(WordDiv,Str[CurPos-1])!=NULL)
          break;
      }
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    case KEY_CTRLQ:
      {
        INPUT_RECORD rec;
        while (1)
        {
          Key=GetInputRecord(&rec);
          if (Key!=KEY_NONE && Key!=KEY_IDLE && rec.Event.KeyEvent.uChar.AsciiChar)
            break;
        }

        InsertKey(rec.Event.KeyEvent.uChar.AsciiChar);
        Show();
      }
      return(TRUE);
    case KEY_CTRLT:
    case KEY_CTRLDEL:
      if (CurPos>=StrSize)
        return(FALSE);
      DisableEditOut(TRUE);
//      while (CurPos<StrSize && isspace(Str[CurPos]))
//        RecurseProcessKey(KEY_DEL);
      while (1)
      {
        int StopDelete=FALSE;
        if (CurPos<StrSize-1 && isspace(Str[CurPos]) && !isspace(Str[CurPos+1]))
          StopDelete=TRUE;
        RecurseProcessKey(KEY_DEL);
        if (CurPos>=StrSize || StopDelete)
          break;
        if (strchr(WordDiv,Str[CurPos])!=NULL)
          break;
      }
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    case KEY_CTRLY:
      CurPos=0;
      *Str=0;
      StrSize=0;
      Str=(char *)realloc(Str,1);
      Select(-1,0);
      Show();
      return(TRUE);
    case KEY_CTRLK:
      if (CurPos>=StrSize)
        return(FALSE);
      if (!EditBeyondEnd)
      {
        if (CurPos<SelEnd)
          SelEnd=CurPos;
        if (SelEnd<SelStart)
        {
          SelEnd=0;
          SelStart=-1;
        }
      }
      Str[CurPos]=0;
      StrSize=CurPos;
      Str=(char *)realloc(Str,StrSize+1);
      Show();
      return(TRUE);
    case KEY_HOME:
    case KEY_CTRLHOME:
      CurPos=0;
      Show();
      return(TRUE);
    case KEY_END:
    case KEY_CTRLEND:
      CurPos=StrSize;
      Show();
      return(TRUE);
    case KEY_LEFT:
    case KEY_CTRLS:
      if (CurPos>0)
      {
        CurPos--;
        Show();
      }
      return(TRUE);
    case KEY_RIGHT:
    case KEY_CTRLD:
      CurPos++;
      Show();
      return(TRUE);
    case KEY_INS:
      Overtype^=1;
      Show();
      return(TRUE);
    case KEY_DEL:
      if (CurPos>=StrSize)
        return(FALSE);
      if (SelStart!=-1)
      {
        if (SelEnd!=-1 && CurPos<SelEnd)
          SelEnd--;
        if (CurPos<SelStart)
          SelStart--;
        if (SelEnd!=-1 && SelEnd<=SelStart)
        {
          SelStart=-1;
          SelEnd=0;
        }
      }
      memmove(Str+CurPos,Str+CurPos+1,StrSize-CurPos);
      StrSize--;
      Str=(char *)realloc(Str,StrSize+1);
      Show();
      return(TRUE);
    case KEY_CTRLLEFT:
      if (CurPos>StrSize)
        CurPos=StrSize;
      if (CurPos>0)
        CurPos--;
      while (CurPos>0 && !(strchr(WordDiv,Str[CurPos])==NULL &&
             strchr(WordDiv,Str[CurPos-1])!=NULL && !isspace(Str[CurPos])))
      {
        if (!isspace(Str[CurPos]) && isspace(Str[CurPos-1]))
          break;
        CurPos--;
      }
      Show();
      return(TRUE);
    case KEY_CTRLRIGHT:
      if (CurPos>=StrSize)
        return(FALSE);
      CurPos++;
      while (CurPos<StrSize && !(strchr(WordDiv,Str[CurPos])!=NULL &&
             strchr(WordDiv,Str[CurPos-1])==NULL))
      {
        if (!isspace(Str[CurPos]) && isspace(Str[CurPos-1]))
          break;
        CurPos++;
      }
      Show();
      return(TRUE);
    case KEY_SHIFTDEL:
      if (SelStart==-1 || SelStart>=SelEnd)
        return(FALSE);
      RecurseProcessKey(KEY_CTRLINS);
      DeleteBlock();
      Show();
      return(TRUE);
    case KEY_CTRLINS:
      if (!PasswordMode)
        if (SelStart==-1 || SelStart>=SelEnd)
          CopyToClipboard(Str);
        else
          if (SelEnd<=StrSize)
          {
            int Ch=Str[SelEnd];
            Str[SelEnd]=0;
            CopyToClipboard(Str+SelStart);
            Str[SelEnd]=Ch;
          }
      return(TRUE);
    case KEY_SHIFTINS:
      {
        char *ClipText=PasteFromClipboard();
        if (ClipText==NULL)
          return(TRUE);
        if (!Opt.EditorPersistentBlocks)
          DeleteBlock();
        for (int I=strlen(Str)-1;I>=0 && iseol(Str[I]);I--)
          Str[I]=0;
        for (int I=0;ClipText[I];I++)
          if (iseol(ClipText[I]))
          {
            if (iseol(ClipText[I+1]))
              memmove(&ClipText[I],&ClipText[I+1],strlen(&ClipText[I+1])+1);
            if (ClipText[I+1]==0)
              ClipText[I]=0;
            else
              ClipText[I]=' ';
          }
        if (ClearFlag)
        {
          LeftPos=0;
          SetString(ClipText);
          ClearFlag=FALSE;
        }
        else
          InsertString(ClipText);
        delete ClipText;
        Show();
      }
      return(TRUE);
    case KEY_SHIFTTAB:
      {
        int Size=CurPos % Opt.TabSize;
        if (Size==0 && CurPos!=0)
          Size=Opt.TabSize;
        CurPos-=Size;
        Show();
      }
      return(TRUE);
    default:
      if (Key==KEY_NONE || Key==KEY_IDLE || Key==KEY_ENTER || Key>=256)
        break;
      if (!Opt.EditorPersistentBlocks)
      {
        if (PrevSelStart!=-1)
        {
          SelStart=PrevSelStart;
          SelEnd=PrevSelEnd;
        }
        DeleteBlock();
      }
      if (InsertKey(Key))
      {
        if (Key==KEY_TAB && ConvertTabs)
          ReplaceTabs();
        Show();
      }
      return(TRUE);
  }
  return(FALSE);
}


int Edit::InsertKey(int Key)
{
  char *NewStr;
  if (Key==KEY_TAB && Overtype)
  {
    CurPos+=Opt.TabSize - (CurPos % Opt.TabSize);
    return(TRUE);
  }
  if (CurPos>=StrSize)
  {
    if ((NewStr=(char *)realloc(Str,CurPos+2))==NULL)
      return(FALSE);
    Str=NewStr;
    sprintf(&Str[StrSize],"%*s",CurPos-StrSize,"");
    StrSize=CurPos+1;
  }
  else
    if (!Overtype)
      StrSize++;
  if ((NewStr=(char *)realloc(Str,StrSize+1))==NULL)
    return(TRUE);
  Str=NewStr;

  if (!Overtype)
  {
    memmove(Str+CurPos+1,Str+CurPos,StrSize-CurPos);
    if (SelStart!=-1)
    {
      if (SelEnd!=-1 && CurPos<SelEnd)
        SelEnd++;
      if (CurPos<SelStart)
        SelStart++;
    }
  }
  Str[CurPos++]=Key;
  Str[StrSize]=0;
  return(TRUE);
}


void Edit::SetObjectColor(int Color,int SelColor)
{
  Edit::Color=Color;
  Edit::SelColor=SelColor;
}


void Edit::GetString(char *Str,int MaxSize)
{
  strncpy(Str,Edit::Str,MaxSize-1);
  Str[MaxSize-1]=0;
}


char* Edit::GetStringAddr()
{
  return(Edit::Str);
}


void Edit::SetString(char *Str)
{
  Select(-1,0);
  SetBinaryString(Str,strlen(Str));
}


void Edit::SetEOL(char *EOL)
{
  if (EOL[0]=='\r')
    if (EOL[1]=='\n')
      EndType=EOL_CRLF;
    else
      EndType=EOL_CR;
  else
    if (EOL[0]=='\n')
      EndType=EOL_LF;
    else
      EndType=EOL_NONE;
}


void Edit::SetBinaryString(char *Str,int Length)
{
  if (Length>0)
    if (Str[Length-1]=='\r')
    {
      EndType=EOL_CR;
      Length--;
    }
    else
      if (Str[Length-1]=='\n')
      {
        Length--;
        if (Length>0 && Str[Length-1]=='\r')
        {
          EndType=EOL_CRLF;
          Length--;
        }
        else
          EndType=EOL_LF;
      }
      else
        EndType=EOL_NONE;
  char *NewStr=(char *)realloc(Edit::Str,Length+1);
  if (NewStr==NULL)
    return;
  Edit::Str=NewStr;
  StrSize=Length;
  memcpy(Edit::Str,Str,Length);
  Edit::Str[Length]=0;
  if (ConvertTabs)
    ReplaceTabs();
  CurPos=StrSize;
}


void Edit::GetBinaryString(char **Str,char **EOL,int &Length)
{
  *Str=Edit::Str;
  if (EOL!=NULL)
    switch(EndType)
    {
      case EOL_NONE:
        *EOL="";
        break;
      case EOL_CR:
        *EOL="\r";
        break;
      case EOL_LF:
        *EOL="\n";
        break;
      case EOL_CRLF:
        *EOL="\r\n";
        break;
    }
  Length=StrSize;
}


int Edit::GetSelString(char *Str,int MaxSize)
{
  if (SelStart==-1 || SelEnd!=-1 && SelEnd<=SelStart ||
      SelStart>=StrSize)
  {
    *Str=0;
    return(FALSE);
  }
  int CopyLength;
  if (SelEnd==-1)
    CopyLength=MaxSize-1;
  else
    CopyLength=Min(MaxSize-1,SelEnd-SelStart);
  strncpy(Str,Edit::Str+SelStart,CopyLength);
  Str[CopyLength]=0;
  return(TRUE);
}


void Edit::InsertString(char *Str)
{
  Select(-1,0);
  InsertBinaryString(Str,strlen(Str));
}


void Edit::InsertBinaryString(char *Str,int Length)
{
  char *NewStr;

  ClearFlag=0;

  if (CurPos>StrSize)
  {
    if ((NewStr=(char *)realloc(Edit::Str,CurPos+1))==NULL)
      return;
    Edit::Str=NewStr;
    sprintf(&Edit::Str[StrSize],"%*s",CurPos-StrSize,"");
    StrSize=CurPos;
  }

  int TmpSize=StrSize-CurPos;
  char *TmpStr=new char[TmpSize];
  memcpy(TmpStr,&Edit::Str[CurPos],TmpSize);

  StrSize+=Length;
  if ((NewStr=(char *)realloc(Edit::Str,StrSize+1))==NULL)
    return;
  Edit::Str=NewStr;
  memcpy(&Edit::Str[CurPos],Str,Length);
  CurPos+=Length;
  memcpy(Edit::Str+CurPos,TmpStr,TmpSize);
  Edit::Str[StrSize]=0;
  delete TmpStr;
  if (ConvertTabs)
    ReplaceTabs();
}


int Edit::GetLength()
{
  return(StrSize);
}


int Edit::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  if ((MouseEvent->dwButtonState & 3)==0)
    return(FALSE);
  if (MouseEvent->dwMousePosition.X<X1 || MouseEvent->dwMousePosition.X>X2 ||
      MouseEvent->dwMousePosition.Y!=Y1)
    return(FALSE);
  SetTabCurPos(MouseEvent->dwMousePosition.X - X1 + LeftPos);
  Show();
  return(TRUE);
}


int Edit::Search(char *Str,int Position,int Case,int Reverse)
{
  if (Reverse)
  {
    Position--;
    if (Position>=StrSize)
      Position=StrSize-1;
    if (Position<0)
      return(FALSE);
  }
  if (Position<StrSize && *Str)
    for (int I=Position;Reverse && I>=0 || !Reverse && I<StrSize;Reverse ? I--:I++)
      for (int J=0;;J++)
      {
        if (Str[J]==0)
        {
          CurPos=I;
          return(TRUE);
        }
        int Ch=(TableSet==NULL) ? Edit::Str[I+J]:TableSet->DecodeTable[Edit::Str[I+J]];
        if (Case)
        {
          if (Ch!=Str[J])
            break;
        }
        else
          if (LocalUpper(Ch)!=LocalUpper(Str[J]))
            break;
      }
  return(FALSE);
}


void Edit::ReplaceTabs()
{
  char *TabPtr;
  int Pos,S;
  while ((TabPtr=(char *)memchr(Str,'\t',StrSize))!=NULL)
  {
    Pos=TabPtr-Str;
    S=Opt.TabSize-((TabPtr-Str) % Opt.TabSize);
    int PrevStrSize=StrSize;
    StrSize+=S-1;
    if (CurPos>Pos)
      CurPos+=S-1;
    Str=(char *)realloc(Str,StrSize+1);
    TabPtr=Str+Pos;
    memmove(TabPtr+S,TabPtr+1,PrevStrSize-Pos);
    memset(TabPtr,' ',S);
    Str[StrSize]=0;
  }
}


int Edit::GetTabCurPos()
{
  return(RealPosToTab(CurPos));
}


void Edit::SetTabCurPos(int NewPos)
{
  CurPos=TabPosToReal(NewPos);
}


int Edit::RealPosToTab(int Pos)
{
  int TabPos,I;

  if (ConvertTabs || memchr(Str,'\t',StrSize)==NULL)
    return(Pos);

  for (TabPos=0,I=0;I<Pos;I++)
  {
    if (Str[I]=='\t')
      TabPos+=Opt.TabSize - (TabPos % Opt.TabSize);
    else
      if (Str[I]==0)
      {
        TabPos+=Pos-I;
        break;
      }
      else
        TabPos++;
  }
  return(TabPos);
}


int Edit::TabPosToReal(int Pos)
{
  int TabPos,I;

  if (ConvertTabs || memchr(Str,'\t',StrSize)==NULL)
    return(Pos);

  for (TabPos=0,I=0;TabPos<Pos;I++)
  {
    if (Str[I]=='\t')
    {
      int NewTabPos=TabPos+Opt.TabSize - (TabPos % Opt.TabSize);
      if (NewTabPos>Pos)
        break;
      TabPos=NewTabPos;
    }
    else
      if (Str[I]==0)
      {
        I+=Pos-TabPos;
        break;
      }
      else
        TabPos++;
  }
  return(I);
}


void Edit::Select(int Start,int End)
{
  SelStart=Start;
  SelEnd=End;
  if (SelEnd<SelStart && SelEnd!=-1)
  {
    SelStart=-1;
    SelEnd=0;
  }
  if (SelEnd>StrSize)
    SelEnd=StrSize;
}


void Edit::AddSelect(int Start,int End)
{
  if (Start<SelStart || SelStart==-1)
    SelStart=Start;
  if (End==-1 || End>SelEnd && SelEnd!=-1)
    SelEnd=End;
  if (SelEnd>StrSize)
    SelEnd=StrSize;
  if (SelEnd<SelStart && SelEnd!=-1)
  {
    SelStart=-1;
    SelEnd=0;
  }
}


void Edit::GetSelection(int &Start,int &End)
{
  if (SelEnd>StrSize+1)
    SelEnd=StrSize+1;
  if (SelStart>StrSize+1)
    SelStart=StrSize+1;

  Start=SelStart;
  End=SelEnd;

  if (End>StrSize)
    End=StrSize;
  if (Start>StrSize)
    Start=StrSize;
}


void Edit::GetRealSelection(int &Start,int &End)
{
  Start=SelStart;
  End=SelEnd;
}


void Edit::DisableEditOut(int Disable)
{
  EditOutDisabled=Disable;
}


void Edit::DisableEncode(int Disable)
{
  EditEncodeDisabled=Disable;
}


void Edit::SetTables(struct CharTableSet *TableSet)
{
  Edit::TableSet=TableSet;
};


void Edit::DeleteBlock()
{
  if (SelStart==-1 || SelStart>=SelEnd)
    return;
  memmove(Str+SelStart,Str+SelEnd,StrSize-SelEnd+1);
  StrSize-=SelEnd-SelStart;
  if (CurPos>SelStart)
    if (CurPos<SelEnd)
      CurPos=SelStart;
    else
      CurPos-=SelEnd-SelStart;
  Str=(char *)realloc(Str,StrSize+1);
  SelStart=-1;
  MarkingBlock=FALSE;
}


void Edit::AddColor(struct ColorItem *col)
{
  if ((ColorCount & 15)==0)
    ColorList=(ColorItem *)realloc(ColorList,(ColorCount+16)*sizeof(*ColorList));
  ColorList[ColorCount++]=*col;
}


int Edit::DeleteColor(int ColorPos)
{
  if (ColorCount==0)
    return(FALSE);
  int Dest=0;
  for (int Src=0;Src<ColorCount;Src++)
    if (ColorPos!=-1 && ColorList[Src].StartPos!=ColorPos)
    {
      if (Dest!=Src)
        ColorList[Dest]=ColorList[Src];
      Dest++;
    }
  int DelCount=ColorCount-Dest;
  ColorCount=Dest;
  if (ColorCount==0)
  {
    delete ColorList;
    ColorList=NULL;
  }
  return(DelCount!=0);
}


int Edit::GetColor(struct ColorItem *col,int Item)
{
  if (Item>=ColorCount)
    return(FALSE);
  *col=ColorList[Item];
  return(TRUE);
}


void Edit::ApplyColor()
{
  for (int Col=0;Col<ColorCount;Col++)
  {
    struct ColorItem *CurItem=ColorList+Col;
    int Start=RealPosToTab(CurItem->StartPos)-LeftPos;
    int End=RealPosToTab(CurItem->EndPos)-LeftPos;
    CHAR_INFO TextData[1024];
    if (Start<=X2 && End>=X1)
    {
      if (Start<X1)
        Start=X1;
      if (End>X2)
        End=X2;
      int Length=End-Start+1;
      if (Length>0 && Length<sizeof(TextData))
      {
        ScrBuf.Read(Start,Y1,End,Y1,TextData);
        for (int I=0;I<Length;I++)
          if (TextData[I].Attributes!=Palette[SelColor-COL_FIRSTPALETTECOLOR])
            TextData[I].Attributes=CurItem->Color;
        ScrBuf.Write(Start,Y1,TextData,Length);
      }
    }
  }
}

