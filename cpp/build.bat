windres resource.rc resource.o
g++ window.cpp -o fly.exe -mwindows resource.o