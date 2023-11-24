compile:
	cd build && cmake .. && cmake --build . && cd ..
run:
	.\build\test\Debug\nodeflow_test.exe
all:
	compile run
