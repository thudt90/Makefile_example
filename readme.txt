# what?
Makefile là một đoạn mã (scrip) dùng để thực thì một vài nhiệm vụ:
  - Compile code
  - Thực thi một vài lệnh: clean file ...

# Structure of makefile
	target … : prerequisites …
		recipe
		…
		…
  - target thông thường tạo ra một file: file thực thi hoặc file object; cũng có thể là một hành động như clean.
  - prerequisite là file sử dụng như ngõ vào để tạo ra target. Target thông thường phụ thuộc vào một vài file.
  - recipe là hành động make thực hiện. Một recipe có một hay nhiều command.
**Note**: Bạn phải dùng "tab" để bắt đầu một dòng recipe!


# How

## Ví dụ đơn giản
Một makefile đơn giản được viết để tạo ra file thực thi _edit_ như sau:

>edit: main.o add.o sub.o mul.o dev.o print.o
    gcc -o edit main.o add.o sub.o mul.o dev.o print.o

main.o: main.c main.h recipes.h print_data.h
    gcc -c main.c
add.o : add.c recipes.h
    gcc -c add.c
sub.o : sub.c recipes.h
    gcc -c sub.c
mul.o : mul.c recipes.h
    gcc -c mul.c
dev.o : dev.c recipes.h
    gcc -c dev.c
print.o : print.c print_data.h main.h
    gcc -c print.c

clean :
    rm edit main.o add.o sub.o mul.o dev.o print.o


Lệnh để chạy makefile trên terminal

	make

Lệnh để xóa những file object và file thực thi

	make clean

** Giải thích **
Trong ví dụ trên, targets của makefile là file thực thi _edit_ và các object files _main.o_ and _print.o_ ... Các file prerequisite là _main.c_ và _main.h_. Recipe là _cc -c main.c_.

Target _clean_ không phải là một file mà là một hành động (_phony targets_). Nó không cần prerequisite và _make_ sẽ không bao giờ thực hiện hành động này nếu không nói chính xác - _make clean_.

Theo lý thuyết, _make_ bắt đầu với target đầu tiên, goi là default goal. Trong ví dụ default goal là update file thực thi _edit_.
_make_ đọc makefile trong thư mục hiện tại và bắt đầu với rule đầu tiên, relink _edit_; nhưng phải thực hiện các rule cho các file mà _edit_ phụ thuộc, trong ví dụ là các file object. Mỗi file object lại phụ thuộc vào các luật riêng của file đó.
Biên dịch lại phải được thực hiện nếu có sự thay đổi source file hay header file trong prerequisites hay file object file không tồn tại.

## Write makefile

Một vài điểm cần lưu ý khi viết makefile:
- ‘#’ bắt đầu commnet trong makefile
- '\' tách một dòng quá dài trong makefile

	Ví dụ:

		edit: main.o add.o sub.o \
			 mul.o dev.o print.o


- Tên chuẩn của makefile: _makefile_ hay _Makefile_. Nếu đặt tên file không chuẩn thì _make_ phải thêm arguments '-f _name_' hay '--file=_name_'

-  Including Other Makefiles

		include filename

	hay

		-include filename

	Ví dụ:

		include commom.mk


- In debug trong makefile
	Ví dụ:

		@echo "hello world"

#### Khai báo biến

Xét ví dụ trên ta thấy list object file

	edit: main.o add.o sub.o mul.o dev.o print.o
		gcc -o edit main.o add.o sub.o mul.o dev.o print.o

List các object file lập lại hai lần, để đơn giản thì dùng biến. Như ví dụ trên biến có thể là _objects, OBJECTS, objs, OBJS, obj,_ hoặc _OBJ_.

	objects = main.o add.o sub.o mul.o dev.o print.o

Khi đó _makefile_ có thể sửa lại thành

	edit: $(objects)
		gcc -o edit $(object)

	....
	clean :
		rm edit $(object)


Biến trong makefile là một chuỗi nhưng không có các ký tự như ':', '#', '=', whitespace. Tên biến phân biệt chữ hoa và chữ thường.

Có cách khác để lấy list các file mà không sợ thiếu file


	objects := $(patsubst %.c,%.o,$(wildcard *.c))


Câu lệnh chuyển list các file C thành list các file object sau đó gán vào biến _objects_.
Than khảo thêm về [wildcard] (https://www.gnu.org/software/make/manual/html_node/Wildcards.html)

#### Implicit rules
Một vài biến thông dụng hay sử dụng trong makefile

- AS: assembly file. default: as
- CC: c program. default: gcc
- CXX: c++ program. default: g++
- CPP: C preprocessor. default: $(CC) -E

### Automatic variables
Automatic variables: sử dụng trong recipe
- $@: file name of the target
- $<: name of the first prerequisite
- $^: the name of prerequisite

Ví dụ:

	foo.o : foo.c
		cc -c $^ -o $@

Trên đây là một vài biến Automatic variables thông dụng, tham khảo thêm về [Automatic variables] (https://www.gnu.org/software/make/manual/make.html#Automatic-Variables)

### Link lib
For example, if there is a /usr/lib/libcurses.a library on your system (and no /usr/lib/libcurses.so file), then

	foo : foo.c -lcurses
		cc $^ -o $@

Would cause the command ‘cc foo.c /usr/lib/libcurses.a -o foo’ to be executed when foo is older than foo.c or than /usr/lib/libcurses.a.

### Pattern Rules
pattern rule chứa ký tự '%' trong target và prerequisite.
Ví dụ, '%.c' tượng trưng cho tất cả các file có tên kết thúc bằng '.c'.
Lúc đó target có thể viết lại như sau:

	%.o : %.c
		recipe...

Xác định file _n_.o, với file _n_.c trong prerequisite, cung cấp _n_.c tồn tại.
Ví dụ:

	%.o : %.c
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


### Conditional
Makefile hỗ trợ các câu lệnh điều kiện như:

	ifeq else endif
	ifneq else endif
	ifdef else endif
	ifndef else endif

Để hiểu thêm về các điều kiện này ta xét các ví dụ

Ví dụ 1:

	BIND=
	ifeq ($(BIND),)
		@echo "if abc"
	else
		@echo "else abc"
	endif

- Result: _if abc_

Ví dụ 2:

	BIND=cc
	ifeq ($(BIND),)
		@echo "if abc"
	else
		@echo "else abc"
	endif

- Result: _else abc_

Ví dụ 3:

	bar =
	foo = $(bar)
	ifdef foo
		frobozz = yes
	else
		frobozz = no
	endif

- Result: yes

Ví dụ 4:

	foo =
	ifdef foo
		frobozz = yes
	else
		frobozz = no
	endif

- Result: no



Makefile
- Works in two distinct phases: read-in and target-update
- when the recipe start with '@', not print this recipe.
   when used command "make -n" or "make --just-print" -> only print recipe,without executing
   when used command "make -s" or "make --silent" -> only executing, without print
- Variable
	is a string, not ':', '#', '=', whitespace
	are case-sensitive
- Assign variable: '=', ':=', '?='
- Function wildcard
	src:=$(wildcard *.c)#is to get a list of all the C source files in a directory or
	src:=$(shell echo *.c)

- if condition
	ifeq else endif
	ifneq else endif
	ifdef else endif
	ifndef else endif
	Example:
		BIND=
		ifeq ($(BIND),)
			@echo "ì abc"
		else
			@echo "else abc"
		endif
		+ Result: ì abc

	BIND=cc
	ifeq ($(BIND),)
		@echo "if abc"
	else
		@echo "else abc"
	endif
	+ Result: else abc

	bar =
	foo = $(bar)
	ifdef foo
		frobozz = yes
	else
		frobozz = no
	endif
	+ Result: yes

	foo =
	ifdef foo
		frobozz = yes
	else
		frobozz = no
	endif
	+ Result: no

- Implicit rules
	Common variables used as programms in build:
	+ AS: assembly file. default: as
	+ CC: c program. default: gcc
	+ CXX: c++ program. default: g++
	+ CPP: C preprocessor. default: $(CC) -E
- Automatic variables: used in  recipe
	+ $@: file name of the target
	+ $<: name of the first prerequisite
	+ $^: the name of prerequisite


- Great lib.a: http://stackoverflow.com/questions/9593286/creating-a-library-file-in-makefile-and-compiling-after-that
	hello.o : hello.c print_1.h cal.h
		cc -c $< -o $@
	hello.a: hello.o
		ar -rv $@ $^
- Create lib.so (share library): https://stackoverflow.com/questions/14884126/build-so-file-from-c-file-using-gcc-command-line?answertab=votes#tab-top
	gcc -c -fPIC hello.c -o hello.o
	gcc hello.o -shared -o libhello.so
  or
	gcc -shared -o libhello.so -fPIC hello.c


#
# CL_*  Compile and link
# CC_*  Compile C/C++ source to an object file
# AS_*  Assemble something to an object file
# AR_*  Generate an object file library (archive)
# LR_*  Link a list of objects/libraries to a relocatable object file
# LD_*  Link a list of objects/libraries to a executable/shared object
# UM_*  Add a usage message to an executable
# PB_*  Add PhAB resources to an executable using "phabbind"
#

- Get current folder{
	PWD_HOST = /bin/pwd
	PWD := $(shell $(PWD_HOST))
}

