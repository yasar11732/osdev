
#include "sched.h"
#include "heap.h"

struct _task {
    void *esp;
    void *cr3;
    struct _task *next;
    task_state_t state;

    // used to free stack after
    // task terminated
    void *stack_bottom;
};

task_t *current_task = 0;
task_t *runnable_tasks = 0;
task_t *blocked_tasks = 0;
static task_t *idle_task = 0;
static task_t *waiting_termination = 0;

static inline void *cr3() {
    void *_cr3;
    __asm__ __volatile__("movl %%cr3,%0" : "=r"(_cr3) : :);
    return _cr3;
}

/*
* This will be called from main thread
* main thread will act as a idle thread
* because after main thread returns to
* initial boot script it just sleeps
*/
void initialize_multitasking()
{
    current_task = kmalloc(sizeof(task_t));
    current_task->cr3 = cr3();
    current_task->next = current_task;
    current_task->state = RUNNING;

    idle_task = current_task;
    runnable_tasks = current_task;
}

void schedule()
{
    switch_task(current_task->next);
}

static void task_normal_termination()
{
    // if a task's initial procedure
    // returns, it comes here
    current_task->next = waiting_termination;
    waiting_termination = current_task;
    
    // schedule();

    printf("A task terminated");
    while(1)
        __asm__ __volatile__("hlt");
}

task_t *new_task(void *eip)
{
    task_t *new_task = kmalloc(sizeof(task_t));
    new_task->cr3 = cr3();

    void **stack = kmalloc(1024 * sizeof(void *));
    new_task->stack_bottom = stack;

    stack[1024] = task_normal_termination;
    stack[1023] = eip;
    /*
    * ebx (1022), esi(1021), edi(1020), ebp(1019)
    * is initially undefined for a task
    */
    new_task->esp = &stack[1019];


    new_task->next = runnable_tasks->next;
    runnable_tasks->next = new_task;
    return new_task;
}

