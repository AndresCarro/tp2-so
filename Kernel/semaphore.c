#include <semaphore.h>

extern int spinlock(uint8_t * lock);
extern void unlock(uint8_t * lock);

SemList semaphores = NULL;

SemNode * find_semaphore(char * name, SemNode ** previous_sem) {
    SemNode * current = semaphores;
    SemNode * previous = NULL;
    while (current != NULL) {
        if (strcmp(current->sem.name, name) == 0) {
            if (previous_sem != NULL) {
                *previous_sem = previous;
            }
            return current;
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

sem_t sem_open(const char * name, uint8_t initial_value) {
    SemNode * sem = find_semaphore(name, NULL);

    if (sem != NULL) {
        sem->sem.linked_processes++;
        return &(sem->sem);
    }

    SemNode * new_sem = memory_manager_alloc(sizeof(SemNode));

    new_sem->sem.name = strcpy(name);
    new_sem->sem.value = initial_value;
    new_sem->sem.linked_processes = 1;
    new_sem->sem.lock = 0;
    new_sem->sem.blocked_processes = new_blocked_queue();
    new_sem->next = semaphores;
    semaphores = new_sem;

    return &(new_sem->sem);
}

void sem_close(sem_t sem) {
    SemNode * previous;
    SemNode * current = find_semaphore(sem->name, &previous);

    if (current == NULL) {
        return;
    }

    if (sem->linked_processes > 1) {
        sem->linked_processes--;
    }

    if (previous == NULL) {
        semaphores = current->next;
    } else {
        previous->next = current->next;
    }

    free_queue(sem->blocked_processes);
    memory_manager_free(sem->name);
    memory_manager_free(current);
}

int sem_post(sem_t sem) {
    SemNode * current = find_semaphore(sem->name, NULL);

    if (current == NULL) {
        return -1;
    }

    spinlock(&(sem->lock));

    if (!is_empty(sem->blocked_processes)) {
        pid_t pid = dequeue_pid(sem->blocked_processes);
        unblock_process(pid);
    } else {
        sem->value++;
    }

    unlock(&(sem->lock));
    return 0;
}

int sem_wait(sem_t sem) {
    SemNode * current = find_semaphore(sem->name, NULL);

    if (current == NULL) {
        return -1;
    }

    spinlock(&(sem->lock));

    if (sem->value > 0) {
        sem->value--;
        unlock(&(sem->lock));
    } else {
        pid_t pid = get_current_pid();
        enqueue_pid(sem->blocked_processes, pid);
        unlock(&(sem->lock));
        block_process(pid);
    }
    return 0;
}

SemInfo * sem_info() {
    SemList current = semaphores;
    SemInfo * info = NULL;
    while (current != NULL) {
        SemInfo * new_info = memory_manager_alloc(sizeof(SemInfo));
        new_info->name = strcpy(current->sem.name);
        new_info->value = current->sem.value;
        new_info->linked_processes = current->sem.linked_processes;
        new_info->blocked_processes = get_blocked_qty(current->sem.blocked_processes);
        new_info->next = info;
        info = new_info;
        current = current->next;
    }
    return info;
}