#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int hits = 0; // Глобальная переменная

// Функция потока, которая имитирует бросание дротика
void *throw_dart(void *arg) {
    int thread_id = *(int *)arg;
    double x = ((double) rand() / (RAND_MAX + 1.0)); // случайное число от 0 до 1
    double y = ((double) rand() / (RAND_MAX + 1.0)); // случайное число от 0 до 1
    // printf("x %f \n", x);
    // printf("y %f \n", y);
    // printf("%f \n", (x * x + y * y));
    if ((x * x + y * y) <= 1) { // если точка внутри окружности
        ++hits;
        // printf("hits %d \n", hits);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // Проверка аргументов командной строки
    if (argc != 3) {
        printf("Usage: %s nthreads ntrials\n", argv[0]);
        return 1;
    }

    int nthreads = atoi(argv[1]); // количество потоков
    int ntrials = atoi(argv[2]); // общее количество попыток

    pthread_t threads[nthreads]; // массив потоков

    for (int i = 0; i < nthreads; ++i) {
        srand(time(NULL));
        pthread_create(&threads[i], NULL, throw_dart, &i);
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < nthreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Pi is approximately %.6f\n", 4 * (double)hits / ntrials);

    return 0;
}
