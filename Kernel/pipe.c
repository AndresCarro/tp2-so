#include <pipe.h>

Pipe * pipe_open() {
    Pipe * pipe = memory_manager_alloc(sizeof(Pipe));
    pipe->read_offset = 0;
    pipe->write_offset = 0;
    pipe->open_for_read = 1;
    pipe->open_for_write = 1;
    pipe->blocked_in_read = new_blocked_queue();
    pipe->blocked_in_write = new_blocked_queue();

    return pipe;
}

void pipe_close(Pipe * pipe, char close_write_end) {
    if (close_write_end) {
        pipe->open_for_write--;
    } else {
        pipe->open_for_read--;
    }
    
    if (pipe->open_for_read == 0 && pipe->open_for_write == 0) {
        memory_manager_free(pipe);
    }
}

int pipe_write(Pipe * p, char * str, int length) {
    for (int i = 0; i < length; i++) {
        if (p->open_for_read == 0) {
            return -1;
        }
        while (p->write_offset == p->read_offset + PIPESIZE) {
            pid_t pid;
            while ((pid = dequeue_pid(p->blocked_in_read)) != -1) {
                unblock_process(pid);
            }
            pid = get_current_pid();
            enqueue_pid(p->blocked_in_write, pid);
            block_process(pid);
        }
        p->data[p->write_offset % PIPESIZE] = str[i];
        p->write_offset++;
    }
    pid_t pid;
    while ((pid = dequeue_pid(p->blocked_in_read)) != -1) {
        unblock_process(pid);
    }
    return length;
}

int pipe_read(Pipe * p, char * str, int length) {
    while (p->read_offset == p->write_offset && p->open_for_write) {
        pid_t pid = get_current_pid();
        enqueue_pid(p->blocked_in_read, pid);
        block_process(pid);
    }
    int i;
    for (i = 0; p->read_offset != p->write_offset && i < length; i++) {
        str[i] = p->data[p->read_offset % PIPESIZE];
        p->read_offset++;
    }
    pid_t pid;
    while ((pid = dequeue_pid(p->blocked_in_write)) != -1) {
        unblock_process(pid);
    }
    return i;
}