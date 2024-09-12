cd 'D:\Users\Admin\Desktop\spin_ice_80400'
plot 'one.dat' every ::4 using 1:2:7:8 with ellipses lc 0 notitle,\
'' using 1:2:($4*80):($5*80) with vectors lw 2 lc 0 notitle,\
'two.dat' every ::4 using 1:2:7:8 with ellipses lc 2 notitle,\
'' using 1:2:($4*80):($5*80) with vectors lw 2 lc 2 notitle
exit
