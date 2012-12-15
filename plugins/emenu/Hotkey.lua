-- Hotkeys [Shift-]Apps, Alt-[Shift-]Apps and Ctrl-Apps to show context menu

Macro {
  area="Shell"; key="Apps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("2") end 
  end;
}

Macro {
  area="Shell"; key="ShiftApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("2") end 
  end;
}

Macro {
  area="Shell"; key="AltApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("1") end 
  end;
}

Macro {
  area="Shell"; key="AltShiftApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("1") end 
  end;
}

Macro {
  area="Shell"; key="CtrlApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") 
  end;
}

Macro {
  area="Tree"; key="Apps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("2") end 
  end;
}

Macro {
  area="Tree"; key="ShiftApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("2") end 
  end;
}

Macro {
  area="Tree"; key="AltApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("1") end 
  end;
}

Macro {
  area="Tree"; key="AltShiftApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") if Menu.Id == "5099B83C-4222-4325-95A6-F6FC4635DED6" then  Keys("1") end 
  end;
}

Macro {
  area="Tree"; key="CtrlApps"; flags="DisableOutput"; action = function()
 Keys("F11 x Enter") 
  end;
}

