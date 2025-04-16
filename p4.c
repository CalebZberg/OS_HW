#include <stdio.h>

typedef struct {
	int arrival_time;
	int burst_time;
	int remaining_time;
	int start_time;
	int finish_time;
	int responded;
} Process;

Process processes[100];
int num_processes = 0;


int fcfs(){ // first come first serve

}

int sjf(){ // shortest job first

}

int srtf(){ // shortest remaining time first

}

int rr() // round robin

}





int main(){

	while (scanf("%d %d", &processes[num_processes].arrival_time, &processes[num_processes].burst_time == 2) {

		processes[num_processes].remaining_time = processes[num_processes].burst_time;
		processes[num_processes].start_time = -1;
		processes[num_processes].finish_time = -1;
		processes[num_processes].responded = 0;

    		num_processes++;
	}










	return 0;
}
