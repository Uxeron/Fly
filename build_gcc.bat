windres resource.rc resource.o

g++ -DUNICODE -static-libgcc -static-libstdc++ -Os ^
    window.cpp ^
    Flies\Black\fly_black.cpp ^
    Flies\Red\fly_red.cpp ^
    Flies\Yellow\fly_yellow.cpp ^
    Flies\White\fly_white.cpp ^
    vector.cpp ^
    -o fly.exe ^
    -mwindows resource.o