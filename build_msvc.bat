windres resource.rc resource.o
cl /DUNICODE /O1 /Fe:fly.exe window.cpp fly.cpp vector.cpp /link /emittoolversioninfo:no user32.lib Gdi32.lib shell32.lib resource.o