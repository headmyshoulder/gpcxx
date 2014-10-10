#make --jobs=8 --makefile=../../../examples/artificial_ant/ant_simulation_plots.makefile $(python -c 'for n in range(0,101): print("%04.2f.dat" % (n * 0.01));')
#(head -q -n 1 tester_pip100_* | head -n1 ; tail -q -n +2 tester_pip100_*) > ~/mnt/kalim/tmp/tester_pip100_0_00_1_00.dat                       

artificial_ant_plots_bin = /home/gchoinka/src/gc_gpcxx/build/examples/artificial_ant/artificial_ant_plots
runns = 100
file_prefix = tester_runs${runns}

%.dat :
	${artificial_ant_plots_bin} ${file_prefix}_$@ ${runns} crossover_pip_rate $(basename $@) $(basename $@) 1

