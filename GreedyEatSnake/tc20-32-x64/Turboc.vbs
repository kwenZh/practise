Set WshShell = CreateObject("Wscript.Shell")
WshShell.CurrentDirectory="dosbox"
WshShell.Run "dosbox.exe -conf tc20.conf -noconsole"
