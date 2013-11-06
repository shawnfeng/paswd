# 加密解密程序Makefile文件

# compile opinions
# CXX =
# CXXFLAGS = -Wall
CXXFLAGS = -Wall -g

# CC =
# CFLAGS = -Wall
CFLAGS = -Wall -g

EXEC=paswd
objfile=main.o secret.o ays_para.o
default: $(EXEC) 

$(EXEC):$(objfile)
	$(CXX) -o $(EXEC) $(objfile)

main.o: main.c secret.h
	$(CXX) $(CXXFLAGS) -c main.c

secret.o: secret.c secret.h
	$(CXX) $(CXXFLAGS) -c secret.c

ays_para.o: ays_para.c ays_para.h
	$(CXX) $(CXXFLAGS) -c ays_para.c

.PHONY:
	clean

clean:
	$(RM) *.o *.exe



testfun := test
testobj = test.o ays_para.o

testrun: $(testfun)

$(testfun): $(testobj)
	$(CC) -o $(testfun) $(testobj)

test.o: test.c ays_para.h
	$(CC) $(CFLAGS) -c test.c

