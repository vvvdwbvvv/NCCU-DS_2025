main: main.cpp
	clang++ -std=c++11 -O2 -o main main.cpp

run: main
	./main

eval: evaluation.cpp main.cpp
	clang++ -std=c++11 -O2 -o eval evaluation.cpp

run_eval: eval
	./eval

format:
	clang-format -i main.cpp evaluation.cpp
