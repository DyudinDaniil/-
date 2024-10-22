// пример ввода в терминал - ./mandelbrot 4 1000
// ./mandelbrot — это команда для запуска
// 4 — это значение параметра nthreads
// 1000 — это значение параметра npoints

#include <iostream>
#include <fstream>
#include <complex>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

std::mutex fileMutex; 

bool isInMandelbrot(std::complex<double> c, int npoints) {
    std::complex<double> z(0);
    for (int i = 0; i < npoints; ++i) {
        if (std::abs(z) > 2) {
            return false;
        }
        z = z * z + c;
    }
    return true; 
}

void computeMandelbrot(int start, int end, int npoints, std::ofstream &outputFile) {
    for (int x = start; x < end; ++x) {
        double real = (x / 1000.0) * 3.0 - 2.0; 
        for (int y = -500; y < 500; ++y) {
            std::complex<double> c(real, y / 1000.0);
            if (isInMandelbrot(c, npoints)) {
                std::lock_guard<std::mutex> guard(fileMutex);
                outputFile << real << "," << (y / 1000.0) << "\n"; }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <nthreads> <npoints>" << std::endl;
        return 1;
    }

    int nthreads = std::stoi(argv[1]); 
    int npoints = std::stoi(argv[2]); 
    const int totalPoints = 1000;  
    std::ofstream outputFile("mandelbrot.csv"); 
    if (!outputFile.is_open()) {
        std::cerr << "Файл не открывается" << std::endl;
        return 1;
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Начало вычислений" << std::endl;

    outputFile << "x,y\n"; std::vector<std::thread> threads;
    int pointsPerThread = totalPoints / nthreads;

    for (int i = 0; i < nthreads; ++i) {
        int start = i * pointsPerThread;
        int end = (i == nthreads - 1) ? totalPoints : start + pointsPerThread;
        threads.emplace_back(computeMandelbrot, start, end, npoints, std::ref(outputFile));
    }

    for (auto &t : threads) {
        t.join();
    }

    outputFile.close();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Вычисления завершены. Время выполнения: " << std::chrono::duration<double>(endTime - startTime).count() << " секунд." << std::endl;

    return 0;
}

