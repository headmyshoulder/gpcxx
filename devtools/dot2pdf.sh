dot -Tps2 $1 > tmp__.ps
ps2pdf tmp__.ps $2
rm tmp__.ps