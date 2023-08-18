all:
	cls && cd build && cmake --build . && .\tests\Debug\test1.exe
