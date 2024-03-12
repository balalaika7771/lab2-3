#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>

void GenerateDataset(const char* filename, int num) {
    std::ofstream file(filename);
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < num; ++i) {
        double number = (rand() % 1000) + (rand() % 10) / 10.0; // Вещественные числа с 3 цифрами и 1 дробной
        file << std::fixed << std::setprecision(1) << number << "\n";
    }

    file.close();
}

void Merge(float* left, int leftCount, float* right, int rightCount, float* result, int& comparisonCount) {
    int i = 0, j = 0, k = 0;

    while (i < leftCount && j < rightCount) {
        comparisonCount++;
        if (left[i] < right[j]) result[k++] = left[i++];
        else result[k++] = right[j++];
    }
    while (i < leftCount) result[k++] = left[i++];
    while (j < rightCount) result[k++] = right[j++];
}

void MergeSort(float* arr, int num, int& comparisonCount) {
    if (num < 2) return; // Базовый случай

    int mid = num / 2;
    float* left = new float[mid];
    float* right = new float[num - mid];

    // Копирование данных во временные подмассивы
    for (int i = 0; i < mid; i++) left[i] = arr[i];
    for (int i = mid; i < num; i++) right[i - mid] = arr[i];

    MergeSort(left, mid, comparisonCount); // Сортировка левой части
    MergeSort(right, num - mid, comparisonCount); // Сортировка правой части
    Merge(left, mid, right, num - mid, arr, comparisonCount); // Слияние обеих частей

    delete[] left;
    delete[] right;
}

void EvaluateSort() {
    const int nums[] = { 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
    std::ofstream file("evaluation.txt");

    // Форматирование заголовка таблицы
    file << std::left << std::setw(10) << "num" << std::setw(10) << "ТЭ" << std::setw(15) << "Т1"
        << std::setw(15) << "Т2" << std::setw(15) << "ТЭ/Т1" << std::setw(15) << "ТЭ/Т2" << "\n";

    for (int num : nums) {
        GenerateDataset("dataset.txt", num);
        float* arr = new float[num];
        std::ifstream dataset("dataset.txt");
        for (int i = 0; i < num; i++) dataset >> arr[i];
        dataset.close();

        int comparisonCount = 0;
        MergeSort(arr, num, comparisonCount);

        double T1 = static_cast<double>(num) *   static_cast<double>(num);
        double T2 = static_cast<double>(num) * log2(static_cast<double>(num));

        // Форматирование и вывод результатов в файл
        file << std::left << std::setw(10) << num
            << std::setw(10) << comparisonCount
            << std::setw(15) << std::fixed << std::setprecision(2) << T1
            << std::setw(15) << T2
            << std::setw(15) << comparisonCount / T1
            << std::setw(15) << comparisonCount / T2 << "\n";

        delete[] arr;
    }

    file.close();
}

int main() {
    EvaluateSort();
    return 0;
}
