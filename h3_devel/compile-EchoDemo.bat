rem *********************************************************************
rem ** make sure to use the Visual Studio x64 Developer Command Prompt **
rem *********************************************************************
cl /EHsc /Iinclude EchoDemo.cpp Echo.cpp lib/*.lib user32.lib
