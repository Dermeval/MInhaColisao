#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Function to be executed by the first thread
void* incrementing_number(void* arg) {
    int num = 0;
    while (1) {
        printf("Number: %d\n", num++);
        sleep(1); // Wait for 1 second
    }
    return NULL;
}

// Function to be executed by the second thread
void* print_hello_world(void* arg) {
    while (1) {
        printf("hello world\n");
        sleep(1); // Wait for 1 second
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Create the first thread
    if (pthread_create(&thread1, NULL, incrementing_number, NULL) != 0) {
        perror("Failed to create thread1");
        return 1;
    }

    // Create the second thread
    if (pthread_create(&thread2, NULL, print_hello_world, NULL) != 0) {
        perror("Failed to create thread2");
        return 1;
    }

    // Wait for the threads to finish (they won't in this case, as they run indefinitely)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}