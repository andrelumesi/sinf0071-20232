#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SIZE 4096  // Tamanho da região de memória compartilhada

int main() {
    int *shared_memory;

    // Criar região de memória compartilhada
    shared_memory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("Falha ao criar a região de memória compartilhada");
        exit(EXIT_FAILURE);
    }

    // Cria novo processo
    pid_t pid = fork();

    if (pid == -1) {
        perror("Falha ao criar o processo filho");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Processo pai
        printf("Processo pai (PID %d)\n", getpid());
        *shared_memory = 123;
        wait(NULL);

        printf("Processo pai: Valor lido da memória compartilhada: %d\n", *shared_memory);
    } else {
        // Processo filho
        printf("Processo filho (PID %d)\n", getpid());
        sleep(1);
        printf("Processo filho: Valor lido da memória compartilhada: %d\n", *shared_memory);
    }

    // Libera a região de memória compartilhada
    if (munmap(shared_memory, SIZE) == -1) {
        perror("Falha ao liberar a região de memória compartilhada");
        exit(EXIT_FAILURE);
    }

    return 0;
}
