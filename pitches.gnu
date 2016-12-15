set bar 1.000000 front
set style circle radius graph 0.02, first 0.00000, 0.00000
set style ellipse size graph 0.05, 0.03, first 0.00000 angle 0 units xy

set pm3d interpolate 0,0
set pm3d map


x = 0.0
xx = 6.08888888888889
dx = 1.11
x0 = -5
x1 = -3.89111111111111
x2 = -2.78222222222222
x3 = -1.67333333333333
x4 = -0.564444444444444
x5 = 0.544444444444445
x6 = 1.65333333333333
x7 = 2.76222222222222
x8 = 3.87111111111111
x9 = 4.98
xmin = -4.99
xmax = 5
n = 10
zbase = -1
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front  bdefault
#splot  'pitch-detection.dat' matrix
plot 'cmake-build-debug/asdf-log.dat' matrix using 2:1:3 with image, 'cmake-build-debug/pitches-log.dat' using 1 with lines lt rgb "#FFFFFF"
