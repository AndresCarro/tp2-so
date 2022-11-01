#include <test_util.h>
#include <syslib.h>

enum State {RUNNING, BLOCKEDD, KILLED};

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

// int64_t test_processes2(int argc, char * argv[]) {
//   uint8_t rq;
//   uint8_t alive = 0;
//   uint8_t action;
//   uint64_t max_processes;
//   char * name = "Endless Loop";
//   char * argvAux[] = {name};

//   if (argc != 2) return -1;

//   if ((max_processes = atoi(argv[1])) <= 0) return -1;

//   p_rq p_rqs[max_processes];

//     int j = 0;
//   while (j < 10) {
//     // Create max_processes processes
//     for(rq = 0; rq < max_processes; rq++){

//       p_rqs[rq].pid = exec((uint64_t) endless_loop, 1, argvAux);

//       if (p_rqs[rq].pid == -1) {
//         fprintf(STDOUT, "test_processes: ERROR creating process\n");
//         return -1;
//       }else{
//         p_rqs[rq].state = RUNNING;
//         alive++;
//       }
//     }

//     // Randomly kills, blocks or unblocks processes until every one has been killed
//     while (alive > 0){

//       for(rq = 0; rq < max_processes; rq++){
//         action = GetUniform(100) % 2; 

//         switch(action) {
//           case 0:
//             if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKEDD){
//               if (kill(p_rqs[rq].pid) == -1){  
//                 fprintf(STDOUT, "test_processes: ERROR killing process\n");
//                 return -1;
//               }
//               p_rqs[rq].state = KILLED; 
//               alive--;
//             }
//             break;

//           case 1:
//             if (p_rqs[rq].state == RUNNING){
//               if(block(p_rqs[rq].pid) == -1){
//                 fprintf(STDOUT, "test_processes: ERROR blocking process\n");
//                 return -1;
//               }
//               p_rqs[rq].state = BLOCKEDD; 
//             }
//             break;
//         }
//       }

//       // Randomly unblocks processes
//       for(rq = 0; rq < max_processes; rq++)
//         if (p_rqs[rq].state == BLOCKEDD && GetUniform(100) % 2){
//           if(unblock(p_rqs[rq].pid) == -1){
//             fprintf(STDOUT, "test_processes: ERROR unblocking process\n");
//             return -1;
//           }
//           p_rqs[rq].state = RUNNING; 
//         }
//     } 
//     j++;
//   }
//     fprintf(STDOUT, "All good with test_processes. ;)\n");
// }