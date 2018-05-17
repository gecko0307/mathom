TARGET = libsparx.so
SRCDIR = .
OBJDIR = .
SRCNAMES = md5mesh.c md5anim.c jp2.c png.c dds.c tga.c bmp.c image.c quat.c bezier.c
CXX = gcc
CXXFLAGS =
LIBS = -L"." libz.a libopenjpeg.a
INCLUDE = 
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.c=.o)))

all:	$(OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $(TARGET) $(OBJ) $(LIBS) -Wl,-rpath,. 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

clean: 
	rm $(OBJDIR)/*.o

.PHONY: all clean






