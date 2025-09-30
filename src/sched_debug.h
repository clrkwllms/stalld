/* SPDX-License-Identifier: GPL-2.0-or-later */
#define TASK_MARKER	"runnable tasks:"
#define TASK_DIVIDER	"-\n"

/*
 * Over time, the various 'runnable task' output in SCHED_DEBUG has
 * changed significantly.
 *
 * Depending on the version of the running kernel, the task formats can
 * differ greatly.
 *
 * For example, in 3.X kernels, the sched_debug running tasks format denotes the current
 * running task on the current CPU with a singular state label, 'R'. Other tasks do not
 * receive a state label.
 *
 * example:
 * '          task   PID         tree-key  switches  prio     wait-time             sum-exec        sum-sleep'
 * ' ----------------------------------------------------------------------------------------------------------'
 * '      watchdog/5    33        -8.984472       151     0         0.000000         0.535614         0.000000 0 /'
 * ' R          less  9542      2382.087644        56   120         0.000000        16.444493         0.000000 0 /'
 *
 * In 4.18+ kernels, the sched_debug format running tasks format included an additional 'S'
 * state field to denote the state of the running tasks on said CPU.
 *
 * example:
 * ' S           task   PID         tree-key  switches  prio     wait-time             sum-exec        sum-sleep'
 * '-----------------------------------------------------------------------------------------------------------'
 * ' I         rcu_gp     3        13.973264         2   100         0.000000         0.004469         0.000000 0 0 /'
 *
 * Introduced in 6.12+, 2cab4bd024d2 sched/debug: Fix the runnable tasks
 * output, the sched_debug running tasks format was changed to include
 * four new EEVDF fields.
 *
 * Example:
 *  'S            task   PID       vruntime   eligible    deadline             slice          sum-exec      switches  prio         wait-time        sum-sleep       sum-block  node   group-id  group-path'
 *  '-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------'
 *  ' I kworker/R-rcu_g     4        -1.048576   E          -1.040501           0.700000         0.000000         2     100         0.000000         0.000000         0.000000   0      0        /'
 *
 * As there are considerable differences in the location of the fields
 * needed to boost task prioriy, handle the logical code differences with
 * an enumerated type.
 */
enum task_format {
	TASK_FORMAT_UNKNOWN =0,
	OLD_TASK_FORMAT,	// 3.10 kernel
	NEW_TASK_FORMAT,	// 4.18+ kernel
	TASK_FORMAT_LIMIT
};


/*
 * set of offsets in a task format line based on offsets
 * discovered by discover_task_format
 *
 * Note: These are *NOT* character offsets, these are "word" offsets.
 * Requiring consumers of this struct to parse through the individual
 * lines.
 */
struct task_format_offsets {
	int task;
	int pid;
	int switches;
	int prio;
	int wait_time;
};

extern struct stalld_backend sched_debug_backend;
