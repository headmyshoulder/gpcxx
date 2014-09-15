#!/bin/bash
artificial_ant_plots_bin=./artificial_ant_plots

$artificial_ant_plots_bin population_variation.dat population_size 25 1000 25
$artificial_ant_plots_bin number_elite_variation.dat number_elite 1 100 5
$artificial_ant_plots_bin mutation_rate_variation.dat mutation_rate 0 1 0.05
$artificial_ant_plots_bin crossover_rate_variation.dat crossover_rate 0 1 0.05
$artificial_ant_plots_bin reproduction_rate_variation.dat reproduction_rate 0 1 0.05
$artificial_ant_plots_bin min_tree_height_variation.dat min_tree_height 1 10 1 max_tree_height 10 10 1
$artificial_ant_plots_bin max_tree_height_variation.dat min_tree_height 1 1 1 max_tree_height 1 10 1
$artificial_ant_plots_bin tournament_size_variation.dat tournament_size 2 50 5
