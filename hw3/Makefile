all: eval

main: main.cpp
	clang++ -std=c++11 -O2 -o main main.cpp

eval: eval.cpp
	clang++ -std=c++11 -O2 -o eval eval.cpp

run_fig1: eval
	./eval 1 > fig1.csv

run_fig2: eval
	./eval 2 > fig2.csv

run_plot: run_fig1 run_fig2
	gnuplot plot_fig1.gnu
	gnuplot plot_fig2.gnu
