#include "BucketSorter.hpp"
#include <algorithm>
#include <cmath>

BucketSorter::BucketSorter(size_t bucketCount) : bucketCount_(bucketCount) {}

template<typename T>
void BucketSorter::bucketSort(std::vector<T>& arr, T maxValue) {
    const size_t n = arr.size();
    
    // Определяем количество корзин
    size_t numBuckets = bucketCount_;
    if (numBuckets == 0) {
        // Эвристика: sqrt(n) корзин, но не меньше 1 и не больше maxValue+1
        numBuckets = std::max(size_t(1), 
                     std::min(static_cast<size_t>(std::sqrt(n)), 
                              static_cast<size_t>(maxValue) + 1));
    }

    // Создаём корзины
    std::vector<std::vector<T>> buckets(numBuckets);

    // Размер диапазона для каждой корзины
    // bucketRange = (maxValue + 1) / numBuckets (с округлением вверх)
    T range = static_cast<T>((static_cast<size_t>(maxValue) + numBuckets) / numBuckets);
    if (range == 0) range = 1;

    // Распределяем элементы по корзинам
    for (const auto& val : arr) {
        size_t bucketIdx = static_cast<size_t>(val) / range;
        if (bucketIdx >= numBuckets) {
            bucketIdx = numBuckets - 1;
        }
        buckets[bucketIdx].push_back(val);
    }

    // Сортируем каждую корзину
    for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
    }

    // Объединяем корзины
    size_t idx = 0;
    for (const auto& bucket : buckets) {
        for (const auto& val : bucket) {
            arr[idx++] = val;
        }
    }
}

void BucketSorter::sort(std::vector<uint16_t>& arr) {
    if (arr.size() <= 1) {
        return;
    }

    bucketSort<uint16_t>(arr, 65535);
}

void BucketSorter::sort(std::vector<uint32_t>& arr, uint32_t maxValue) {
    if (arr.size() <= 1) {
        return;
    }

    if (maxValue == 0 && !arr.empty()) {
        maxValue = *std::max_element(arr.begin(), arr.end());
    }

    bucketSort<uint32_t>(arr, maxValue);
}