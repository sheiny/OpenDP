set exp_folder or-low-util-test-03
set design ibex_core 

set TIME_start [clock clicks -milliseconds]

set odp [get_opendp]
read_lef nangate45-bench/tech/NangateOpenCellLibrary.lef
read_def nangate45-bench/${design}/${design}_replace.def

legalize_placement

set TIME_taken [expr [clock clicks -milliseconds] - $TIME_start]

set fp [open ${exp_folder}/${design}.rpt w]
set legality [$odp check_legality]

puts $fp "Legality          : $legality"
puts $fp "Runtime           : $TIME_taken"
puts $fp "Sum displacement: : [$odp get_sum_displacement]"
puts $fp "Avg displacement: : [$odp get_average_displacement]"
puts $fp "Max displacement: : [$odp get_max_displacement]"
puts $fp "Original HPWL     : [$odp get_original_hpwl]"
puts $fp "Legalized HPWL    : [$odp get_legalized_hpwl]"

close $fp

exit