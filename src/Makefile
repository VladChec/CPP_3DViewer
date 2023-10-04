src=3DViewerV2.cc
obj=$(src:.c=.o)
CC=g++
FLAGS=-Wall -Werror -Wextra -std=c++17
PROJECT_NAME= 3DViewer
c_flags=-c -Wall -Werror -Wextra -std=c++17
check_flags=$(shell pkg-config --libs check)
gcov_flags=--coverage

ifeq ($(OS), Darwin)
	check_flags=-lcheck -lpthread -pthread -lrt -lm -lsubunit
else
	check_flags=-lcheck
endif

all: clean gcov_report install

model:
	cd model && $(CC) $(FLAGS) -c $(src)
.PHONY: model

3DViewer.a: model
	cd Model && ar rc $(PROJECT_NAME).a 3DViewerV2.o
	cd Model && ranlib $(PROJECT_NAME).a


%_test.o: %.c
	cd Model && $(CC) $(c_flags) $< -o $@ $(gcov_flags) $(check_flags)

test: 3DViewer.a
	cd Model && ${CC} -L. $(PROJECT_NAME).a test.cc -o test $(gcov_flags) $(check_flags) -g -lgtest
	cd Model && ./test

gcov_report:	
	cd Model && $(CC) --coverage $(check_flags) $(FLAGS) 3DViewerV2.cc test.cc -o test -g	-lgtest
	cd Model && ./test
	lcov -t "test" -o test.info -c -d . 
	genhtml -o report test.info
	open report/index.html	
	
install: 
	rm -rf install
	mkdir install
	cd Install && qmake ../3DW/3DW.pro && make && mv 3DW.app ../
	cd Install && rm -rf *.* && rm -rf Makefile && rm -rf .qmake.stash
	cd ..
	chmod 777 3DW.app/
	open 3DW.app/

uninstall:
	rm -rf install

clean:
	rm -rf docs/html/search && rm -rf docs/html
	rm -rf Install/*.* && rm -rf Install/Makefile && rm -rf Install/.qmake.stash
	rm -rf report && rm -rf test.c 3DW.gc* test1
	rm -rf Model/*.o Model/*.a *.tar *.app 3DWw
	rm -rf Model/*.gcda Model/*.gcno *.info
	rm -rf ./*/*.o ./*/*.a 
	rm -rf Model/test 

html:
	makeinfo --no-split --html doc/doc.texi -o doc/doc.html

dvi:
	texi2dvi doc/doc.info

dist: 
	rm -rf 3DWw
	mkdir 3DWw
	cp -R Model s213DWw_v2 Controller tests 3DWw/ && cp Makefile 3DWw/  && cp -R s213DWw_v2 3DWw  && cp -R doxygen 3DWw
	tar  -zcf s213DWw_v2_inst.tar 3DWw
	rm -rf 3DWw


