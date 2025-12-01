# ДЗ-09: Сортировка миллиарда чисел

Реализация алгоритмов линейной сортировки: CountingSort, RadixSort, BucketSort.

## Сборка
```bash
mkdir build && cd build
cmake ..
make
```

## Запуск тестов
```bash
./test_linear_sorters
```

## Бенчмарки

### Junior Level (массивы 10²-10⁶, значения 0-999)
```bash
./benchmark_junior
```

### Middle Level (файл с 16-bit числами)
```bash
# Генерация файла (например, 10⁸ чисел ≈ 190 MB)
./generate_file 100000000 data.bin

# Запуск бенчмарка
./benchmark_middle data.bin
```

## Алгоритмы

| Алгоритм | Время | Память | Особенности |
|----------|-------|--------|-------------|
| CountingSort | O(n + k) | O(k) | Стабильный, требует знания диапазона |
| RadixSort | O(d(n + base)) | O(n + base) | Стабильный, побайтовая сортировка |
| BucketSort | O(n + k) avg | O(n + k) | Эффективен при равномерном распределении |