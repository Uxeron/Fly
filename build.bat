windres resource.rc resource.o
g++ -DUNICODE -static-libgcc -static-libstdc++ -Os window.cpp fly.cpp vector.cpp -o fly.exe -mwindows resource.o