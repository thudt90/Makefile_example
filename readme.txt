# what?
Makefile là một đoạn mã (scrip) dùng để thực thì một vài nhiệm vụ:
  - Compile code
  - Thực thi một vài lệnh: clean file ...

# Structure of makefile
	target … : prerequisites …
		recipe
		…
		…

  - **target** thông thường tạo ra một file: file thực thi hoặc file object; cũng có thể là một hành động như clean.
  - **prerequisite** là file sử dụng như ngõ vào để tạo ra target. Target thông thường phụ thuộc vào một vài file.
  - **recipe** là các bước cần thiết để tạo **target**. Một recipe có một hay nhiều bước.

**Note**: Bạn phải dùng "tab" để bắt đầu một dòng recipe!


# How

## Ví dụ đơn giản
Một _makefile_ đơn giản được viết để tạo ra file thực thi _edit_ như sau:

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
Trong ví dụ trên, _targets_ của makefile là file thực thi _edit_ và các object files _main.o_ and _print.o_ ... Các file _prerequisite_ là _main.c_ và _main.h_. Recipe là _cc -c main.c_.

Target _clean_ không phải là một file mà là một hành động (_phony targets_). Nó không cần _prerequisite_ và _make_ sẽ không bao giờ thực hiện hành động này nếu không nói chính xác - _make clean_.

Theo lý thuyết, _make_ bắt đầu với target đầu tiên, goi là _default goal_. Trong ví dụ trên default goal là update file thực thi _edit_.
_make_ đọc makefile trong thư mục hiện tại và bắt đầu với rule đầu tiên, relink _edit_; nhưng file _edit_ lại phụ thuộc vào các file _object_. Mỗi _object_ lại phụ thuộc vào các lệnh riêng của file đó.
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

	src := $(wildcard *.c)

Từ khóa _$(wildcard *.c)_ sẽ trả về list các _file.c_ trong thư mục hiện tại
Than khảo thêm về [wildcard](https://www.gnu.org/software/make/manual/html_node/Wildcards.html)

	objects := $(patsubst %.c,%.o,$(wildcard *.c))

Câu lệnh lấy tên các file _file.c_ và chuyển tên _file.o_ sau đó gán vào biến _objects_.


#### Implicit rules
Một vài biến thông dụng hay sử dụng trong makefile

- AS: Trình biên dịch ngôn ngữ asembly. default: as
- CC: Trình biên dịch ngôn ngữ C. default: gcc hay cc
- CPP: Trình biên dịch ngôn ngữ c++ program. default: g++
- ASFLAG : tham số cho AS. Default là = rv
- CCFLAG : tham số cho CC. Default là rỗng
- CPPFLAG : tham số cho AS. Default là rỗng

### Automatic variables
Automatic variables: sử dụng trong recipe
- $@: file name of the target
- $<: name of the first prerequisite
- $^: the name of prerequisite

Ví dụ:

	foo.o : foo.c
		cc -c $^ -o $@

Trên đây là một vài biến Automatic variables thông dụng, tham khảo thêm về [Automatic variables](https://www.gnu.org/software/make/manual/make.html#Automatic-Variables)

### Link lib
Theo ví dụ dưới, nếu có có file lib /usr/lib/libcurses.a (và không có file /usr/lib/libcurses.so ),

	foo : foo.c -lcurses
		cc -c $^ -o $@

Câu lệnh trên sẽ thực hiện _‘cc foo.c /usr/lib/libcurses.a -o foo’_ để tạo file thực thi _foo_ khi _foo_ cũ hơn _foo.c_ hoặc _/usr/lib/libcurses.a_.

### Pattern Rules
Quay lại ví dụ với makefile trên, ta thấy cần khai báo các target cho từng file object. Gỉa sử ta có một ứng dụng có số mã nguông lớn thì việc tạo makefile sẽ rất vất vả. Makefile cung cấp một số luật mẫu (pattern rule) kết hợp với biến tự động (Automatic variables) sẽ giảm bớt công việc cần phải làm.
Một pattern rule thông dụng là '%', xuất hiện trong _target_ và _prerequisite_.
Ví dụ, '%.c' tượng trưng cho tất cả các file có tên kết thúc bằng '.c'.
Lúc đó target có thể viết lại như sau:

	%.o : %.c
		recipe...

Xác định file _n_.o, với file _n_.c trong prerequisite.

Ví dụ (kết hợp pattern rule và Automatic variables):

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


