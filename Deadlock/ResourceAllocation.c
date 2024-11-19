#include <stdio.h>
#include <stdlib.h>

int Threads, Resources;
int done = 1;

void input() {

	printf("Enter no of threads: ");
	scanf("%d", &Threads);
	
	printf("Enter no of resources: ");
	scanf("%d", &Resources);
	
}

int LESS(int NEED[], int AVAILABLE[]) {

	for (int i = 0; i < Resources; i++) {
		if (NEED[i] > AVAILABLE[i]) {
			return 0;
		}
	}
	
	return 1;
}

int Safety(int ALLOCATION[Threads][Resources],
	    int NEED[Threads][Resources], int AVAILABLE[Resources], int FINISH[Resources]) 
	    
	    {
	    	printf("<");
	    	
	    	while (done) {
	    		done = 0;
	    		
	    		for (int Thread = 0; Thread < Threads; Thread++) {
	    			if (FINISH[Thread] != 1 && LESS(NEED[Thread], AVAILABLE) == 1) {
	    			
	    				done = 1;
	    				FINISH[Thread] = 1;
	    				printf("P%d ", Thread);
	    				
	    				// RESOURCE THAT THREADS RESOUCES
	    				for (int i = 0; i < Resources; i++) {
	    					AVAILABLE[i] += ALLOCATION[Thread][i];
	    				}
	    			}
	    		}
	    	}
	    	printf(">\n");
	    	for (int i = 0; i < Threads; i++) {
		if (FINISH[i] == 0) {
			return 0;
					}
		}
	    	return 1;
	    }

void ResourceRequest(int ALLOCATION[Threads][Resources], int NEED[Threads][Resources], 
		     int AVAILABLE[Resources], int FINISH[Resources]) 
		     
		     {
	
			int Request[Resources];
			int PID;
			printf("Enter request for which process: ");
			scanf("%d", &PID);
			
			for (int i = 0; i < Resources; i++) {
				printf("Enter request for resource %d: ", i);
				scanf("%d", &Request[i]);
			}
			
			if (LESS(NEED[PID], Request) == 1) {
				printf("Request is greater than need, can't be allocated\n");
				return;
			}
			
			if (LESS(Request, AVAILABLE) == 0) {
        		printf("Request cannot be fulfilled due to insufficient available resources\n");
        		return;
    			}
			
			for (int i = 0; i < Resources; i++) {
				ALLOCATION[PID][i] += Request[i];
				NEED[PID][i] -= Request[i];
				AVAILABLE[i] -= Request[i];
			}
			
			int isSafe = Safety(ALLOCATION, NEED, AVAILABLE, FINISH);
			
			if (isSafe) {
				printf("Yes, Safe state exists\n");
			}
			else {
				printf("No, Safe state does not exist\n");
				for (int i = 0; i < Resources; i++) {
				ALLOCATION[PID][i] -= Request[i];
				NEED[PID][i] += Request[i];
				AVAILABLE[i] += Request[i];
				}
			}
			
		     }

int main() {

	input();
	int ALLOCATION[Threads][Resources], MAX[Threads][Resources], NEED[Threads][Resources];
	
	int AVAILABLE[Resources]; 
	int TOTAL[Resources]; 
	int INITIAL[Resources];
	int FINISH[Resources];
	
	for (int i = 0; i < Resources; i++) {
		AVAILABLE[i] = 0;
		TOTAL[i] = 0;
		INITIAL[i] = 0;
		FINISH[i] = 0;
	}
	
	for (int Resource = 0; Resource < Resources; Resource++) {
		printf("Enter instances of resource %d: ", Resource);
		scanf("%d", &INITIAL[Resource]);
	}
	
	// INITIAL ALLOCATION
	printf("Enter initial allocation\n");
	for (int Thread = 0; Thread < Threads; Thread++) {
		for (int Resource = 0; Resource < Resources; Resource++) {
			scanf("%d", &ALLOCATION[Thread][Resource]);
			TOTAL[Resource] += ALLOCATION[Thread][Resource];	
		}	
	}
	
	// MAX
	printf("Enter max\n");
	for (int Thread = 0; Thread < Threads; Thread++) {
		for (int Resource = 0; Resource < Resources; Resource++) {
			scanf("%d", &MAX[Thread][Resource]);	
			NEED[Thread][Resource] = MAX[Thread][Resource] - ALLOCATION[Thread][Resource];
		}	
	}
	
	// CALCULATE AVAILABLE INSTANCES OF EACH RESOURCE
	for (int Resource = 0; Resource < Resources; Resource++) {
		AVAILABLE[Resource] = INITIAL[Resource] - TOTAL[Resource];
	}
	
	ResourceRequest(ALLOCATION, NEED, AVAILABLE, FINISH);
	
	return 0;
}
