#include <stdio.h>

int n, m, k;

int minIndex(int memory[], int request, int visited[], int n) {

	int min = 100000;
	int index = -99;
	
	for (int i = 1; i < n; i++) {
		if (memory[i] > request && memory[i] < min && !visited[i]) {
			index = i;
			min = memory[i];
		}
	}
	visited[index] = 1;
	return index;
}

int maxIndex(int memory[], int request, int visited[], int n) {

	int max = -100000;
	int index = -99;
	
	for (int i = 1; i < n; i++) {
		if (memory[i] > request && memory[i] > max && !visited[i]) {
			index = i;
			max = memory[i];
		}
	}
	visited[index] = 1;
	return index;
}

void BestFit(int memory[], int requests[], int visited[], int n, int m) {

	for (int i = 0; i < m; i++) {
		int minBlock = minIndex(memory, requests[i], visited, n);
		printf("memory request %d is allocated to blocked %d\n", requests[i], minBlock);
	}

}

void FirstFit(int memory[], int requests[], int visited[], int n, int m) {

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (requests[i] < memory[j] && !visited[j]) {
				printf("memory request %d stored at block %d\n", requests[i], j);
				visited[j] = 1;
				break;
			}
		}
	}

}

void WorstFit(int memory[], int requests[], int visited[], int n, int m) {
	
	for (int i = 0; i < m; i++) {
		int maxBlock = maxIndex(memory, requests[i], visited, n);
		if (maxBlock == -99) {printf("memory request %d cannot be allocated to any block\n", requests[i]);}
		else {
		printf("memory request %d is allocated to blocked %d\n", requests[i], maxBlock);
	}
	}
}

int main() {

	printf("Enter no of memory blocks: ");
	scanf("%d", &n);
	
	printf("no of requests: ");
	scanf("%d", &m);

	int visited[n];
	int requests[m];
	int memory[n];
	
	for (int i = 0; i < n; i++) {
		visited[i] = 0;
	}

	printf("Enter memory structure\n");
	for (int i = 0; i < n; i++) {
		printf("Block %d: ", i);
		scanf("%d", &memory[i]);
	}
	
	printf("Enter the memory requests\n");
	for (int i = 0; i < m; i++) {
		scanf("%d", &requests[i]);
	}

	//FirstFit(memory, requests, visited, n, m);
	//BestFit(memory, requests, visited, n, m);
	WorstFit(memory, requests, visited, n, m);
	return 0;
}
