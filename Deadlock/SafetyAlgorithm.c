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

void Safety(int ALLOCATION[Threads][Resources],
	    int NEED[Threads][Resources], int AVAILABLE[Resources], int FINISH[Resources]) 
	    
	    {
	    
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
	
	Safety(ALLOCATION, NEED, AVAILABLE, FINISH);
	
	for (int i = 0; i < Threads; i++) {
		if (FINISH[i] == 0) {
			printf("\nNo safe state\n");
			return 0;
		}
	}
	
	return 0;
}

