#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	char buf[50];

	const char *hands[] = {
		"ROCK",
		"PAPER",
		"SCISSORS"
	};

	for (int i = 0; i < 300; ++i) {
		// fprintf(stderr, "Waiting for input...\n");

		// Read the line stating the game is ready for our input
		fgets(buf, sizeof(buf), stdin);
		// fprintf(stderr, buf);

		// Choose our hand
		printf("%s\n", hands[rand() % 3]);
		// printf("ROCK\n");
		fflush(stdout);
		// fprintf(stderr, "ROCK\n");

		// Check if we won, lost, or it was a tie
		fgets(buf, sizeof(buf), stdin);
		
		// if (fgets(buf, sizeof(buf), stdin) == NULL) {
		// 	fprintf(stderr, "STDIN error!\n");
		// }
		// fprintf(stderr, buf);
	}

	return 0;
}