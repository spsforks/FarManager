#define _FAR_NO_NAMELESS_UNIONS
#define _FAR_USE_FARFINDDATA
#include <plugin.hpp>
#include <CRT/crt.hpp>

#ifdef __GNUC__
#define _i64(num) num##ll
#define _ui64(num) num##ull
#else
#define _i64(num) num##i64
#define _ui64(num) num##ui64
#endif

#ifndef UNICODE
#define EXP_NAME(p) _export p
#else
#define EXP_NAME(p) _export p ## W
#endif

#ifndef UNICODE
#define _cFileName            cFileName
#else
#define _cFileName            lpwszFileName
#endif

/****************************************************************************
 * ����� ��� ���������� ��������� startup-���� ��� ���������� ��� GCC
 ****************************************************************************/
#if defined(__GNUC__)
#ifdef __cplusplus
extern "C"{
#endif
  BOOL WINAPI DllMainCRTStartup(HANDLE hDll, DWORD dwReason, LPVOID lpReserved);
#ifdef __cplusplus
};
#endif

BOOL WINAPI DllMainCRTStartup(HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
  (void) lpReserved;
  (void) dwReason;
  (void) hDll;
  return TRUE;
}
#endif

/****************************************************************************
 * ��������� ��� ���������� ����� �� .lng �����
 ****************************************************************************/
enum CompareLng {
  MNoLngStringDefined = -1,

  MOK,
  MCancel,

  MCompare,

  MCmpTitle,
  MProcessBox,
  MProcessSubfolders,
  MUseMaxScanDepth,
  MProcessSelected,
  MCompareBox,
  MCompareTime,
  MCompareLowPrecision,
  MCompareIgnoreTimeZone,
  MCompareSize,
  MCompareContents,
  MCompareContentsIgnore,
  MCompareIgnoreNewLines,
  MCompareIgnoreWhitespace,
  MMessageWhenNoDiff,

  MFilePanelsRequired,

  MComparing,
  MComparingWith,

  MComparingFiles,

  MNoDiffTitle,
  MNoDiffBody,

  MNoMemTitle,
  MNoMemBody,

  MOldFARTitle,
  MOldFARBody,

  MEscTitle,
  MEscBody,
};

/****************************************************************************
 * ������� ��������� �������
 ****************************************************************************/
struct Options {
  int ProcessSubfolders,
      UseMaxScanDepth,
      MaxScanDepth,
      ProcessSelected,
      ProcessHidden,
      CompareTime,
      LowPrecisionTime,
      IgnorePossibleTimeZoneDifferences,
      CompareSize,
      CompareContents,
      CompareContentsIgnore,
      IgnoreWhitespace,
      IgnoreNewLines,
      MessageWhenNoDiff;
} Opt;

/****************************************************************************
 * ����� ����������� �������� FAR
 ****************************************************************************/
static struct PluginStartupInfo Info;
static struct FarStandardFunctions FSF;
static TCHAR  *PluginRootKey = NULL;

static void WFD2FFD(WIN32_FIND_DATA &wfd, FAR_FIND_DATA &ffd)
{
  ffd.dwFileAttributes = wfd.dwFileAttributes;
  ffd.ftCreationTime   = wfd.ftCreationTime;
  ffd.ftLastAccessTime = wfd.ftLastAccessTime;
  ffd.ftLastWriteTime  = wfd.ftLastWriteTime;
#ifndef UNICODE
  ffd.nFileSizeHigh    = wfd.nFileSizeHigh;
  ffd.nFileSizeLow     = wfd.nFileSizeLow;
  ffd.dwReserved0      = wfd.dwReserved0;
  ffd.dwReserved1      = wfd.dwReserved1;
  lstrcpy(ffd.cFileName, wfd.cFileName);
  lstrcpy(ffd.cAlternateFileName, wfd.cAlternateFileName);
#else
  ffd.nFileSize               = ((__int64)wfd.nFileSizeHigh << 32) | wfd.nFileSizeLow;
  ffd.nPackSize               = 0;
  ffd.lpwszFileName           = wcsdup(wfd.cFileName);
  ffd.lpwszAlternateFileName  = wcsdup(wfd.cAlternateFileName);
#endif
}

/****************************************************************************
 * ������ ��������� ������� FAR: ��������� ������ �� .lng-�����
 ****************************************************************************/
static const TCHAR *GetMsg(int CompareLng)
{
  return Info.GetMsg(Info.ModuleNumber, CompareLng);
}


static int iTruncLen;

/****************************************************************************
 * ������� ������ ������� ���� ������ (��� ��������� �������� �����)
 * ��� ����������� ������ � ��������� ���������
 ****************************************************************************/
static void TrunCopy(TCHAR *cpDest, const TCHAR *cpSrc)
{
  int iLen = (int)lstrlen(FSF.TruncStr(lstrcpy(cpDest, cpSrc), iTruncLen));

  if (iLen < iTruncLen)
  {
#ifndef UNICODE
    memset(&cpDest[iLen], ' ', iTruncLen - iLen);
#else
    do cpDest[iLen] = _T(' '); while(++iLen < iTruncLen);
#endif
    cpDest[iTruncLen] = _T('\0');
  }
}

static bool bStart;

/****************************************************************************
 * ���������� ��������� � ��������� ���� ������
 ****************************************************************************/
static void ShowMessage(const TCHAR *Name1, const TCHAR *Name2)
{
  static DWORD dwTicks;
  DWORD dwNewTicks = GetTickCount();
  if (dwNewTicks - dwTicks < 500)
    return;
  dwTicks = dwNewTicks;

  TCHAR TruncName1[NM], TruncName2[NM];
  TrunCopy(TruncName1, Name1);
  TrunCopy(TruncName2, Name2);

  const TCHAR *MsgItems[] = {
    GetMsg(MCmpTitle),
    GetMsg(MComparing),
    TruncName1,
    GetMsg(MComparingWith),
    TruncName2
  };
  Info.Message(Info.ModuleNumber, bStart ? FMSG_LEFTALIGN :
               FMSG_LEFTALIGN|FMSG_KEEPBACKGROUND,
               NULL,
               MsgItems,
               sizeof(MsgItems) / sizeof(MsgItems[0]),
               0);
  bStart = false;
}


/****************************************************************************
 * ���������� ������� ��� ShowDialog
 ****************************************************************************/
LONG_PTR WINAPI ShowDialogProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2)
{
  static int CompareContents,
             CompareContentsIgnore,
             ProcessSubfolders,
             CompareTime;

  switch (Msg)
  {
    case DN_INITDIALOG:
      CompareContents = ((DWORD)Info.SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0)) & 0x000000FF;
      CompareContentsIgnore = CompareContents + 1;
      ProcessSubfolders = (((DWORD)Info.SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0)) >> 8) & 0x000000FF;
      CompareTime = ((DWORD)Info.SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0)) >> 16;
      break;

    case DN_BTNCLICK:
      if (Param1 == CompareTime || Param1 == ProcessSubfolders || Param1 == CompareContents || Param1 == CompareContentsIgnore)
      {
        if (Param2)
        {
          Info.SendDlgMessage(hDlg, DM_ENABLE, Param1+1, TRUE);
          if (!(Param1 == CompareContents && !Info.SendDlgMessage(hDlg, DM_GETCHECK, CompareContentsIgnore, 0)))
          {
            Info.SendDlgMessage(hDlg, DM_ENABLE, Param1+2, TRUE);
            if (Param1 == CompareContents)
              Info.SendDlgMessage(hDlg, DM_ENABLE, Param1+3, TRUE);
          }
        }
        else
        {
          Info.SendDlgMessage(hDlg, DM_ENABLE, Param1+1, FALSE);
          Info.SendDlgMessage(hDlg, DM_ENABLE, Param1+2, FALSE);
          if (Param1 == CompareContents)
            Info.SendDlgMessage(hDlg, DM_ENABLE, Param1+3, FALSE);
        }
      }
      break;
  }

  return Info.DefDlgProc(hDlg, Msg, Param1, Param2);
}

/****************************************************************************
 * ������ ��������� �� �������, ���������� ������ � ������� ���������,
 * ��������� ��������� Opt, ��������� (���� ����) ����� ��������� � �������,
 * ���������� true, ���� ������������ ����� OK
 ****************************************************************************/
static bool ShowDialog(bool bPluginPanels, bool bSelectionPresent)
{
  static struct InitDialogItem {
    unsigned char Type;
    unsigned char X1, Y1, X2, Y2;
    int           Data;
    int           DefaultRegValue;
    TCHAR         *SelectedRegValue;
    unsigned int  Flags;
    int          *StoreTo;
  } InitItems[] = {
    /* 0*/ { DI_DOUBLEBOX,    3,  1, 62, 20, MCmpTitle,                0, NULL,                                    0, NULL },
    /* 1*/ { DI_TEXT,         5,  2,  0,  0, MProcessBox,              0, NULL,                                    0, NULL },
    /* 2*/ { DI_CHECKBOX,     5,  3,  0,  0, MProcessSubfolders,       0, _T("ProcessSubfolders"),                 0, &Opt.ProcessSubfolders },
    /* 3*/ { DI_CHECKBOX,     9,  4,  0,  0, MUseMaxScanDepth,         0, _T("UseMaxScanDepth"),                   0, &Opt.UseMaxScanDepth },
    /* 4*/ { DI_FIXEDIT,      0,  4,  4,  0, MNoLngStringDefined,     99, _T("MaxScanDepth"),           DIF_MASKEDIT, &Opt.MaxScanDepth },
    /* 5*/ { DI_CHECKBOX,     5,  5,  0,  0, MProcessSelected,         0, _T("ProcessSelected"),                   0, &Opt.ProcessSelected },
    /* 6*/ { DI_TEXT,         0,  6,  0,  0, MNoLngStringDefined,      0, NULL,                        DIF_SEPARATOR, NULL },
    /* 7*/ { DI_TEXT,         5,  7,  0,  0, MCompareBox,              0, NULL,                                    0, NULL },
    /* 8*/ { DI_CHECKBOX,     5,  8,  0,  0, MCompareTime,             1, _T("CompareTime"),                       0, &Opt.CompareTime },
    /* 9*/ { DI_CHECKBOX,     9,  9,  0,  0, MCompareLowPrecision,     1, _T("LowPrecisionTime"),                  0, &Opt.LowPrecisionTime },
    /*10*/ { DI_CHECKBOX,     9, 10,  0,  0, MCompareIgnoreTimeZone,   1, _T("IgnorePossibleTimeZoneDifferences"), 0, &Opt.IgnorePossibleTimeZoneDifferences },
    /*11*/ { DI_CHECKBOX,     5, 11,  0,  0, MCompareSize,             1, _T("CompareSize"),                       0, &Opt.CompareSize },
    /*12*/ { DI_CHECKBOX,     5, 12,  0,  0, MCompareContents,         0, _T("CompareContents"),                   0, &Opt.CompareContents },
    /*13*/ { DI_CHECKBOX,     9, 13,  0,  0, MCompareContentsIgnore,   0, _T("CompareContentsIgnore"),             0, &Opt.CompareContentsIgnore },
    /*14*/ { DI_RADIOBUTTON, 13, 14,  0,  0, MCompareIgnoreNewLines,   1, _T("IgnoreNewLines"),            DIF_GROUP, &Opt.IgnoreNewLines },
    /*15*/ { DI_RADIOBUTTON, 13, 15,  0,  0, MCompareIgnoreWhitespace, 0, _T("IgnoreWhitespace"),                  0, &Opt.IgnoreWhitespace },
    /*16*/ { DI_TEXT,         0, 16,  0,  0, MNoLngStringDefined,      0, NULL,                        DIF_SEPARATOR, NULL },
    /*17*/ { DI_CHECKBOX,     5, 17,  0,  0, MMessageWhenNoDiff,       0, _T("MessageWhenNoDiff"),                 0, &Opt.MessageWhenNoDiff },
    /*18*/ { DI_TEXT,         0, 18,  0,  0, MNoLngStringDefined,      0, NULL,                        DIF_SEPARATOR, NULL },
    /*19*/ { DI_BUTTON,       0, 19,  0,  0, MOK,                      0, NULL,                      DIF_CENTERGROUP, NULL },
    /*20*/ { DI_BUTTON,       0, 19,  0,  0, MCancel,                  0, NULL,                      DIF_CENTERGROUP, NULL }
  };
  struct FarDialogItem DialogItems[sizeof(InitItems) / sizeof(InitItems[0])];
  TCHAR Mask[] = _T("99999");
#ifdef UNICODE
  wchar_t tmpnum[sizeof(InitItems)/sizeof(InitItems[0])][32];
#endif

  memset(DialogItems,0,sizeof(DialogItems));

  HKEY hKey;
  if (!PluginRootKey ||
       RegOpenKeyEx(HKEY_CURRENT_USER, PluginRootKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
    hKey = 0;

  size_t DlgData=0;
  bool bNoFocus = true;
  size_t i;
  for (i = 0; i < sizeof(InitItems) / sizeof(InitItems[0]); i++)
  {
    DWORD dwRegValue;
    DWORD dwSize                  = sizeof(DWORD);
    DialogItems[i].Type           = InitItems[i].Type;
    DialogItems[i].X1             = InitItems[i].X1;
    DialogItems[i].Y1             = InitItems[i].Y1;
    DialogItems[i].X2             = InitItems[i].X2;
    DialogItems[i].Y2             = InitItems[i].Y2;
    DialogItems[i].Focus          = FALSE;
    DialogItems[i].Flags          = InitItems[i].Flags;
#ifndef UNICODE
    lstrcpy(DialogItems[i].Data.Data, (InitItems[i].Data == MNoLngStringDefined)
            ? _T("") : GetMsg(InitItems[i].Data));
#else
    DialogItems[i].PtrData = (InitItems[i].Data == MNoLngStringDefined)
            ? _T("") : GetMsg(InitItems[i].Data);
#endif
    dwRegValue = (hKey && InitItems[i].SelectedRegValue &&
                  RegQueryValueEx(hKey, InitItems[i].SelectedRegValue, NULL,
                  NULL, (LPBYTE)&dwRegValue, &dwSize)
                  == ERROR_SUCCESS ) ? dwRegValue : InitItems[i].DefaultRegValue;
    if (DialogItems[i].Type == DI_CHECKBOX || DialogItems[i].Type == DI_RADIOBUTTON)
      DialogItems[i].Param.Selected = dwRegValue;
    else if (DialogItems[i].Type == DI_FIXEDIT)
    {
#ifndef UNICODE
#define _DD Data.Data
#else
      DialogItems[i].PtrData = tmpnum[i];
#define _DD PtrData
#endif
      FSF.itoa(dwRegValue, DialogItems[i]._DD, 10);
      DialogItems[i].Param.Mask = Mask;
      DialogItems[i].X1 = DialogItems[i-1].X1 + lstrlen(DialogItems[i-1]._DD)
                          - (_tcschr(DialogItems[i-1]._DD, _T('&'))?1:0) + 5;
      DialogItems[i].X2 += DialogItems[i].X1;
    }

    switch (InitItems[i].Data)
    {
      case MCompareContents:
        DlgData += i;
        if (bPluginPanels)
        {
          DialogItems[i].Flags |= DIF_DISABLE;
          DialogItems[i].Param.Selected = 0;
        }
        if (!DialogItems[i].Param.Selected)
        {
          InitItems[i+1].Flags |= DIF_DISABLE;
          InitItems[i+2].Flags |= DIF_DISABLE;
          InitItems[i+3].Flags |= DIF_DISABLE;
        }
        else
        {
          InitItems[i+1].Flags &= ~DIF_DISABLE;
          InitItems[i+2].Flags &= ~DIF_DISABLE;
          InitItems[i+3].Flags &= ~DIF_DISABLE;
        }
        break;
      case MCompareContentsIgnore:
        if (!DialogItems[i].Param.Selected || DialogItems[i].Flags & DIF_DISABLE)
        {
          InitItems[i+1].Flags |= DIF_DISABLE;
          InitItems[i+2].Flags |= DIF_DISABLE;
        }
        else
        {
          InitItems[i+1].Flags &= ~DIF_DISABLE;
          InitItems[i+2].Flags &= ~DIF_DISABLE;
        }
        break;
      case MCompareIgnoreWhitespace:
        if (DialogItems[i].Param.Selected == DialogItems[i-1].Param.Selected)
        {
          DialogItems[i-1].Param.Selected = 1;
          DialogItems[i].Param.Selected = 0;
        }
        break;
      case MProcessSubfolders:
        DlgData += i<<8;
        if (bPluginPanels)
        {
          DialogItems[i].Flags |= DIF_DISABLE;
          DialogItems[i].Param.Selected = 0;
        }
        if (!DialogItems[i].Param.Selected)
        {
          InitItems[i+1].Flags |= DIF_DISABLE;
          InitItems[i+2].Flags |= DIF_DISABLE;
        }
        else
        {
          InitItems[i+1].Flags &= ~DIF_DISABLE;
          InitItems[i+2].Flags &= ~DIF_DISABLE;
        }
        break;
      case MProcessSelected:
        if (!bSelectionPresent)
        {
          DialogItems[i].Flags |= DIF_DISABLE;
          DialogItems[i].Param.Selected = 0;
        }
        break;
      case MCompareTime:
        DlgData += i<<16;
        if (!DialogItems[i].Param.Selected)
        {
          InitItems[i+1].Flags |= DIF_DISABLE;
          InitItems[i+2].Flags |= DIF_DISABLE;
        }
        else
        {
          InitItems[i+1].Flags &= ~DIF_DISABLE;
          InitItems[i+2].Flags &= ~DIF_DISABLE;
        }
        break;
      case MOK:
        DialogItems[i].DefaultButton = 1;
        break;
    }

    if (bNoFocus && DialogItems[i].Type == DI_CHECKBOX && !(DialogItems[i].Flags & DIF_DISABLE))
    {
      DialogItems[i].Focus = TRUE;
      bNoFocus = false;
    }
  }

  if (hKey)
    RegCloseKey(hKey);

  int ExitCode = Info.DialogEx(Info.ModuleNumber, -1, -1, 66, 22, _T("Contents"),
                               DialogItems, sizeof(DialogItems) / sizeof(DialogItems[0]),
                               0, 0, ShowDialogProc, DlgData);
  if (ExitCode == (sizeof(InitItems) / sizeof(InitItems[0]) - 1) || ExitCode == -1)
    return false;

  for (i = 0; i < sizeof(InitItems) / sizeof(InitItems[0]); i++)
    if (InitItems[i].StoreTo)
      if (InitItems[i].Type == DI_CHECKBOX || InitItems[i].Type == DI_RADIOBUTTON)
        *InitItems[i].StoreTo = (DWORD)DialogItems[i].Param.Selected;
      else if (InitItems[i].Type == DI_FIXEDIT)
        *InitItems[i].StoreTo = FSF.atoi(DialogItems[i]._DD);
#undef _DD

  DWORD dwDisposition;
  if (PluginRootKey &&
       RegCreateKeyEx(HKEY_CURRENT_USER, PluginRootKey, 0, NULL, REG_OPTION_NON_VOLATILE,
       KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS)
  {
    for (i = 0; i < sizeof(InitItems) / sizeof(InitItems[0]); i++)
      if (!(DialogItems[i].Flags & DIF_DISABLE) && InitItems[i].SelectedRegValue)
      {
        DWORD dwValue = *InitItems[i].StoreTo;
        RegSetValueEx(hKey, InitItems[i].SelectedRegValue, 0, REG_DWORD, (BYTE *)&dwValue, sizeof(dwValue));
      }
    RegCloseKey(hKey);
  }

  if (bPluginPanels)
  {
    Opt.ProcessSubfolders = FALSE;
    Opt.CompareContents = FALSE;
  }
  Opt.ProcessHidden = (Info.AdvControl(Info.ModuleNumber, ACTL_GETPANELSETTINGS, NULL) &
                        FPS_SHOWHIDDENANDSYSTEMFILES) != 0;

  return true;
}

static bool bBrokenByEsc;
static HANDLE hConInp = INVALID_HANDLE_VALUE;

/****************************************************************************
 * �������� �� Esc. ���������� true, ���� ������������ ����� Esc
 ****************************************************************************/
static bool CheckForEsc(void)
{
  if (hConInp == INVALID_HANDLE_VALUE)
    return false;

  static DWORD dwTicks;
  DWORD dwNewTicks = GetTickCount();
  if (dwNewTicks - dwTicks < 500)
    return false;
  dwTicks = dwNewTicks;

  INPUT_RECORD rec;
  DWORD ReadCount;
  while (PeekConsoleInput(hConInp, &rec, 1, &ReadCount) && ReadCount)
  {
    ReadConsoleInput(hConInp, &rec, 1, &ReadCount);
    if ( rec.EventType == KEY_EVENT && rec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE &&
         rec.Event.KeyEvent.bKeyDown )
      // ������������ ������������� ���������� �� Esc
      if ( Info.AdvControl(Info.ModuleNumber, ACTL_GETCONFIRMATIONS, NULL) & FCS_INTERRUPTOPERATION )
      {
        const TCHAR *MsgItems[] = {
          GetMsg(MEscTitle),
          GetMsg(MEscBody),
          GetMsg(MOK),
          GetMsg(MCancel)
        };
        if ( !Info.Message(Info.ModuleNumber,
                           FMSG_WARNING,
                           NULL,
                           MsgItems,
                           sizeof(MsgItems) / sizeof(MsgItems[0]),
                           2) )
          return bBrokenByEsc = true;
      }
      else
        return bBrokenByEsc = true;
  }

  return false;
}

/****************************************************************************
 * ������ ������ ��� ����� �� ���� � �����
 ****************************************************************************/
static TCHAR *BuildFullFilename(const TCHAR *cpDir, const TCHAR *cpFileName)
{
  static TCHAR cName[NM];
  FSF.AddEndSlash(lstrcpy(cName, cpDir));

  return lstrcat(cName, cpFileName);
}

struct FileIndex {
  PluginPanelItem **ppi;
  int iCount;
};

/****************************************************************************
 * ������� ��������� ��� ������ � ���� ���������� PluginPanelItem
 * ��� ���� qsort()
 ****************************************************************************/
static int __cdecl PICompare(const void *el1, const void *el2)
{
  const PluginPanelItem *ppi1 = *(const PluginPanelItem **)el1, *ppi2 = *(const PluginPanelItem **)el2;

  if (ppi1->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    if (!(ppi2->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      return -1;
  }
  else
  {
    if (ppi2->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return 1;
  }

  return -FSF.LStricmp(ppi1->FindData._cFileName, ppi2->FindData._cFileName);
}

/****************************************************************************
 * ���������� �������������� ������ ������ ��� �������� ���������
 ****************************************************************************/
static bool BuildPanelIndex(const struct PanelInfo *pInfo, struct FileIndex *pIndex)
{
  bool bProcessSelected;
  pIndex->ppi = NULL;
  pIndex->iCount = ( bProcessSelected = (Opt.ProcessSelected && pInfo->SelectedItemsNumber &&
                     (pInfo->SelectedItems[0].Flags & PPIF_SELECTED)) ) ? pInfo->SelectedItemsNumber :
                     pInfo->ItemsNumber;
  if (!pIndex->iCount)
    return true;
  if (!(pIndex->ppi = (PluginPanelItem **)malloc(pIndex->iCount*sizeof(pIndex->ppi[0]))))
    return false;
  int j = 0;
  for (int i = pInfo->ItemsNumber - 1; i >= 0 && j < pIndex->iCount; i--)
    if ( (Opt.ProcessSubfolders ||
         !(pInfo->PanelItems[i].FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) &&
         (!bProcessSelected || (pInfo->PanelItems[i].Flags & PPIF_SELECTED)) &&
         lstrcmp(pInfo->PanelItems[i].FindData._cFileName, _T("..")) &&
         lstrcmp(pInfo->PanelItems[i].FindData._cFileName, _T(".")) )
      {
        pIndex->ppi[j++] = &pInfo->PanelItems[i];
      }
  if ((pIndex->iCount = j) != 0)
    FSF.qsort(pIndex->ppi, j, sizeof(pIndex->ppi[0]), PICompare);
  else {
    free(pIndex->ppi);
    pIndex->ppi = NULL;
    pIndex->iCount = 0;
  }

  return true;
}

/****************************************************************************
 * ������������ ������
 ****************************************************************************/
static void FreePanelIndex(struct FileIndex *pIndex)
{
  if (pIndex->ppi)
    free(pIndex->ppi);
  pIndex->ppi = NULL;
  pIndex->iCount = 0;
}

/****************************************************************************
 * ������ ��������� ������� Info.GetDirList(). � ������� �� ���� ����������
 * ������ ������ ������ � �������� Dir, ��� ������������.
 ****************************************************************************/
static int GetDirList(PanelInfo *PInfo, const TCHAR *Dir)
{
  TCHAR cPathMask[MAX_PATH];
  WIN32_FIND_DATA wfdFindData;
  HANDLE hFind;
  struct PluginPanelItem **pPanelItem = &PInfo->PanelItems;
  int *pItemsNumber = &PInfo->ItemsNumber;
#ifndef UNICODE
  lstrcpy(PInfo->CurDir, Dir);
#else
  PInfo->lpwszCurDir = wcsdup(Dir);
#endif
  *pPanelItem = NULL;
  *pItemsNumber = 0;

  if ( (hFind = FindFirstFile(lstrcat(lstrcpy(cPathMask, Dir), _T("\\*")), &wfdFindData)) ==
       INVALID_HANDLE_VALUE )
    return TRUE;

  int iRet = TRUE;
  do
  {
    if (!lstrcmp(wfdFindData.cFileName, _T(".")) || !lstrcmp(wfdFindData.cFileName, _T("..")))
      continue;
    if ((wfdFindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !Opt.ProcessHidden)
      continue;
    struct PluginPanelItem *pPPI;
    if (!(pPPI = (struct PluginPanelItem *)realloc(*pPanelItem, (*pItemsNumber + 1) * sizeof(*pPPI))))
    {
      iRet = FALSE;
      break;
    }
    *pPanelItem = pPPI;
    WFD2FFD(wfdFindData,(*pPanelItem)[(*pItemsNumber)++].FindData);
  } while (FindNextFile(hFind, &wfdFindData));
  FindClose(hFind);
  return iRet;
}

/****************************************************************************
 * ������ ��������� ������� Info.FreeDirList().
 ****************************************************************************/
static void FreeDirList(struct PanelInfo *AInfo)
{
  if (AInfo->PanelItems) {
#ifdef UNICODE
    for ( int i = 0; i < AInfo->ItemsNumber; i++ ) {
      free(AInfo->PanelItems[i].FindData.lpwszAlternateFileName);
      free(AInfo->PanelItems[i].FindData.lpwszFileName);
    }
#endif
    free(AInfo->PanelItems);
  }
}

static bool CompareDirs(const struct PanelInfo *AInfo, const struct PanelInfo *PInfo, bool bCompareAll, int ScanDepth);
static DWORD bufSize;

//TODO: ��� ����� (�� ����� CompareFiles) �� ������������ � unicode/locale
//      �, ��� ����, � ��������������� ������
static char *ABuf, *PBuf;

bool isnewline(int c)
{
  return (c == '\r' || c == '\n');
}

/****************************************************************************
 * ��������� ��������� � ������� ��� ���� ���������� ��������� (������ ���
 * ������������).
 * ���������� true, ���� ��� ���������.
 ****************************************************************************/
static bool CompareFiles( const FAR_FIND_DATA *AData, const FAR_FIND_DATA *PData,
                          const TCHAR *ACurDir, const TCHAR *PCurDir, int ScanDepth )
{
  if (AData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    // ����� ���������� ��� �����������
    if (Opt.ProcessSubfolders)
    {
      if (Opt.UseMaxScanDepth && Opt.MaxScanDepth<ScanDepth+1)
        return true;
      // �������� ������ ������ � ������������
      struct PanelInfo AInfo, PInfo;
      memset(&AInfo, 0, sizeof(AInfo));
      memset(&PInfo, 0, sizeof(PInfo));
      bool bEqual;
      if(   !GetDirList(&AInfo, BuildFullFilename(ACurDir, AData->_cFileName))
         || !GetDirList(&PInfo, BuildFullFilename(PCurDir, PData->_cFileName)))
      {
        bBrokenByEsc = true; // �� �� ���� �������, �� �� ������ ������
        bEqual = false; // ��������� ���������
      }
      else
        bEqual = CompareDirs(&AInfo, &PInfo, false, ScanDepth+1);
      FreeDirList(&AInfo);
      FreeDirList(&PInfo);
#ifdef UNICODE
      free(AInfo.lpwszCurDir);
      free(PInfo.lpwszCurDir);
#endif
      return bEqual;
    }
  }
  else
  {
    // ����� ���������� ��� �����
    if (Opt.CompareSize)
#ifndef UNICODE
      if (AData->nFileSizeLow != PData->nFileSizeLow || AData->nFileSizeHigh != PData->nFileSizeHigh)
#else
      if (AData->nFileSize != PData->nFileSize)
#endif
        return false;
    if (Opt.CompareTime)
    {
      if (Opt.LowPrecisionTime || Opt.IgnorePossibleTimeZoneDifferences)
      {
        union {
          __int64 num;
          struct
          {
            DWORD lo;
            DWORD hi;
          } hilo;
        } Precision, Difference, TimeDelta, temp;

        Precision.hilo.hi = 0;
        Precision.hilo.lo = Opt.LowPrecisionTime ? 20000000 : 0; //2s or 0s
        Difference.num = _i64(9000000000); //15m

        if (AData->ftLastWriteTime.dwHighDateTime > PData->ftLastWriteTime.dwHighDateTime)
        {
          TimeDelta.hilo.hi = AData->ftLastWriteTime.dwHighDateTime - PData->ftLastWriteTime.dwHighDateTime;
          TimeDelta.hilo.lo = AData->ftLastWriteTime.dwLowDateTime - PData->ftLastWriteTime.dwLowDateTime;
          if (TimeDelta.hilo.lo > AData->ftLastWriteTime.dwLowDateTime)
            --TimeDelta.hilo.hi;
        }
        else
        {
          if (AData->ftLastWriteTime.dwHighDateTime == PData->ftLastWriteTime.dwHighDateTime)
          {
            TimeDelta.hilo.hi = 0;
            TimeDelta.hilo.lo = max(PData->ftLastWriteTime.dwLowDateTime,AData->ftLastWriteTime.dwLowDateTime)-
                                min(PData->ftLastWriteTime.dwLowDateTime,AData->ftLastWriteTime.dwLowDateTime);
          }
          else
          {
            TimeDelta.hilo.hi = PData->ftLastWriteTime.dwHighDateTime - AData->ftLastWriteTime.dwHighDateTime;
            TimeDelta.hilo.lo = PData->ftLastWriteTime.dwLowDateTime - AData->ftLastWriteTime.dwLowDateTime;
            if (TimeDelta.hilo.lo > PData->ftLastWriteTime.dwLowDateTime)
              --TimeDelta.hilo.hi;
          }
        }

        //������������ �������� �� ������ ��� 26 �����.
        if (Opt.IgnorePossibleTimeZoneDifferences)
        {
          int counter=0;
          while (TimeDelta.hilo.hi > Difference.hilo.hi && counter<=26*4)
          {
            temp.hilo.lo = TimeDelta.hilo.lo - Difference.hilo.lo;
            temp.hilo.hi = TimeDelta.hilo.hi - Difference.hilo.hi;
            if (temp.hilo.lo > TimeDelta.hilo.lo)
              --temp.hilo.hi;
            TimeDelta.hilo.lo = temp.hilo.lo;
            TimeDelta.hilo.hi = temp.hilo.hi;
            ++counter;
          }
          if (counter<=26*4 && TimeDelta.hilo.hi == Difference.hilo.hi)
          {
            TimeDelta.hilo.hi = 0;
            TimeDelta.hilo.lo = max(TimeDelta.hilo.lo,Difference.hilo.lo) - min(TimeDelta.hilo.lo,Difference.hilo.lo);
          }
        }

        if (Precision.hilo.hi < TimeDelta.hilo.hi ||
            (Precision.hilo.hi == TimeDelta.hilo.hi && Precision.hilo.lo < TimeDelta.hilo.lo))
          return false;
      }
      else if (AData->ftLastWriteTime.dwLowDateTime != PData->ftLastWriteTime.dwLowDateTime ||
               AData->ftLastWriteTime.dwHighDateTime != PData->ftLastWriteTime.dwHighDateTime)
        return false;
    }
    if (Opt.CompareContents)
    {
      HANDLE hFileA, hFileP;
      TCHAR cpFileA[MAX_PATH], cpFileP[MAX_PATH];
      ShowMessage(lstrcpy(cpFileA, BuildFullFilename(ACurDir, AData->_cFileName)),
                  lstrcpy(cpFileP, BuildFullFilename(PCurDir, PData->_cFileName)));
      if ((hFileA = CreateFile(cpFileA, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
        return false;
      if ((hFileP = CreateFile(cpFileP, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
      {
        CloseHandle(hFileA);
        return false;
      }

      bool bEqual = true;
      DWORD ReadSizeA, ReadSizeP;
      if (!Opt.CompareContentsIgnore)
      {
        do
        {
          if (   CheckForEsc()
              || !ReadFile(hFileA, ABuf, bufSize, &ReadSizeA, NULL)
              || !ReadSizeA
              || !ReadFile(hFileP, PBuf, bufSize, &ReadSizeP, NULL)
              || ReadSizeA != ReadSizeP
              || memcmp(ABuf, PBuf, ReadSizeA) )
          {
            bEqual = false;
            break;
          }
        } while (ReadSizeA == bufSize);
      }
      else
      {
        ReadSizeA=1;
        ReadSizeP=1;
        char *PtrA=ABuf+ReadSizeA, *PtrP=PBuf+ReadSizeP;
        bool bExpectNewLineA = false;
        bool bExpectNewLineP = false;
        while (true)
        {
          while (PtrA >= ABuf+ReadSizeA && ReadSizeA)
          {
            if (CheckForEsc() || !ReadFile(hFileA, ABuf, bufSize, &ReadSizeA, NULL))
            {
              bEqual = false;
              break;
            }
            PtrA=ABuf;
          }

          if (!bEqual)
            break;

          while (PtrP >= PBuf+ReadSizeP && ReadSizeP)
          {
            if (CheckForEsc() || !ReadFile(hFileP, PBuf, bufSize, &ReadSizeP, NULL))
            {
              bEqual = false;
              break;
            }
            PtrP=PBuf;
          }

          if (!bEqual || (!ReadSizeP && !ReadSizeA))
            break;

          if (Opt.IgnoreWhitespace)
          {
            while (PtrA < ABuf+ReadSizeA && PtrP < PBuf+ReadSizeP && !isspace(*PtrA) && !isspace(*PtrP))
            {
              if (*PtrA != *PtrP)
              {
                bEqual = false;
                break;
              }
              ++PtrA;
              ++PtrP;
            }

            if (!bEqual)
              break;

            while (PtrA < ABuf+ReadSizeA && isspace(*PtrA))
              ++PtrA;

            while (PtrP < PBuf+ReadSizeP && isspace(*PtrP))
              ++PtrP;
          }
          else
          {
            if (bExpectNewLineA)
            {
              bExpectNewLineA = false;
              if (PtrA < ABuf+ReadSizeA && *PtrA == '\n')
                ++PtrA;
            }

            if (bExpectNewLineP)
            {
              bExpectNewLineP = false;
              if (PtrP < PBuf+ReadSizeP && *PtrP == '\n')
                ++PtrP;
            }

            while (PtrA < ABuf+ReadSizeA && PtrP < PBuf+ReadSizeP && !isnewline(*PtrA) && !isnewline(*PtrP))
            {
              if (*PtrA != *PtrP)
              {
                bEqual = false;
                break;
              }
              ++PtrA;
              ++PtrP;
            }

            if (!bEqual)
              break;

            if (PtrA < ABuf+ReadSizeA && PtrP < PBuf+ReadSizeP && (!isnewline(*PtrA) || !isnewline(*PtrP)))
            {
              bEqual = false;
              break;
            }

            if (PtrA < ABuf+ReadSizeA && PtrP < PBuf+ReadSizeP)
            {
              if (*PtrA == '\r')
                bExpectNewLineA = true;

              if (*PtrP == '\r')
                bExpectNewLineP = true;

              ++PtrA;
              ++PtrP;
            }
          }

          if (PtrA < ABuf+ReadSizeA && !ReadSizeP)
          {
            bEqual = false;
            break;
          }

          if (PtrP < PBuf+ReadSizeP && !ReadSizeA)
          {
            bEqual = false;
            break;
          }
        }
      }

      CloseHandle(hFileA);
      CloseHandle(hFileP);
      return bEqual;
    }
  }
  return true;
}

/****************************************************************************
 * ��������� ���� ���������, ��������� ����������� AInfo � PInfo.
 * ���������� true, ���� ��� ���������.
 * �������� bCompareAll ����������,
 * ���� �� ���������� ��� ����� � �������� PPIF_SELECTED (bCompareAll == true)
 * ��� ������ ������� false ��� ������ ������������ (bCompareAll == false).
 ****************************************************************************/
static bool CompareDirs(const struct PanelInfo *AInfo, const struct PanelInfo *PInfo, bool bCompareAll, int ScanDepth)
{
#ifndef _UNICODE
#define _CurDir     CurDir
#else
#define _CurDir     lpwszCurDir
#endif
  // ������ ������� ������ ��� �������� ���������
  struct FileIndex sfiA, sfiP;
  TCHAR DirA[MAX_PATH], DirP[MAX_PATH];
  ShowMessage(lstrcpy(DirA, BuildFullFilename(AInfo->_CurDir, _T("*"))),
              lstrcpy(DirP, BuildFullFilename(PInfo->_CurDir, _T("*"))));
  if (!BuildPanelIndex(AInfo, &sfiA) || !BuildPanelIndex(PInfo, &sfiP))
  {
    const TCHAR *MsgItems[] = {
      GetMsg(MNoMemTitle),
      GetMsg(MNoMemBody),
      GetMsg(MOK)
    };
    Info.Message(Info.ModuleNumber,
                 FMSG_WARNING,
                 NULL,
                 MsgItems,
                 sizeof(MsgItems) / sizeof(MsgItems[0]),
                 1);
    bBrokenByEsc = true;
    FreePanelIndex(&sfiA);
    FreePanelIndex(&sfiP);
    return true;
  }
  bool bDifferenceNotFound = true;
  int i = sfiA.iCount - 1, j = sfiP.iCount - 1;
  while (i >= 0 && j >= 0 && (bDifferenceNotFound || bCompareAll) && !bBrokenByEsc)
  {
    const int iMaxCounter = 256;
    static int iCounter = iMaxCounter;
    if (!--iCounter)
    {
      iCounter = iMaxCounter;
      if (CheckForEsc())
        break;
    }
    switch (PICompare(&sfiA.ppi[i], &sfiP.ppi[j]))
    {
      case 0: // ����� ������� - ��������� �� ���������
        if (CompareFiles(&sfiA.ppi[i]->FindData, &sfiP.ppi[j]->FindData, AInfo->_CurDir, PInfo->_CurDir, ScanDepth))
        { // � ��������� �������
          sfiA.ppi[i--]->Flags &= ~PPIF_SELECTED;
          sfiP.ppi[j--]->Flags &= ~PPIF_SELECTED;
        }
        else
        {
          bDifferenceNotFound = false;
          sfiA.ppi[i--]->Flags |= PPIF_SELECTED;
          sfiP.ppi[j--]->Flags |= PPIF_SELECTED;
        }
        break;
      case 1: // ������� sfiA.ppi[i] �� ����� ���������� � sfiP.ppi
        bDifferenceNotFound = false;
        sfiA.ppi[i--]->Flags |= PPIF_SELECTED;
        break;
      case -1: // ������� sfiP.ppi[j] �� ����� ���������� � sfiA.ppi
        bDifferenceNotFound = false;
        sfiP.ppi[j--]->Flags |= PPIF_SELECTED;
        break;
    }
  }
  if (!bBrokenByEsc)
  {
    // ���������� ��������� ��������. ������� ��, ��� �������� �������������� � ��������
    if (i >= 0)
    {
      bDifferenceNotFound = false;
      if (bCompareAll)
        for (; i >= 0; i--)
          sfiA.ppi[i]->Flags |= PPIF_SELECTED;
    }
    if (j >= 0)
    {
      bDifferenceNotFound = false;
      if (bCompareAll)
        for (; j >= 0; j--)
          sfiP.ppi[j]->Flags |= PPIF_SELECTED;
    }
  }
  FreePanelIndex(&sfiA);
  FreePanelIndex(&sfiP);
  return bDifferenceNotFound;
#undef _CurDir
}

/****************************************************************************
 ***************************** Exported functions ***************************
 ****************************************************************************/

static bool bOldFAR = false;

/****************************************************************************
 * ��� ������� ������� FAR �������� � ������ �������
 ****************************************************************************/
void WINAPI EXP_NAME(SetStartupInfo)(const struct PluginStartupInfo *Info)
{
  const TCHAR *cpPlugRegKey = _T("\\AdvCompare");
  ::Info = *Info;

  if (Info->StructSize >= (int)sizeof(struct PluginStartupInfo))
    FSF = *Info->FSF;
  else
    bOldFAR = true;

  if (PluginRootKey)
  {
    free(PluginRootKey);
    PluginRootKey = NULL;
  }

  if (PluginRootKey = (TCHAR*)malloc(sizeof(TCHAR)*(lstrlen(Info->RootKey) + lstrlen(cpPlugRegKey) + 1)))
  {
    lstrcpy(PluginRootKey, Info->RootKey);
    lstrcat(PluginRootKey, cpPlugRegKey);
  }
  else
  {
    const TCHAR *MsgItems[] = {
      GetMsg(MNoMemTitle),
      GetMsg(MNoMemBody),
      GetMsg(MOK)
    };
    ::Info.Message(::Info.ModuleNumber,
                   FMSG_WARNING,
                   NULL,
                   MsgItems,
                   sizeof(MsgItems) / sizeof(MsgItems[0]),
                   1);
  }
}

/****************************************************************************
 * ��� ������� ������� FAR �������� �� ������ �������
 ****************************************************************************/
void WINAPI EXP_NAME(GetPluginInfo)(struct PluginInfo *Info)
{
  static const TCHAR *PluginMenuStrings[1];

  Info->StructSize              = (int)sizeof(*Info);
  Info->Flags                   = 0;
  Info->DiskMenuStrings         = NULL;
  PluginMenuStrings[0]          = GetMsg(MCompare);
  Info->PluginMenuStrings       = PluginMenuStrings;
  Info->PluginMenuStringsNumber = sizeof(PluginMenuStrings) / sizeof(PluginMenuStrings[0]);
  Info->PluginConfigStrings     = NULL;
  Info->CommandPrefix           = NULL;
}

/****************************************************************************
 * �������� ������� �������. FAR � ��������, ����� ������������ ���� ������
 ****************************************************************************/
HANDLE WINAPI EXP_NAME(OpenPlugin)(int OpenFrom, INT_PTR Item)
{
  // ���� ������ ���� ������� �����...
  if (bOldFAR)
  {
    const TCHAR *MsgItems[] = {
      GetMsg(MOldFARTitle),
      GetMsg(MOldFARBody),
      GetMsg(MOK)
    };
    Info.Message(Info.ModuleNumber,
                 FMSG_WARNING,
                 NULL,
                 MsgItems,
                 sizeof(MsgItems) / sizeof(MsgItems[0]),
                 1);

    return INVALID_HANDLE_VALUE;
  }

  struct PanelInfo AInfo, PInfo;

  // ���� �� ������� ��������� ���������� � �������...
#ifndef UNICODE
  if (   !Info.Control(INVALID_HANDLE_VALUE, FCTL_GETPANELINFO, &AInfo)
      || !Info.Control(INVALID_HANDLE_VALUE, FCTL_GETANOTHERPANELINFO, &PInfo) )
#else
  if (   !Info.Control(CURRENT_PANEL, FCTL_GETPANELINFO, &AInfo)
      || !Info.Control(ANOTHER_PANEL, FCTL_GETPANELINFO, &PInfo) )
#endif
  {
    return INVALID_HANDLE_VALUE;
  }

  // ���� ������ ����������...
  if (AInfo.PanelType != PTYPE_FILEPANEL || PInfo.PanelType != PTYPE_FILEPANEL)
  {
    const TCHAR *MsgItems[] = {
      GetMsg(MCmpTitle),
      GetMsg(MFilePanelsRequired),
      GetMsg(MOK)
    };
    Info.Message(Info.ModuleNumber,
                 FMSG_WARNING,
                 NULL,
                 MsgItems,
                 sizeof(MsgItems) / sizeof(MsgItems[0]),
                 1);

    return INVALID_HANDLE_VALUE;
  }

  // ���� �� ����� �������� ������ �������...
  if ( !ShowDialog(AInfo.Plugin || PInfo.Plugin,
                  (AInfo.SelectedItemsNumber && (AInfo.SelectedItems[0].Flags & PPIF_SELECTED)) ||
                  (PInfo.SelectedItemsNumber && (PInfo.SelectedItems[0].Flags & PPIF_SELECTED))) )
  {
    return INVALID_HANDLE_VALUE;
  }

  // ������� ���������� ���� ��� �������� �� Esc...
  HANDLE hScreen = Info.SaveScreen(0, 0, -1, -1);
  hConInp = CreateFile(_T("CONIN$"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

  // ��������� ����������� ������ ������� ���������...
  HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  CONSOLE_SCREEN_BUFFER_INFO csbiNfo;
  if (GetConsoleScreenBufferInfo(hConOut, &csbiNfo))
  {
    if ((iTruncLen = csbiNfo.dwSize.X - 20) > NM - 2)
      iTruncLen = NM - 2;
    else if (iTruncLen < 0)
      iTruncLen = csbiNfo.dwSize.X - csbiNfo.dwSize.X / 4;
  }
  else
    iTruncLen = 60;
  CloseHandle(hConOut);

  // �� ����� ��������� ������� ��������� ������� ����...
  TCHAR cConsoleTitle[MAX_PATH], cBuffer[MAX_PATH];
  DWORD dwTitleSaved = GetConsoleTitle(cConsoleTitle, sizeof(cConsoleTitle));
#ifndef UNICODE
  OSVERSIONINFO ovi;
  ovi.dwOSVersionInfoSize = sizeof(ovi);
  if (GetVersionEx(&ovi) && ovi.dwPlatformId != VER_PLATFORM_WIN32_NT)
    OemToChar(GetMsg(MComparingFiles), cBuffer);
  else
#endif
    lstrcpy(cBuffer, GetMsg(MComparingFiles));
  SetConsoleTitle(cBuffer);

  // ������ ������ ������ ��������� �� �������...
  HKEY hKey;
  if ( !PluginRootKey || RegOpenKeyEx(HKEY_CURRENT_USER, PluginRootKey, 0, KEY_QUERY_VALUE, &hKey) !=
       ERROR_SUCCESS )
    hKey = 0;
  DWORD dwSize = sizeof(DWORD);
  bufSize = ( hKey && (RegQueryValueEx(hKey, _T("CompareBufferSize"), NULL, NULL, (LPBYTE)&bufSize,
              &dwSize) == ERROR_SUCCESS) && (bufSize > 32767) ) ? bufSize : 32768;
  if (hKey)
    RegCloseKey(hKey);
  ABuf = (char*)malloc(bufSize);
  PBuf = (char*)malloc(bufSize);

  bBrokenByEsc = false;
  bStart = true;
  bool bDifferenceNotFound = false;

  // ������ ����� �������� ������� �� �������...
  if (ABuf && PBuf)
    bDifferenceNotFound = CompareDirs(&AInfo, &PInfo, true, 0);

  free (ABuf);
  free (PBuf);
  CloseHandle(hConInp);
  Info.RestoreScreen(hScreen);

  // �������� ����� � �������������� ������. ���� ����� ���������� ���������...
  if (!bBrokenByEsc)
  {
#ifndef UNICODE
    Info.Control(INVALID_HANDLE_VALUE, FCTL_SETSELECTION, &AInfo);
    Info.Control(INVALID_HANDLE_VALUE, FCTL_SETANOTHERSELECTION, &PInfo);
    Info.Control(INVALID_HANDLE_VALUE, FCTL_REDRAWPANEL, NULL);
    Info.Control(INVALID_HANDLE_VALUE, FCTL_REDRAWANOTHERPANEL, NULL);
#else
    Info.Control(CURRENT_PANEL, FCTL_SETSELECTION, &AInfo);
    Info.Control(ANOTHER_PANEL, FCTL_SETSELECTION, &PInfo);
    Info.Control(CURRENT_PANEL, FCTL_REDRAWPANEL, NULL);
    Info.Control(ANOTHER_PANEL, FCTL_REDRAWPANEL, NULL);
#endif
    if (bDifferenceNotFound && Opt.MessageWhenNoDiff)
    {
      const TCHAR *MsgItems[] = {
        GetMsg(MNoDiffTitle),
        GetMsg(MNoDiffBody),
        GetMsg(MOK)
      };
      Info.Message(Info.ModuleNumber,
                   0,
                   NULL,
                   MsgItems,
                   sizeof(MsgItems) / sizeof(MsgItems[0]),
                   1);
    }
  }
  // ����������� ��������� ������� ����...
  if (dwTitleSaved)
    SetConsoleTitle(cConsoleTitle);

  return INVALID_HANDLE_VALUE;
}

/****************************************************************************
 * ��� ������� FAR �������� ����� ��������� �������
 ****************************************************************************/
void WINAPI EXP_NAME(ExitFAR)(void)
{
  if (PluginRootKey)
  {
    free(PluginRootKey);
    PluginRootKey = NULL;
  }
}
