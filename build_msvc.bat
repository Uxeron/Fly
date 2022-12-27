windres resource.rc resource.o

cl /DUNICODE /O1 /Fe:fly.exe ^
    window.cpp ^
    Flies\Black\fly_black.cpp ^
    Flies\Red\fly_red.cpp ^
    Flies\Yellow\fly_yellow.cpp ^
    Flies\White\fly_white.cpp ^
    vector.cpp ^
    /link /emittoolversioninfo:no ^
    user32.lib Gdi32.lib shell32.lib resource.o