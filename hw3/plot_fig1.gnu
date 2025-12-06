set datafile separator ","

set term pngcairo size 900,650 enhanced font "Helvetica,16" linewidth 3
set output "fig1_insert.png"

set border 3 linewidth 2
set grid xtics ytics lc rgb "#e0e0e0" lt 1 lw 1.2
set style fill transparent solid 0.1 noborder

set title "Insertion Time vs n" font "Helvetica,20" offset 0,-1
set xlabel "n" font "Helvetica,16"
set ylabel "Time (ns)" font "Helvetica,16"

set logscale x 2
set tics nomirror scale 0.8 out
set mxtics 2
set format x "10^{%L}"

set style line 1 lc rgb "#2E86C1" lt 1 lw 3 pt 7 ps 1.2 pi -1
set style line 2 lc rgb "#E74C3C" lt 1 lw 3 pt 5 ps 1.2 pi -1

plot "fig1.csv" using 1:2 with linespoints ls 1 title "BST (std::map)", \
     "fig1.csv" using 1:3 with linespoints ls 2 title "HT (std::unorderedmap)"
