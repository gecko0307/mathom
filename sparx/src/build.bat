gcc -c quat.c -o quat.o
gcc -c md5mesh.c -o md5mesh.o
gcc -c md5anim.c -o md5anim.o
gcc -c image.c -o image.o
gcc -c dds.c -o dds.o
gcc -c tga.c -o tga.o
gcc -c bezier.c -o bezier.o
gcc -c bmp.c -o bmp.o
gcc -c png.c -o png.o
gcc -c jp2.c -o jp2.o
gcc -shared -o sparx.dll -L. quat.o image.o jp2.o png.o dds.o tga.o bmp.o bezier.o md5mesh.o md5anim.o libz32.a libopenjpeg32.a
strip -o sparx.stripped.dll sparx.dll   