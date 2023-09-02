all:
	cd build && cmake --build . && .\test\Debug\nodeflow_test.exe
