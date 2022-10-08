windres resource.rc resource.o
g++ window.cpp fly.cpp vector.cpp -o fly.exe -mwindows resource.o