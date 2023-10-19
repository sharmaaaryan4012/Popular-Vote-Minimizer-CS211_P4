# Given:
build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

run:
	./app.exe

run_quiet:
	./app.exe -q

valgrind:
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe

# Added functionality:
run_fast:
	./app.exe -f  # Assuming -f turns on the fast mode

run_verbose:
	./app.exe -v  # Assuming -v turns on verbose logging

run_debug:
	./app.exe -d  # Assuming -d turns on debug mode

built_test:
	rm -f test.exe
	gcc test.c MinPopVote.c -o test.exe

run_test:
	./test.exe

clean:
	rm -f app.exe test.exe

test_all:
	build_test
	run_test
