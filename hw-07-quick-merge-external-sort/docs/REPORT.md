# Отчет о выполненной работе 

Реализованы алгоритмы быстрой сортировки, сортировки слиянием, реализованы ES1, ES2, ES3 алгоритмы внешней сортировки.
Написаны unit-тесты на все сортировщики, написаны все бенчмарки.
Реализовано много вспомогательных классов.

## Важно!
Пусть к тестовой папке ROOT_DIR, где будут создаваться/удаляться файлы задается в 
```cpp
include/FileSystemSetting.h
```

## Улучшения
Реализован "честный" бенчмарк: каждый сортировщик работает с одним и тем же случайным массивом данных (метод RandomArrayGenerator::regenerate).

Изменен интерфес сортировщика, теперь метод sort() возвращает void.
А все необходимые события реализуются за счет слушателей.
Например, для подсчета времени сортировки используется DurationSortListener, для логирования LoggingSortListener.

В прошлых ДЗ не переделывал сортировщики на новый интерфейс.

## Результаты бенчмарка Junior
```cpp
demo/benchmark_sorters.cpp
```

Если выставить таймаут 2 минуты как в тебованиях, то бенчмарк останавливается на 1 млн записей.

      Size  QuickSort(ms)  MergeSort(ms)
========================================
       100              0              0
      1000              2              3
     10000             67             41
    100000           5447            481
   1000000        timeout              -

Если выставить таймаут 10 минут, то отрабатывают оба алгоритма сортировки.

         Size  QuickSort(ms)  MergeSort(ms)
========================================
       100              0              0
      1000              2              4
     10000             66             40
    100000           5437            476
   1000000         518551           5449

 ## Результаты бенчмарка Middle
```cpp
demo/benchmark_external_sorters.cpp
```

 Таймаут выставил 30 минут.

  N         T      Sorter  Time          
==================================================
100       10        ES1     1.4 ms        
100       10        ES2     301.4 µs     
100       10        ES3     249.3 µs     
100       100       ES1     636.8 µs     
100       100       ES2     275.0 µs     
100       100       ES3     278.1 µs     
1000      10        ES1     7.5 ms        
1000      10        ES2     2.8 ms        
1000      10        ES3     1.8 ms        
1000      1000      ES1     5.3 ms        
1000      1000      ES2     1.2 ms        
1000      1000      ES3     1.1 ms        
10000     10        ES1     203.5 ms      
10000     10        ES2     166.4 ms      
10000     10        ES3     11.2 ms       
10000     10000     ES1     49.2 ms       
10000     10000     ES2     11.6 ms       
10000     10000     ES3     12.3 ms       
100000    10        ES1     16.4 s        
100000    10        ES2     16.1 s        
100000    10        ES3     128.7 ms      
100000    100000    ES1     517.1 ms      
100000    100000    ES2     153.9 ms      
100000    100000    ES3     149.8 ms      
1000000   10        ES1     26 m 19.0 s   
1000000   10        ES2     25 m 56.0 s   
1000000   10        ES3     1.6 s         
1000000   1000000   ES1     6.6 s         
1000000   1000000   ES2     1.8 s         
1000000   1000000   ES3     1.9 s 

## Результаты бенчмарка Senior
```cpp
demo/benchmark_sorters_extended.cpp
```

 Size        Type      Sorter      Duration            Success 
==============================================================
1           random    QuickSort   525.7 µs           yes     
1           random    MergeSorter 471.0 µs           yes     
1           digits    QuickSort   400.6 µs           yes     
1           digits    MergeSorter 253.9 µs           yes     
1           sorted    QuickSort   144.4 µs           yes     
1           sorted    MergeSorter 146.5 µs           yes     
1           revers    QuickSort   154.9 µs           yes     
1           revers    MergeSorter 182.6 µs           yes     
10          random    QuickSort   207.3 µs           yes     
10          random    MergeSorter 284.0 µs           yes     
10          digits    QuickSort   221.2 µs           yes     
10          digits    MergeSorter 209.2 µs           yes     
10          sorted    QuickSort   206.9 µs           yes     
10          sorted    MergeSorter 211.6 µs           yes     
10          revers    QuickSort   224.1 µs           yes     
10          revers    MergeSorter 202.4 µs           yes     
100         random    QuickSort   223.5 µs           yes     
100         random    MergeSorter 348.8 µs           yes     
100         digits    QuickSort   261.4 µs           yes     
100         digits    MergeSorter 393.1 µs           yes     
100         sorted    QuickSort   359.1 µs           yes     
100         sorted    MergeSorter 308.7 µs           yes     
100         revers    QuickSort   345.4 µs           yes     
100         revers    MergeSorter 331.8 µs           yes     
1000        random    QuickSort   1.2 ms              yes     
1000        random    MergeSorter 3.1 ms              yes     
1000        sorted    MergeSorter 3.5 ms              yes     
1000        revers    MergeSorter 3.1 ms              yes     
1000        digits    QuickSort   6.8 ms              yes     
1000        digits    MergeSorter 6.8 ms              yes     
1000        sorted    QuickSort   24.8 ms             yes     
10000       random    QuickSort   29.8 ms             yes     
10000       revers    MergeSorter 47.8 ms             yes     
10000       digits    MergeSorter 56.3 ms             yes     
1000        revers    QuickSort   57.4 ms             yes     
10000       sorted    MergeSorter 55.2 ms             yes     
10000       random    MergeSorter 57.9 ms             yes     
10000       digits    QuickSort   213.5 ms            yes     
100000      random    QuickSort   232.4 ms            yes     
100000      revers    MergeSorter 286.2 ms            yes     
100000      digits    MergeSorter 376.6 ms            yes     
100000      random    MergeSorter 439.9 ms            yes     
10000       sorted    QuickSort   1.1 s               yes     
10000       revers    QuickSort   1.7 s               yes     
100000      digits    QuickSort   timeout             no      
100000      revers    QuickSort   timeout             no      
100000      sorted    MergeSorter timeout             no      
100000      sorted    QuickSort   timeout             no 