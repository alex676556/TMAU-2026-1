#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include "models.h"

std::vector<double> generateInputSignal(int type, int steps) {
    std::vector<double> u(steps, 0.0);
    for (int t = 0; t < steps; ++t) {
        if (type == 1) {
            u[t] = 1.0;
        } else if (type == 2) {
            u[t] = (t == 0) ? 1.0 : 0.0; 
        } else if (type == 3) {
            u[t] = std::sin(static_cast<double>(t));
        }
    }
    return u;
}

void saveToCSV(const std::string& filename, const std::vector<double>& input, const std::vector<double>& output) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    file << "Time,Input,Output\n";
    for (size_t t = 0; t < input.size(); ++t) {
        file << t << "," << input[t] << "," << output[t] << "\n";
    }
    file.close();
    std::cout << "Saved: " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    int steps = 100; 

    if (argc > 1) {
        try {
            int parsed = std::stoi(argv[1]);
            if (parsed > 0 && parsed <= 10000) {
                steps = parsed;
            }
        } catch (...) {
            std::cerr << "Некорректный аргумент командной строки. Используется значение по умолчанию." << std::endl;
            steps = 100;
        }
    } else {
        std::cout << "Введите количество шагов симуляции (n) [по умолчанию 100]: ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            try {
                int parsed = std::stoi(input);
                if (parsed > 0 && parsed <= 10000) {
                    steps = parsed;
                }
            } catch (...) {
                std::cout << "Некорректный ввод. Будет использовано значение по умолчанию (100)." << std::endl;
                steps = 100;
            }
        }
    }

    if (steps <= 0) {
        std::cerr << "Количество шагов должно быть больше 0! Установлено значение по умолчанию: 100." << std::endl;
        steps = 100;
    }

    std::cout << "Запуск симуляции для n = " << steps << " шагов...\n" << std::endl;

    LinearModel1_4 linearModel(0.8, 0.2);
    NonlinearModel2_8 nonlinearModel(0.5, 0.3, 0.1);
    DiffModel3_2 diffModel(0.5, 0.1); 

    std::vector<std::string> signalNames = {"step", "impulse", "harmonic"};

    for (int sigType = 1; sigType <= 3; ++sigType) {
        auto input = generateInputSignal(sigType, steps);

        linearModel.reset();
        auto outLinear = linearModel.simulate(input);
        saveToCSV("linear_" + signalNames[sigType - 1] + ".csv", input, outLinear);

        nonlinearModel.reset();
        auto outNonlinear = nonlinearModel.simulate(input);
        saveToCSV("nonlinear_" + signalNames[sigType - 1] + ".csv", input, outNonlinear);

        diffModel.reset();
        auto outDiff = diffModel.simulate(input);
        saveToCSV("diff_" + signalNames[sigType - 1] + ".csv", input, outDiff);
    }

    std::cout << "\nSimulation completed successfully!" << std::endl;
    return 0;
}