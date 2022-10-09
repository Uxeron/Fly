windres resource.rc resource.o
g++ -DUNICODE -Os window.cpp fly.cpp vector.cpp -o fly.exe -mwindows resource.o