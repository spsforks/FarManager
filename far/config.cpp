/*
config.cpp

���䨣����

*/

/* Revision: 1.01 29.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
  29.06.2000 SVS
    + �����뢠�� �� ScrollBar ��� Menu
  30.06.2000 SVS
    - ������ ������� �� ࠬ�� :-) � ������� Options|Panel settings
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

void SystemSettings()
{
  static struct DialogData CfgDlgData[]={
    DI_DOUBLEBOX,3,1,52,16,0,0,0,0,(char *)MConfigSystemTitle,
    DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MConfigRO,
    DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MConfigRecycleBin,
    DI_CHECKBOX,5,4,0,0,0,0,0,0,(char *)MConfigSystemCopy,
    DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MConfigCopySharing,
    DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MConfigCreateUppercaseFolders,
    DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MConfigInactivity,
    DI_FIXEDIT,9,8,11,6,0,0,0,0,"",
    DI_TEXT,13,8,0,0,0,0,0,0,(char *)MConfigInactivityMinutes,
    DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MConfigSaveHistory,
    DI_CHECKBOX,5,10,0,0,0,0,0,0,(char *)MConfigSaveFoldersHistory,
    DI_CHECKBOX,5,11,0,0,0,0,0,0,(char *)MConfigSaveViewHistory,
    DI_CHECKBOX,5,12,0,0,0,0,0,0,(char *)MConfigRegisteredTypes,
    DI_CHECKBOX,5,13,0,0,0,0,0,0,(char *)MConfigAutoSave,
    DI_TEXT,5,14,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_BUTTON,0,15,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,15,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(CfgDlgData,CfgDlg);

  CfgDlg[1].Selected=Opt.ClearReadOnly;
  CfgDlg[2].Selected=Opt.DeleteToRecycleBin;
  CfgDlg[3].Selected=Opt.UseSystemCopy;
  CfgDlg[4].Selected=Opt.CopyOpened;
  CfgDlg[5].Selected=Opt.CreateUppercaseFolders;

  CfgDlg[6].Selected=Opt.InactivityExit;
  sprintf(CfgDlg[7].Data,"%d",Opt.InactivityExitTime);
  CfgDlg[9].Selected=Opt.SaveHistory;
  CfgDlg[10].Selected=Opt.SaveFoldersHistory;
  CfgDlg[11].Selected=Opt.SaveViewHistory;
  CfgDlg[12].Selected=Opt.UseRegisteredTypes;
  CfgDlg[13].Selected=Opt.AutoSaveSetup;
  {
    Dialog Dlg(CfgDlg,sizeof(CfgDlg)/sizeof(CfgDlg[0]));
    Dlg.SetHelp("SystemSettings");
    Dlg.SetPosition(-1,-1,56,18);
    Dlg.Process();
    if (Dlg.GetExitCode()!=15)
      return;
  }

  Opt.ClearReadOnly=CfgDlg[1].Selected;
  Opt.DeleteToRecycleBin=CfgDlg[2].Selected;
  Opt.UseSystemCopy=CfgDlg[3].Selected;
  Opt.CopyOpened=CfgDlg[4].Selected;
  Opt.CreateUppercaseFolders=CfgDlg[5].Selected;
  Opt.InactivityExit=CfgDlg[6].Selected;
  if ((Opt.InactivityExitTime=atoi(CfgDlg[7].Data))<=0)
    Opt.InactivityExit=Opt.InactivityExitTime=0;
  Opt.SaveHistory=CfgDlg[9].Selected;
  Opt.SaveFoldersHistory=CfgDlg[10].Selected;
  Opt.SaveViewHistory=CfgDlg[11].Selected;
  Opt.UseRegisteredTypes=CfgDlg[12].Selected;
  Opt.AutoSaveSetup=CfgDlg[13].Selected;
}


void PanelSettings()
{
  static struct DialogData CfgDlgData[]={
    /* $ 30.06.2000 SVS
       - ������ ������� �� ࠬ�� :-) � ������� Options|Panel settings
    */
    DI_DOUBLEBOX,3,1,52,18,0,0,0,0,(char *)MConfigPanelTitle,
    /* SVS $ */
    DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MConfigHidden,
    DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MConfigHighlight,
    DI_CHECKBOX,5,4,0,0,0,0,0,0,(char *)MConfigAutoChange,
    DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MConfigSelectFolders,
    DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MConfigReverseSort,
    DI_TEXT,3,7,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_CHECKBOX,5,8,0,0,0,0,0,0,(char *)MConfigShowColumns,
    DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MConfigShowStatus,
    DI_CHECKBOX,5,10,0,0,0,0,0,0,(char *)MConfigShowTotal,
    DI_CHECKBOX,5,11,0,0,0,0,0,0,(char *)MConfigShowFree,
    DI_CHECKBOX,5,12,0,0,0,0,0,0,(char *)MConfigShowScrollbar,
    /* $ 29.06.2000
       + �����뢠�� �� ScrollBar ��� Menu
    */
    DI_CHECKBOX,5,13,0,0,0,0,0,0,(char *)MConfigShowMenuScrollbar,
    DI_CHECKBOX,5,14,0,0,0,0,0,0,(char *)MConfigShowScreensNumber,
    DI_CHECKBOX,5,15,0,0,0,0,0,0,(char *)MConfigShowSortMode,
    DI_TEXT,3,16,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_BUTTON,0,17,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,17,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
    /* SVS $ */
  };
  MakeDialogItems(CfgDlgData,CfgDlg);

  CfgDlg[1].Selected=Opt.ShowHidden;
  CfgDlg[2].Selected=Opt.Highlight;
  CfgDlg[3].Selected=Opt.AutoChangeFolder;
  CfgDlg[4].Selected=Opt.SelectFolders;
  CfgDlg[5].Selected=Opt.ReverseSort;

  CfgDlg[7].Selected=Opt.ShowColumnTitles;
  CfgDlg[8].Selected=Opt.ShowPanelStatus;
  CfgDlg[9].Selected=Opt.ShowPanelTotals;
  CfgDlg[10].Selected=Opt.ShowPanelFree;
  CfgDlg[11].Selected=Opt.ShowPanelScrollbar;
  /* $ 29.06.2000 SVS
     + �����뢠�� �� ScrollBar ��� Menu
  */
  CfgDlg[12].Selected=Opt.ShowMenuScrollbar;
  CfgDlg[13].Selected=Opt.ShowScreensNumber;
  CfgDlg[14].Selected=Opt.ShowSortMode;

  {
    Dialog Dlg(CfgDlg,sizeof(CfgDlg)/sizeof(CfgDlg[0]));
    Dlg.SetHelp("PanelSettings");
    Dlg.SetPosition(-1,-1,56,20);
    Dlg.Process();
    if (Dlg.GetExitCode()!=16)
      return;
  }

  Opt.ShowHidden=CfgDlg[1].Selected;
  Opt.Highlight=CfgDlg[2].Selected;
  Opt.AutoChangeFolder=CfgDlg[3].Selected;
  Opt.SelectFolders=CfgDlg[4].Selected;
  Opt.ReverseSort=CfgDlg[5].Selected;

  Opt.ShowColumnTitles=CfgDlg[7].Selected;
  Opt.ShowPanelStatus=CfgDlg[8].Selected;
  Opt.ShowPanelTotals=CfgDlg[9].Selected;
  Opt.ShowPanelFree=CfgDlg[10].Selected;
  Opt.ShowPanelScrollbar=CfgDlg[11].Selected;
  Opt.ShowMenuScrollbar=CfgDlg[12].Selected;
  Opt.ShowScreensNumber=CfgDlg[13].Selected;
  Opt.ShowSortMode=CfgDlg[14].Selected;
  /* SVS $ */

  CtrlObject->LeftPanel->Update(UPDATE_KEEP_SELECTION);
  CtrlObject->RightPanel->Update(UPDATE_KEEP_SELECTION);
  CtrlObject->SetScreenPositions();
}


void InterfaceSettings()
{
  static struct DialogData CfgDlgData[]={
    DI_DOUBLEBOX,3,1,52,16,0,0,0,0,(char *)MConfigInterfaceTitle,
    DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MConfigClock,
    DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MConfigViewerEditorClock,
    DI_CHECKBOX,5,4,0,0,0,0,0,0,(char *)MConfigMouse,
    DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MConfigKeyBar,
    DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MConfigMenuBar,
    DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MConfigSaver,
    DI_FIXEDIT,9,8,11,5,0,0,0,0,"",
    DI_TEXT,13,8,0,0,0,0,0,0,(char *)MConfigSaverMinutes,
    DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MConfigDialogsEditHistory,
    DI_CHECKBOX,5,10,0,0,0,0,0,0,(char *)MConfigUsePromptFormat,
    DI_EDIT,9,11,24,10,0,0,0,0,"",
    DI_CHECKBOX,5,12,0,0,0,0,0,0,(char *)MConfigAltGr,
    DI_CHECKBOX,5,13,0,0,0,0,0,0,(char *)MConfigCopyTotal,
    DI_TEXT,3,14,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_BUTTON,0,15,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,15,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(CfgDlgData,CfgDlg);

  if (!RegVer)
  {
    CfgDlg[2].Type=DI_TEXT;
    sprintf(CfgDlg[2].Data," *  %s",MSG(MRegOnlyShort));
  }

  CfgDlg[1].Selected=Opt.Clock;
  CfgDlg[2].Selected=Opt.ViewerEditorClock;
  CfgDlg[3].Selected=Opt.Mouse;
  CfgDlg[4].Selected=Opt.ShowKeyBar;
  CfgDlg[5].Selected=Opt.ShowMenuBar;
  CfgDlg[6].Selected=Opt.ScreenSaver;
  sprintf(CfgDlg[7].Data,"%d",Opt.ScreenSaverTime);
  CfgDlg[9].Selected=Opt.DialogsEditHistory;
  CfgDlg[10].Selected=Opt.UsePromptFormat;
  strcpy(CfgDlg[11].Data,Opt.PromptFormat);
  CfgDlg[12].Selected=Opt.AltGr;
  CfgDlg[13].Selected=Opt.CopyShowTotal;

  {
    Dialog Dlg(CfgDlg,sizeof(CfgDlg)/sizeof(CfgDlg[0]));
    Dlg.SetHelp("InterfSettings");
    Dlg.SetPosition(-1,-1,56,18);
    Dlg.Process();
    if (Dlg.GetExitCode()!=15)
      return;
  }

  Opt.Clock=CfgDlg[1].Selected;
  Opt.ViewerEditorClock=CfgDlg[2].Selected;
  Opt.Mouse=CfgDlg[3].Selected;
  Opt.ShowKeyBar=CfgDlg[4].Selected;
  Opt.ShowMenuBar=CfgDlg[5].Selected;
  Opt.ScreenSaver=CfgDlg[6].Selected;
  Opt.DialogsEditHistory=CfgDlg[9].Selected;
  if ((Opt.ScreenSaverTime=atoi(CfgDlg[7].Data))<=0)
    Opt.ScreenSaver=Opt.ScreenSaverTime=0;
  Opt.UsePromptFormat=CfgDlg[10].Selected;
  strncpy(Opt.PromptFormat,CfgDlg[11].Data,sizeof(Opt.PromptFormat));
  Opt.AltGr=CfgDlg[12].Selected;
  Opt.CopyShowTotal=CfgDlg[13].Selected;
  SetFarConsoleMode();
  CtrlObject->LeftPanel->Update(UPDATE_KEEP_SELECTION);
  CtrlObject->RightPanel->Update(UPDATE_KEEP_SELECTION);
  CtrlObject->SetScreenPositions();
}


void SetConfirmations()
{
  static struct DialogData ConfDlgData[]={
    DI_DOUBLEBOX,3,1,41,10,0,0,0,0,(char *)MSetConfirmTitle,
    DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MSetConfirmCopy,
    DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MSetConfirmMove,
    DI_CHECKBOX,5,4,0,0,0,0,0,0,(char *)MSetConfirmDrag,
    DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MSetConfirmDelete,
    DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MSetConfirmDeleteFolders,
    DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MSetConfirmExit,
    DI_TEXT,3,8,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_BUTTON,0,9,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,9,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(ConfDlgData,ConfDlg);
  ConfDlg[1].Selected=Opt.Confirm.Copy;
  ConfDlg[2].Selected=Opt.Confirm.Move;
  ConfDlg[3].Selected=Opt.Confirm.Drag;
  ConfDlg[4].Selected=Opt.Confirm.Delete;
  ConfDlg[5].Selected=Opt.Confirm.DeleteFolder;
  ConfDlg[6].Selected=Opt.Confirm.Exit;
  Dialog Dlg(ConfDlg,sizeof(ConfDlg)/sizeof(ConfDlg[0]));
  Dlg.SetHelp("ConfirmDlg");
  Dlg.SetPosition(-1,-1,45,12);
  Dlg.Process();
  if (Dlg.GetExitCode()!=8)
    return;
  Opt.Confirm.Copy=ConfDlg[1].Selected;
  Opt.Confirm.Move=ConfDlg[2].Selected;
  Opt.Confirm.Drag=ConfDlg[3].Selected;
  Opt.Confirm.Delete=ConfDlg[4].Selected;
  Opt.Confirm.DeleteFolder=ConfDlg[5].Selected;
  Opt.Confirm.Exit=ConfDlg[6].Selected;
}


void SetDizConfig()
{
  static struct DialogData DizDlgData[]=
  {
    DI_DOUBLEBOX,3,1,72,13,0,0,0,0,(char *)MCfgDizTitle,
    DI_TEXT,5,2,0,0,0,0,0,0,(char *)MCfgDizListNames,
    DI_EDIT,5,3,70,3,1,0,0,0,"",
    DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MCfgDizSetHidden,
    DI_FIXEDIT,5,6,7,6,0,0,0,0,"",
    DI_TEXT,9,6,0,0,0,0,0,0,(char *)MCfgDizStartPos,
    DI_TEXT,3,7,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_RADIOBUTTON,5,8,0,0,0,0,DIF_GROUP,0,(char *)MCfgDizNotUpdate,
    DI_RADIOBUTTON,5,9,0,0,0,0,0,0,(char *)MCfgDizUpdateIfDisplayed,
    DI_RADIOBUTTON,5,10,0,0,0,0,0,0,(char *)MCfgDizAlwaysUpdate,
    DI_TEXT,3,11,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_BUTTON,0,12,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,12,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(DizDlgData,DizDlg);
  Dialog Dlg(DizDlg,sizeof(DizDlg)/sizeof(DizDlg[0]));
  Dlg.SetPosition(-1,-1,76,15);
  Dlg.SetHelp("FileDiz");
  strcpy(DizDlg[2].Data,Opt.Diz.ListNames);
  if (Opt.Diz.UpdateMode==DIZ_NOT_UPDATE)
    DizDlg[8].Selected=TRUE;
  else
    if (Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED)
      DizDlg[9].Selected=TRUE;
    else
      DizDlg[10].Selected=TRUE;
  DizDlg[4].Selected=Opt.Diz.SetHidden;
  sprintf(DizDlg[5].Data,"%d",Opt.Diz.StartPos);

  Dlg.Process();
  if (Dlg.GetExitCode()!=12)
    return;
  strcpy(Opt.Diz.ListNames,DizDlg[2].Data);
  if (DizDlg[8].Selected)
    Opt.Diz.UpdateMode=DIZ_NOT_UPDATE;
  else
    if (DizDlg[9].Selected)
      Opt.Diz.UpdateMode=DIZ_UPDATE_IF_DISPLAYED;
    else
      Opt.Diz.UpdateMode=DIZ_UPDATE_ALWAYS;
  Opt.Diz.SetHidden=DizDlg[4].Selected;
  Opt.Diz.StartPos=atoi(DizDlg[5].Data);
}


void ViewerConfig()
{
  static struct DialogData CfgDlgData[]={
    DI_DOUBLEBOX,3,1,47,14,0,0,0,0,(char *)MViewConfigTitle,
    DI_SINGLEBOX,5,2,45,7,0,0,DIF_LEFTTEXT,0,(char *)MViewConfigExternal,
    DI_RADIOBUTTON,7,3,0,0,1,0,DIF_GROUP,0,(char *)MViewConfigExternalF3,
    DI_RADIOBUTTON,7,4,0,0,0,0,0,0,(char *)MViewConfigExternalAltF3,
    DI_TEXT,7,5,0,0,0,0,0,0,(char *)MViewConfigExternalCommand,
    DI_EDIT,7,6,43,6,0,0,0,0,"",
    DI_SINGLEBOX,5,8,45,12,0,0,DIF_LEFTTEXT,0,(char *)MViewConfigInternal,
    DI_CHECKBOX,7,9,0,0,0,0,0,0,(char *)MViewConfigSavePos,
    DI_CHECKBOX,7,10,0,0,0,0,0,0,(char *)MViewAutoDetectTable,
    DI_FIXEDIT,7,11,9,15,0,0,0,0,"",
    DI_TEXT,11,11,0,0,0,0,0,0,(char *)MViewConfigTabSize,
    DI_BUTTON,0,13,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,13,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(CfgDlgData,CfgDlg);

  CfgDlg[2].Selected=Opt.UseExternalViewer;
  CfgDlg[3].Selected=!Opt.UseExternalViewer;
  strcpy(CfgDlg[5].Data,Opt.ExternalViewer);
  CfgDlg[7].Selected=Opt.SaveViewerPos;
  CfgDlg[8].Selected=Opt.ViewerAutoDetectTable;
  sprintf(CfgDlg[9].Data,"%d",Opt.ViewTabSize);

  if (!RegVer)
  {
    CfgDlg[9].Type=CfgDlg[10].Type=DI_TEXT;
    sprintf(CfgDlg[9].Data," *  %s",MSG(MRegOnlyShort));
    *CfgDlg[10].Data=0;
  }

  {
    Dialog Dlg(CfgDlg,sizeof(CfgDlg)/sizeof(CfgDlg[0]));
    Dlg.SetHelp("ViewerSettings");
    Dlg.SetPosition(-1,-1,51,16);
    Dlg.Process();
    if (Dlg.GetExitCode()!=11)
      return;
  }

  Opt.UseExternalViewer=CfgDlg[2].Selected;
  strcpy(Opt.ExternalViewer,CfgDlg[5].Data);
  Opt.SaveViewerPos=CfgDlg[7].Selected;
  Opt.ViewerAutoDetectTable=CfgDlg[8].Selected;
  Opt.ViewTabSize=atoi(CfgDlg[9].Data);
  if (Opt.ViewTabSize<1 || Opt.ViewTabSize>512)
    Opt.ViewTabSize=8;
}


void EditorConfig()
{
  static struct DialogData CfgDlgData[]={
    DI_DOUBLEBOX,3,1,47,19,0,0,0,0,(char *)MEditConfigTitle,
    DI_SINGLEBOX,5,2,45,7,0,0,DIF_LEFTTEXT,0,(char *)MEditConfigExternal,
    DI_RADIOBUTTON,7,3,0,0,1,0,DIF_GROUP,0,(char *)MEditConfigEditorF4,
    DI_RADIOBUTTON,7,4,0,0,0,0,0,0,(char *)MEditConfigEditorAltF4,
    DI_TEXT,7,5,0,0,0,0,0,0,(char *)MEditConfigEditorCommand,
    DI_EDIT,7,6,43,6,0,0,0,0,"",
    DI_SINGLEBOX,5,8,45,17,0,0,DIF_LEFTTEXT,0,(char *)MEditConfigInternal,
    DI_CHECKBOX,7,9,0,0,0,0,0,0,(char *)MEditConfigTabsToSpaces,
    DI_CHECKBOX,7,10,0,0,0,0,0,0,(char *)MEditConfigPersistentBlocks,
    DI_CHECKBOX,7,11,0,0,0,0,0,0,(char *)MEditConfigDelRemovesBlocks,
    DI_CHECKBOX,7,12,0,0,0,0,0,0,(char *)MEditConfigAutoIndent,
    DI_CHECKBOX,7,13,0,0,0,0,0,0,(char *)MEditConfigSavePos,
    DI_CHECKBOX,7,14,0,0,0,0,0,0,(char *)MEditAutoDetectTable,
    DI_FIXEDIT,7,15,9,15,0,0,0,0,"",
    DI_TEXT,11,15,0,0,0,0,0,0,(char *)MEditConfigTabSize,
    DI_CHECKBOX,7,16,0,0,0,0,0,0,(char *)MEditCursorBeyondEnd,
    DI_BUTTON,0,18,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,18,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(CfgDlgData,CfgDlg);

  CfgDlg[2].Selected=Opt.UseExternalEditor;
  CfgDlg[3].Selected=!Opt.UseExternalEditor;
  strcpy(CfgDlg[5].Data,Opt.ExternalEditor);
  CfgDlg[7].Selected=Opt.EditorExpandTabs;
  CfgDlg[8].Selected=Opt.EditorPersistentBlocks;
  CfgDlg[9].Selected=Opt.EditorDelRemovesBlocks;
  CfgDlg[10].Selected=Opt.EditorAutoIndent;
  CfgDlg[11].Selected=Opt.SaveEditorPos;
  CfgDlg[12].Selected=Opt.EditorAutoDetectTable;
  sprintf(CfgDlg[13].Data,"%d",Opt.TabSize);
  CfgDlg[15].Selected=Opt.EditorCursorBeyondEOL;

  if (!RegVer)
  {
    CfgDlg[13].Type=CfgDlg[14].Type=DI_TEXT;
    sprintf(CfgDlg[13].Data," *  %s",MSG(MRegOnlyShort));
    *CfgDlg[14].Data=0;
  }

  {
    Dialog Dlg(CfgDlg,sizeof(CfgDlg)/sizeof(CfgDlg[0]));
    Dlg.SetHelp("EditorSettings");
    Dlg.SetPosition(-1,-1,51,21);
    Dlg.Process();
    if (Dlg.GetExitCode()!=16)
      return;
  }

  Opt.UseExternalEditor=CfgDlg[2].Selected;
  strcpy(Opt.ExternalEditor,CfgDlg[5].Data);
  Opt.EditorExpandTabs=CfgDlg[7].Selected;
  Opt.EditorPersistentBlocks=CfgDlg[8].Selected;
  Opt.EditorDelRemovesBlocks=CfgDlg[9].Selected;
  Opt.EditorAutoIndent=CfgDlg[10].Selected;
  Opt.SaveEditorPos=CfgDlg[11].Selected;
  Opt.EditorAutoDetectTable=CfgDlg[12].Selected;
  Opt.TabSize=atoi(CfgDlg[13].Data);
  if (Opt.TabSize<1 || Opt.TabSize>512)
    Opt.TabSize=8;
  Opt.EditorCursorBeyondEOL=CfgDlg[15].Selected;
}


void SetFolderInfoFiles()
{
  GetString(MSG(MSetFolderInfoTitle),MSG(MSetFolderInfoNames),NULL,Opt.FolderInfoFiles,Opt.FolderInfoFiles,sizeof(Opt.FolderInfoFiles),"OptMenu",TRUE);
}


void ReadConfig()
{
  //                                                    �뫮 sizeof(Palette)
  GetRegKey("Colors","CurrentPalette",Palette,DefaultPalette,SizeArrayPalette);

  GetRegKey("Screen","Clock",Opt.Clock,1);
  GetRegKey("Screen","ViewerEditorClock",Opt.ViewerEditorClock,0);
  GetRegKey("Screen","KeyBar",Opt.ShowKeyBar,1);
  GetRegKey("Screen","ScreenSaver",Opt.ScreenSaver,1);
  GetRegKey("Screen","ScreenSaverTime",Opt.ScreenSaverTime,5);
  GetRegKey("Screen","UsePromptFormat",Opt.UsePromptFormat,0);
  GetRegKey("Screen","PromptFormat",Opt.PromptFormat,"$p>",sizeof(Opt.PromptFormat));

  GetRegKey("Interface","DialogsEditHistory",Opt.DialogsEditHistory,1);
  GetRegKey("Interface","Mouse",Opt.Mouse,1);
  GetRegKey("Interface","AltGr",Opt.AltGr,1);
  GetRegKey("Interface","CopyShowTotal",Opt.CopyShowTotal,0);

  GetRegKey("Viewer","ExternalViewerName",Opt.ExternalViewer,"",sizeof(Opt.ExternalViewer));
  GetRegKey("Viewer","UseExternalViewer",Opt.UseExternalViewer,0);
  GetRegKey("Viewer","SaveViewerPos",Opt.SaveViewerPos,0);
  GetRegKey("Viewer","AutoDetectTable",Opt.ViewerAutoDetectTable,0);
  GetRegKey("Viewer","TabSize",Opt.ViewTabSize,8);

  GetRegKey("Editor","ExternalEditorName",Opt.ExternalEditor,"",sizeof(Opt.ExternalEditor));
  GetRegKey("Editor","UseExternalEditor",Opt.UseExternalEditor,0);
  GetRegKey("Editor","ExpandTabs",Opt.EditorExpandTabs,0);
  GetRegKey("Editor","TabSize",Opt.TabSize,8);
  GetRegKey("Editor","PersistentBlocks",Opt.EditorPersistentBlocks,1);
  GetRegKey("Editor","DelRemovesBlocks",Opt.EditorDelRemovesBlocks,0);
  GetRegKey("Editor","AutoIndent",Opt.EditorAutoIndent,0);
  GetRegKey("Editor","SaveEditorPos",Opt.SaveEditorPos,0);
  GetRegKey("Editor","AutoDetectTable",Opt.EditorAutoDetectTable,0);
  GetRegKey("Editor","EditorCursorBeyondEOL",Opt.EditorCursorBeyondEOL,1);

  GetRegKey("System","SaveHistory",Opt.SaveHistory,0);
  GetRegKey("System","SaveFoldersHistory",Opt.SaveFoldersHistory,0);
  GetRegKey("System","SaveViewHistory",Opt.SaveViewHistory,0);
  GetRegKey("System","UseRegisteredTypes",Opt.UseRegisteredTypes,1);
  GetRegKey("System","AutoSaveSetup",Opt.AutoSaveSetup,0);
  GetRegKey("System","ClearReadOnly",Opt.ClearReadOnly,0);
  GetRegKey("System","DeleteToRecycleBin",Opt.DeleteToRecycleBin,1);
  GetRegKey("System","UseSystemCopy",Opt.UseSystemCopy,0);
  GetRegKey("System","CopyOpened",Opt.CopyOpened,0);
  GetRegKey("System","CreateUppercaseFolders",Opt.CreateUppercaseFolders,0);
  GetRegKey("System","InactivityExit",Opt.InactivityExit,0);
  GetRegKey("System","InactivityExitTime",Opt.InactivityExitTime,15);
  GetRegKey("System","DriveMenuMode",Opt.ChangeDriveMode,DRIVE_SHOW_TYPE|DRIVE_SHOW_PLUGINS);
  GetRegKey("System","FileSearchMode",Opt.FileSearchMode,SEARCH_ROOT);
  GetRegKey("System","FolderInfo",Opt.FolderInfoFiles,"DirInfo,File_Id.diz,Descript.ion,ReadMe,Read.Me,ReadMe.txt,ReadMe.*",sizeof(Opt.FolderInfoFiles));

  GetRegKey("Language","Main",Opt.Language,"English",sizeof(Opt.Language));
  GetRegKey("Language","Help",Opt.HelpLanguage,"English",sizeof(Opt.HelpLanguage));

  GetRegKey("Confirmations","Copy",Opt.Confirm.Copy,1);
  GetRegKey("Confirmations","Move",Opt.Confirm.Move,1);
  GetRegKey("Confirmations","Drag",Opt.Confirm.Drag,1);
  GetRegKey("Confirmations","Delete",Opt.Confirm.Delete,1);
  GetRegKey("Confirmations","DeleteFolder",Opt.Confirm.DeleteFolder,1);
  GetRegKey("Confirmations","Exit",Opt.Confirm.Exit,1);

  GetRegKey("Panel","ShowHidden",Opt.ShowHidden,1);
  GetRegKey("Panel","Highlight",Opt.Highlight,1);
  GetRegKey("Panel","AutoChangeFolder",Opt.AutoChangeFolder,0);
  GetRegKey("Panel","SelectFolders",Opt.SelectFolders,0);
  GetRegKey("Panel","ReverseSort",Opt.ReverseSort,1);

  GetRegKey("Panel\\Left","Type",Opt.LeftPanel.Type,0);
  GetRegKey("Panel\\Left","Visible",Opt.LeftPanel.Visible,1);
  GetRegKey("Panel\\Left","Focus",Opt.LeftPanel.Focus,1);
  GetRegKey("Panel\\Left","ViewMode",Opt.LeftPanel.ViewMode,2);
  GetRegKey("Panel\\Left","SortMode",Opt.LeftPanel.SortMode,1);
  GetRegKey("Panel\\Left","SortOrder",Opt.LeftPanel.SortOrder,1);
  GetRegKey("Panel\\Left","SortGroups",Opt.LeftPanel.SortGroups,0);
  GetRegKey("Panel\\Left","ShortNames",Opt.LeftPanel.ShowShortNames,0);
  GetRegKey("Panel\\Left","Folder",Opt.LeftFolder,"",sizeof(Opt.LeftFolder));

  GetRegKey("Panel\\Right","Type",Opt.RightPanel.Type,0);
  GetRegKey("Panel\\Right","Visible",Opt.RightPanel.Visible,1);
  GetRegKey("Panel\\Right","Focus",Opt.RightPanel.Focus,0);
  GetRegKey("Panel\\Right","ViewMode",Opt.RightPanel.ViewMode,2);
  GetRegKey("Panel\\Right","SortMode",Opt.RightPanel.SortMode,1);
  GetRegKey("Panel\\Right","SortOrder",Opt.RightPanel.SortOrder,1);
  GetRegKey("Panel\\Right","SortGroups",Opt.RightPanel.SortGroups,0);
  GetRegKey("Panel\\Right","ShortNames",Opt.RightPanel.ShowShortNames,0);
  GetRegKey("Panel\\Right","Folder",Opt.RightFolder,"",sizeof(Opt.RightFolder));

  GetRegKey("Panel\\Layout","ColumnTitles",Opt.ShowColumnTitles,1);
  GetRegKey("Panel\\Layout","StatusLine",Opt.ShowPanelStatus,1);
  GetRegKey("Panel\\Layout","TotalInfo",Opt.ShowPanelTotals,1);
  GetRegKey("Panel\\Layout","FreeInfo",Opt.ShowPanelFree,0);
  GetRegKey("Panel\\Layout","Scrollbar",Opt.ShowPanelScrollbar,0);
  /* $ 29.06.2000 SVS
     + �����뢠�� �� ScrollBar ��� Menu
  */
  GetRegKey("Panel\\Layout","ScrollbarMenu",Opt.ShowMenuScrollbar,0);
  /* SVS $ */
  GetRegKey("Panel\\Layout","ScreensNumber",Opt.ShowScreensNumber,1);
  GetRegKey("Panel\\Layout","SortMode",Opt.ShowSortMode,1);

  GetRegKey("Layout","HeightDecrement",Opt.HeightDecrement,0);
  GetRegKey("Layout","WidthDecrement",Opt.WidthDecrement,0);
  GetRegKey("Layout","ShowMenuBar",Opt.ShowMenuBar,0);
  Help::SetFullScreenMode(GetRegKey("Layout","FullscreenHelp",0));

  GetRegKey("Layout","PassiveFolder",Opt.PassiveFolder,"",sizeof(Opt.PassiveFolder));

  GetRegKey("Descriptions","ListNames",Opt.Diz.ListNames,"Descript.ion,Files.bbs",sizeof(Opt.Diz.ListNames));
  GetRegKey("Descriptions","UpdateMode",Opt.Diz.UpdateMode,DIZ_UPDATE_IF_DISPLAYED);
  GetRegKey("Descriptions","SetHidden",Opt.Diz.SetHidden,TRUE);
  GetRegKey("Descriptions","StartPos",Opt.Diz.StartPos,0);

  FileList::ReadPanelModes();
  GetTempPath(sizeof(Opt.TempPath),Opt.TempPath);
  RemoveTrailingSpaces(Opt.TempPath);
  AddEndSlash(Opt.TempPath);
  CtrlObject->EditorPosCache.Read("Editor\\LastPositions");
  CtrlObject->ViewerPosCache.Read("Viewer\\LastPositions");
  if (Opt.TabSize<1 || Opt.TabSize>512)
    Opt.TabSize=8;
  if (Opt.ViewTabSize<1 || Opt.ViewTabSize>512)
    Opt.ViewTabSize=8;
}


void SaveConfig(int Ask)
{
  char OutText[NM];

  if (Ask && Message(0,2,MSG(MSaveSetupTitle),MSG(MSaveSetupAsk1),MSG(MSaveSetupAsk2),MSG(MSaveSetup),MSG(MCancel))!=0)
    return;

  //                                       �뫮 sizeof(Palette)
  SetRegKey("Colors","CurrentPalette",Palette,SizeArrayPalette);
  SetRegKey("Screen","Clock",Opt.Clock);
  SetRegKey("Screen","ViewerEditorClock",Opt.ViewerEditorClock);
  SetRegKey("Screen","KeyBar",Opt.ShowKeyBar);
  SetRegKey("Screen","ScreenSaver",Opt.ScreenSaver);
  SetRegKey("Screen","ScreenSaverTime",Opt.ScreenSaverTime);
  SetRegKey("Screen","UsePromptFormat",Opt.UsePromptFormat);
  SetRegKey("Screen","PromptFormat",Opt.PromptFormat);

  SetRegKey("Interface","DialogsEditHistory",Opt.DialogsEditHistory);
  SetRegKey("Interface","Mouse",Opt.Mouse);
  SetRegKey("Interface","AltGr",Opt.AltGr);
  SetRegKey("Interface","CopyShowTotal",Opt.CopyShowTotal);

  SetRegKey("Viewer","ExternalViewerName",Opt.ExternalViewer);
  SetRegKey("Viewer","UseExternalViewer",Opt.UseExternalViewer);
  SetRegKey("Viewer","SaveViewerPos",Opt.SaveViewerPos);
  SetRegKey("Viewer","AutoDetectTable",Opt.ViewerAutoDetectTable);
  SetRegKey("Viewer","TabSize",Opt.ViewTabSize);

  SetRegKey("Editor","ExternalEditorName",Opt.ExternalEditor);
  SetRegKey("Editor","UseExternalEditor",Opt.UseExternalEditor);
  SetRegKey("Editor","ExpandTabs",Opt.EditorExpandTabs);
  SetRegKey("Editor","TabSize",Opt.TabSize);
  SetRegKey("Editor","PersistentBlocks",Opt.EditorPersistentBlocks);
  SetRegKey("Editor","DelRemovesBlocks",Opt.EditorDelRemovesBlocks);
  SetRegKey("Editor","AutoIndent",Opt.EditorAutoIndent);
  SetRegKey("Editor","SaveEditorPos",Opt.SaveEditorPos);
  SetRegKey("Editor","AutoDetectTable",Opt.EditorAutoDetectTable);
  SetRegKey("Editor","EditorCursorBeyondEOL",Opt.EditorCursorBeyondEOL);

  SetRegKey("System","SaveHistory",Opt.SaveHistory);
  SetRegKey("System","SaveFoldersHistory",Opt.SaveFoldersHistory);
  SetRegKey("System","SaveViewHistory",Opt.SaveViewHistory);
  SetRegKey("System","UseRegisteredTypes",Opt.UseRegisteredTypes);
  SetRegKey("System","AutoSaveSetup",Opt.AutoSaveSetup);
  SetRegKey("System","ClearReadOnly",Opt.ClearReadOnly);
  SetRegKey("System","DeleteToRecycleBin",Opt.DeleteToRecycleBin);
  SetRegKey("System","UseSystemCopy",Opt.UseSystemCopy);
  SetRegKey("System","CopyOpened",Opt.CopyOpened);
  SetRegKey("System","CreateUppercaseFolders",Opt.CreateUppercaseFolders);
  SetRegKey("System","InactivityExit",Opt.InactivityExit);
  SetRegKey("System","InactivityExitTime",Opt.InactivityExitTime);
  SetRegKey("System","DriveMenuMode",Opt.ChangeDriveMode);
  SetRegKey("System","FileSearchMode",Opt.FileSearchMode);
  SetRegKey("System","FolderInfo",Opt.FolderInfoFiles);

  SetRegKey("Language","Main",Opt.Language);
  SetRegKey("Language","Help",Opt.HelpLanguage);

  SetRegKey("Confirmations","Copy",Opt.Confirm.Copy);
  SetRegKey("Confirmations","Move",Opt.Confirm.Move);
  SetRegKey("Confirmations","Drag",Opt.Confirm.Drag);
  SetRegKey("Confirmations","Delete",Opt.Confirm.Delete);
  SetRegKey("Confirmations","DeleteFolder",Opt.Confirm.DeleteFolder);
  SetRegKey("Confirmations","Exit",Opt.Confirm.Exit);

  SetRegKey("Panel","ShowHidden",Opt.ShowHidden);
  SetRegKey("Panel","Highlight",Opt.Highlight);
  SetRegKey("Panel","AutoChangeFolder",Opt.AutoChangeFolder);
  SetRegKey("Panel","SelectFolders",Opt.SelectFolders);
  SetRegKey("Panel","ReverseSort",Opt.ReverseSort);

  Panel *LeftPanel=CtrlObject->LeftPanel;
  SetRegKey("Panel\\Left","Visible",LeftPanel->IsVisible());
  SetRegKey("Panel\\Left","Focus",LeftPanel->GetFocus());
  if (LeftPanel->GetMode()==NORMAL_PANEL)
  {
    SetRegKey("Panel\\Left","Type",LeftPanel->GetType());
    SetRegKey("Panel\\Left","ViewMode",LeftPanel->GetViewMode());
    SetRegKey("Panel\\Left","SortMode",LeftPanel->GetSortMode());
    SetRegKey("Panel\\Left","SortOrder",LeftPanel->GetSortOrder());
    SetRegKey("Panel\\Left","SortGroups",LeftPanel->GetSortGroups());
    SetRegKey("Panel\\Left","ShortNames",LeftPanel->GetShowShortNamesMode());
    LeftPanel->GetCurDir(OutText);
    SetRegKey("Panel\\Left","Folder",OutText);
  }

  Panel *RightPanel=CtrlObject->RightPanel;
  SetRegKey("Panel\\Right","Visible",RightPanel->IsVisible());
  SetRegKey("Panel\\Right","Focus",RightPanel->GetFocus());
  if (RightPanel->GetMode()==NORMAL_PANEL)
  {
    SetRegKey("Panel\\Right","Type",RightPanel->GetType());
    SetRegKey("Panel\\Right","ViewMode",RightPanel->GetViewMode());
    SetRegKey("Panel\\Right","SortMode",RightPanel->GetSortMode());
    SetRegKey("Panel\\Right","SortOrder",RightPanel->GetSortOrder());
    SetRegKey("Panel\\Right","SortGroups",RightPanel->GetSortGroups());
    SetRegKey("Panel\\Right","ShortNames",RightPanel->GetShowShortNamesMode());
    RightPanel->GetCurDir(OutText);
    SetRegKey("Panel\\Right","Folder",OutText);
  }

  SetRegKey("Panel\\Layout","ColumnTitles",Opt.ShowColumnTitles);
  SetRegKey("Panel\\Layout","StatusLine",Opt.ShowPanelStatus);
  SetRegKey("Panel\\Layout","TotalInfo",Opt.ShowPanelTotals);
  SetRegKey("Panel\\Layout","FreeInfo",Opt.ShowPanelFree);
  SetRegKey("Panel\\Layout","Scrollbar",Opt.ShowPanelScrollbar);
  /* $ 29.06.2000 SVS
     + �����뢠�� �� ScrollBar ��� Menu
  */
  SetRegKey("Panel\\Layout","ScrollbarMenu",Opt.ShowMenuScrollbar);
  /* SVS $ */
  SetRegKey("Panel\\Layout","ScreensNumber",Opt.ShowScreensNumber);
  SetRegKey("Panel\\Layout","SortMode",Opt.ShowSortMode);

  SetRegKey("Layout","HeightDecrement",Opt.HeightDecrement);
  SetRegKey("Layout","WidthDecrement",Opt.WidthDecrement);
  SetRegKey("Layout","ShowMenuBar",Opt.ShowMenuBar);
  SetRegKey("Layout","FullscreenHelp",Help::GetFullScreenMode());

  CtrlObject->GetAnotherPanel(CtrlObject->ActivePanel)->GetCurDir(OutText);
  SetRegKey("Layout","PassiveFolder",OutText);

  SetRegKey("Descriptions","ListNames",Opt.Diz.ListNames);
  SetRegKey("Descriptions","UpdateMode",Opt.Diz.UpdateMode);
  SetRegKey("Descriptions","SetHidden",Opt.Diz.SetHidden);
  SetRegKey("Descriptions","StartPos",Opt.Diz.StartPos);

  PanelFilter::SaveSelection();
  FileList::SavePanelModes();
  if (Ask)
    CtrlObject->Macro.SaveMacros();
}
