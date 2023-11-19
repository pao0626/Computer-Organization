#define macro_baseline_cycle_count  baseline_cycle_count = arith_CPI*arith_cnt+lw_CPI*lw_cnt+sw_CPI*sw_cnt+others_CPI*others_cnt;
#define macro_baseline_cpu_time baseline_cpu_time = cycle_time*baseline_cycle_count;
#define macro_improved_cycle_count improved_cycle_count = arith_CPI*arith_cnt+lw_CPI*lw_cnt+sw_CPI*sw_cnt+others_CPI*others_cnt;
#define macro_improved_cpu_time improved_cpu_time = cycle_time*improved_cycle_count;
