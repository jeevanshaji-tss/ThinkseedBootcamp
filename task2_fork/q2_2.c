#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    int n = 100; //limit

    int pipe_even[2];
    int pipe_odd[2];
    if (pipe(pipe_even) == -1) {
        perror("Pipe even creation failed");
        return 1;
    }
    if (pipe(pipe_odd) == -1) {
        perror("Pipe odd creation failed");
        return 1;
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        // printing even number
        close(pipe_even[0]); // Close the unused read end of the even pipe
        close(pipe_odd[1]); // Close the unused write end of the odd pipe

        for (int i = 2; i <= n; i += 2) {
        	int num;
            if (read(pipe_odd[0], &num, sizeof(num)) < 0) {
                perror("Error reading from odd pipe");
                exit(1);
            }
            printf("odd: %d\n", num);
            write(pipe_even[1], &i, sizeof(i)); // Write even number to the even pipe
            
            
        }

        close(pipe_even[1]);
        close(pipe_odd[0]);
        exit(0);
    } else if (child_pid > 0) {
        // printing odd number
        close(pipe_even[1]); // Close the unused write end of the even pipe
        close(pipe_odd[0]); // Close the unused read end of the odd pipe

        for (int i = 1; i <= n; i += 2) {
           
            write(pipe_odd[1], &i, sizeof(i)); // Write odd number to the odd pipe
            int num;
            if (read(pipe_even[0], &num, sizeof(num)) < 0) {
                perror("Error reading from even pipe");
                exit(1);
            }
            printf("even: %d\n",num);
        }

        close(pipe_even[0]);
        close(pipe_odd[1]);
       
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}

