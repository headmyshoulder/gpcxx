#!/bin/bash
artificial_ant_plots_bin=$1
do_avg_over=5

default_args="$default_args population_size 500 500 1"
default_args="$default_args generation_max 1000 1000 1"
default_args="$default_args number_elite 2 2 1"
default_args="$default_args mutation_rate 0 0 1"
default_args="$default_args crossover_rate 0.9 0.9 1"
default_args="$default_args reproduction_rate 0.1 0.1 1"
default_args="$default_args min_tree_height 3 3 1"
default_args="$default_args init_max_tree_height 6 6 1"
default_args="$default_args max_tree_height 17 17 1"
default_args="$default_args tournament_size 2 2 1"

$artificial_ant_plots_bin population_variation.dat $do_avg_over $default_args population_size 25 1000 1
#$artificial_ant_plots_bin number_elite_variation.dat $default_args number_elite 1 100 5
#$artificial_ant_plots_bin mutation_rate_variation.dat $default_args mutation_rate 0 1 0.05
#$artificial_ant_plots_bin crossover_rate_variation.dat $default_args crossover_rate 0 1 0.05
#$artificial_ant_plots_bin reproduction_rate_variation.dat $default_args reproduction_rate 0 1 0.05
#$artificial_ant_plots_bin min_tree_height_variation.dat $default_args min_tree_height 1 10 1 max_tree_height 10 10 1
#$artificial_ant_plots_bin max_tree_height_variation.dat $default_args min_tree_height 1 1 1 max_tree_height 1 10 1
#$artificial_ant_plots_bin tournament_size_variation.dat $default_args tournament_size 2 50 5
