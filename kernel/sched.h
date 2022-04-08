
#ifndef _SCHED_H
#define _SCHED_H

typedef enum {RUNNING, RUNNABLE, BLOCKED, WAITINGTERMINATION} task_state_t;

typedef struct _task task_t;

void initialize_multitasking(void);
void switch_task(task_t *next_task);
task_t *new_task(void *eip);
void schedule();

#endif