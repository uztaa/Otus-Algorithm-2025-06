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

Переписал сортировщики из прошлого ДЗ на новый интерфейс.
Добавл их в бенчмарк итоговый (SENIOR).

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
Настройка PATH_TO_SAVE_FILES в бенчмарке отвечает за путь к сохраняемым файлам.
Флаг включения сохранения отсортированных файлов SAVE_FILE_ENABLE (по умолчанию выключен, т.е. false).

Для настройки продолжительности бенчмарка (TIMEOUT_NS) за 1 час получаем такую картину:

Size        Type      Sorter                          Duration            Success   
====================================================================================
1           random    QuickSorter                     2.4 µs             yes       
1           random    MergeSorter                     1.9 µs             yes       
1           random    BinaryInsertionSorter           1.9 µs             yes       
1           random    CocktailSorter                  1.9 µs             yes       
1           random    InsertionSorter                 1.6 µs             yes       
1           random    BubbleSorter                    1.9 µs             yes       
1           random    InsertionShiftSorter            962 ns              yes       
1           random    ShellSorter.Classic             1.7 µs             yes       
1           random    ShellSorter.Hibbard             3.1 µs             yes       
1           random    ShellSorter.Sedgewick           621 ns              yes       
1           digits    QuickSorter                     832 ns              yes       
1           digits    MergeSorter                     1.3 µs             yes       
1           digits    BinaryInsertionSorter           691 ns              yes       
1           digits    CocktailSorter                  861 ns              yes       
1           digits    InsertionSorter                 871 ns              yes       
1           digits    BubbleSorter                    902 ns              yes       
1           digits    InsertionShiftSorter            672 ns              yes       
1           digits    ShellSorter.Classic             571 ns              yes       
1           digits    ShellSorter.Hibbard             1.6 µs             yes       
1           digits    ShellSorter.Sedgewick           531 ns              yes       
1           sorted    QuickSorter                     721 ns              yes       
1           sorted    MergeSorter                     732 ns              yes       
1           sorted    BinaryInsertionSorter           671 ns              yes       
1           sorted    CocktailSorter                  792 ns              yes       
1           sorted    InsertionSorter                 371 ns              yes       
1           sorted    BubbleSorter                    611 ns              yes       
1           sorted    InsertionShiftSorter            872 ns              yes       
1           sorted    ShellSorter.Classic             631 ns              yes       
1           sorted    ShellSorter.Hibbard             982 ns              yes       
1           sorted    ShellSorter.Sedgewick           601 ns              yes       
1           revers    QuickSorter                     391 ns              yes       
1           revers    MergeSorter                     221 ns              yes       
1           revers    BinaryInsertionSorter           461 ns              yes       
1           revers    CocktailSorter                  641 ns              yes       
1           revers    InsertionSorter                 330 ns              yes       
1           revers    BubbleSorter                    341 ns              yes       
1           revers    InsertionShiftSorter            511 ns              yes       
1           revers    ShellSorter.Classic             762 ns              yes       
1           revers    ShellSorter.Hibbard             952 ns              yes       
1           revers    ShellSorter.Sedgewick           752 ns              yes       
10          random    QuickSorter                     2.4 µs             yes       
10          random    MergeSorter                     13.8 µs            yes       
10          random    BinaryInsertionSorter           3.1 µs             yes       
10          random    CocktailSorter                  3.6 µs             yes       
10          random    InsertionSorter                 2.8 µs             yes       
10          random    BubbleSorter                    4.5 µs             yes       
10          random    InsertionShiftSorter            3.3 µs             yes       
10          random    ShellSorter.Classic             2.9 µs             yes       
10          random    ShellSorter.Hibbard             3.7 µs             yes       
10          random    ShellSorter.Sedgewick           4.8 µs             yes       
10          digits    QuickSorter                     2.3 µs             yes       
10          digits    MergeSorter                     9.6 µs             yes       
10          digits    BinaryInsertionSorter           2.8 µs             yes       
10          digits    CocktailSorter                  4.4 µs             yes       
10          digits    InsertionSorter                 2.5 µs             yes       
10          digits    BubbleSorter                    8.0 µs             yes       
10          digits    InsertionShiftSorter            2.8 µs             yes       
10          digits    ShellSorter.Classic             4.2 µs             yes       
10          digits    ShellSorter.Hibbard             3.6 µs             yes       
10          digits    ShellSorter.Sedgewick           3.3 µs             yes       
10          sorted    QuickSorter                     3.0 µs             yes       
10          sorted    MergeSorter                     9.0 µs             yes       
10          sorted    BinaryInsertionSorter           1.6 µs             yes       
10          sorted    CocktailSorter                  1.2 µs             yes       
10          sorted    InsertionSorter                 671 ns              yes       
10          sorted    BubbleSorter                    2.2 µs             yes       
10          sorted    InsertionShiftSorter            1.1 µs             yes       
10          sorted    ShellSorter.Classic             2.8 µs             yes       
10          sorted    ShellSorter.Hibbard             2.1 µs             yes       
10          sorted    ShellSorter.Sedgewick           1.9 µs             yes       
10          revers    QuickSorter                     2.4 µs             yes       
10          revers    MergeSorter                     22.4 µs            yes       
10          revers    BinaryInsertionSorter           3.2 µs             yes       
10          revers    CocktailSorter                  4.0 µs             yes       
10          revers    InsertionSorter                 3.9 µs             yes       
10          revers    BubbleSorter                    3.7 µs             yes       
10          revers    InsertionShiftSorter            4.1 µs             yes       
10          revers    ShellSorter.Classic             3.5 µs             yes       
10          revers    ShellSorter.Hibbard             4.0 µs             yes       
10          revers    ShellSorter.Sedgewick           5.6 µs             yes       
100         random    QuickSorter                     36.6 µs            yes       
100         random    MergeSorter                     143.9 µs           yes       
100         random    BinaryInsertionSorter           119.3 µs           yes       
100         random    InsertionSorter                 1.8 ms              yes       
100         random    CocktailSorter                  2.1 ms              yes       
100         random    BubbleSorter                    1.0 ms              yes       
100         random    ShellSorter.Classic             57.6 µs            yes       
100         random    InsertionShiftSorter            263.9 µs           yes       
100         random    ShellSorter.Hibbard             57.6 µs            yes       
100         random    ShellSorter.Sedgewick           45.4 µs            yes       
100         digits    QuickSorter                     57.8 µs            yes       
100         digits    MergeSorter                     162.6 µs           yes       
100         digits    BinaryInsertionSorter           165.6 µs           yes       
100         digits    CocktailSorter                  240.8 µs           yes       
100         digits    InsertionSorter                 199.6 µs           yes       
100         digits    BubbleSorter                    240.5 µs           yes       
100         digits    ShellSorter.Classic             37.4 µs            yes       
100         digits    InsertionShiftSorter            168.5 µs           yes       
100         digits    ShellSorter.Hibbard             32.4 µs            yes       
100         digits    ShellSorter.Sedgewick           45.6 µs            yes       
100         sorted    QuickSorter                     189.9 µs           yes       
100         sorted    MergeSorter                     142.5 µs           yes       
100         sorted    BinaryInsertionSorter           17.0 µs            yes       
100         sorted    CocktailSorter                  3.2 µs             yes       
100         sorted    InsertionSorter                 3.7 µs             yes       
100         sorted    BubbleSorter                    141.9 µs           yes       
100         sorted    InsertionShiftSorter            7.8 µs             yes       
100         sorted    ShellSorter.Classic             23.1 µs            yes       
100         sorted    ShellSorter.Hibbard             17.0 µs            yes       
100         sorted    ShellSorter.Sedgewick           13.2 µs            yes       
100         revers    MergeSorter                     115.0 µs           yes       
100         revers    QuickSorter                     226.9 µs           yes       
100         revers    BinaryInsertionSorter           238.0 µs           yes       
100         revers    CocktailSorter                  395.4 µs           yes       
100         revers    InsertionSorter                 399.7 µs           yes       
100         revers    ShellSorter.Classic             33.6 µs            yes       
100         revers    BubbleSorter                    403.9 µs           yes       
100         revers    InsertionShiftSorter            342.6 µs           yes       
100         revers    ShellSorter.Hibbard             31.9 µs            yes       
100         revers    ShellSorter.Sedgewick           37.8 µs            yes       
1000        random    QuickSorter                     620.6 µs           yes       
1000        random    MergeSorter                     2.4 ms              yes       
1000        random    ShellSorter.Classic             1.4 ms              yes       
1000        random    ShellSorter.Sedgewick           1.0 ms              yes       
1000        random    ShellSorter.Hibbard             1.5 ms              yes       
1000        digits    QuickSorter                     3.0 ms              yes       
1000        digits    MergeSorter                     2.7 ms              yes       
1000        digits    ShellSorter.Classic             831.1 µs           yes       
1000        digits    ShellSorter.Hibbard             732.5 µs           yes       
1000        digits    ShellSorter.Sedgewick           639.9 µs           yes       
1000        sorted    BinaryInsertionSorter           625.2 µs           yes       
1000        sorted    CocktailSorter                  265.1 µs           yes       
1000        sorted    InsertionSorter                 229.5 µs           yes       
1000        sorted    MergeSorter                     2.5 ms              yes       
1000        sorted    InsertionShiftSorter            279.4 µs           yes       
1000        random    BinaryInsertionSorter           11.8 ms             yes       
1000        sorted    ShellSorter.Classic             543.8 µs           yes       
1000        sorted    ShellSorter.Hibbard             586.9 µs           yes       
1000        sorted    ShellSorter.Sedgewick           559.2 µs           yes       
1000        revers    MergeSorter                     2.4 ms              yes       
1000        revers    ShellSorter.Hibbard             912.6 µs           yes       
1000        revers    ShellSorter.Classic             1.1 ms              yes       
1000        digits    InsertionShiftSorter            24.6 ms             yes       
1000        revers    ShellSorter.Sedgewick           813.9 µs           yes       
1000        random    InsertionShiftSorter            30.1 ms             yes       
1000        random    CocktailSorter                  31.2 ms             yes       
1000        random    InsertionSorter                 32.3 ms             yes       
1000        sorted    QuickSorter                     27.4 ms             yes       
1000        digits    InsertionSorter                 33.2 ms             yes       
1000        sorted    BubbleSorter                    29.5 ms             yes       
1000        revers    QuickSorter                     27.7 ms             yes       
1000        digits    CocktailSorter                  38.1 ms             yes       
1000        digits    BinaryInsertionSorter           42.6 ms             yes       
10000       random    QuickSorter                     13.4 ms             yes       
1000        random    BubbleSorter                    46.7 ms             yes       
1000        revers    BinaryInsertionSorter           32.7 ms             yes       
1000        digits    BubbleSorter                    61.3 ms             yes       
10000       random    ShellSorter.Sedgewick           30.1 ms             yes       
10000       random    ShellSorter.Classic             36.0 ms             yes       
10000       sorted    InsertionSorter                 2.1 ms              yes       
1000        revers    CocktailSorter                  72.2 ms             yes       
10000       digits    ShellSorter.Classic             21.6 ms             yes       
10000       sorted    CocktailSorter                  8.6 ms              yes       
10000       sorted    InsertionShiftSorter            3.5 ms              yes       
10000       random    MergeSorter                     62.7 ms             yes       
10000       digits    ShellSorter.Hibbard             28.9 ms             yes       
10000       digits    ShellSorter.Sedgewick           25.6 ms             yes       
10000       sorted    BinaryInsertionSorter           20.9 ms             yes       
1000        revers    BubbleSorter                    81.0 ms             yes       
10000       sorted    ShellSorter.Classic             9.2 ms              yes       
10000       random    ShellSorter.Hibbard             57.4 ms             yes       
10000       sorted    ShellSorter.Sedgewick           14.7 ms             yes       
10000       sorted    ShellSorter.Hibbard             22.0 ms             yes       
1000        revers    InsertionSorter                 115.4 ms            yes       
1000        revers    InsertionShiftSorter            111.8 ms            yes       
10000       sorted    MergeSorter                     67.8 ms             yes       
10000       digits    MergeSorter                     91.1 ms             yes       
10000       revers    ShellSorter.Hibbard             35.2 ms             yes       
10000       revers    ShellSorter.Sedgewick           38.6 ms             yes       
10000       revers    ShellSorter.Classic             45.6 ms             yes       
10000       revers    MergeSorter                     63.7 ms             yes       
100000      digits    ShellSorter.Sedgewick           716.4 ms            yes       
100000      digits    ShellSorter.Hibbard             794.5 ms            yes       
100000      revers    ShellSorter.Sedgewick           776.4 ms            yes       
100000      digits    ShellSorter.Classic             957.0 ms            yes       
100000      revers    ShellSorter.Hibbard             938.2 ms            yes       
100000      random    QuickSorter                     1.2 s               yes       
10000       digits    QuickSorter                     1.5 s               yes       
100000      revers    ShellSorter.Classic             1.3 s               yes       
100000      random    ShellSorter.Sedgewick           1.6 s               yes       
100000      digits    MergeSorter                     2.1 s               yes       
100000      random    MergeSorter                     2.3 s               yes       
100000      revers    MergeSorter                     2.6 s               yes       
100000      random    ShellSorter.Hibbard             3.1 s               yes       
100000      random    ShellSorter.Classic             3.3 s               yes       
1000000     digits    ShellSorter.Sedgewick           9.7 s               yes       
10000       sorted    QuickSorter                     11.0 s              yes       
10000       random    BinaryInsertionSorter           11.7 s              yes       
1000000     digits    ShellSorter.Hibbard             11.3 s              yes       
1000000     revers    ShellSorter.Sedgewick           12.8 s              yes       
1000000     revers    ShellSorter.Hibbard             13.8 s              yes       
1000000     digits    ShellSorter.Classic             14.0 s              yes       
1000000     revers    ShellSorter.Classic             14.1 s              yes       
10000       digits    InsertionShiftSorter            17.8 s              yes       
1000000     random    QuickSorter                     18.3 s              yes       
10000       revers    QuickSorter                     20.1 s              yes       
10000       random    InsertionSorter                 20.6 s              yes       
10000       digits    BinaryInsertionSorter           20.7 s              yes       
10000       sorted    BubbleSorter                    21.1 s              yes       
10000       revers    BinaryInsertionSorter           21.5 s              yes       
10000       random    InsertionShiftSorter            21.9 s              yes       
10000       digits    InsertionSorter                 22.0 s              yes       
1000000     random    ShellSorter.Sedgewick           24.9 s              yes       
10000       digits    CocktailSorter                  26.6 s              yes       
10000       digits    BubbleSorter                    27.2 s              yes       
1000000     revers    MergeSorter                     28.3 s              yes       
10000       random    CocktailSorter                  29.7 s              yes       
10000       random    BubbleSorter                    31.6 s              yes       
10000       revers    InsertionShiftSorter            33.6 s              yes       
1000000     digits    MergeSorter                     34.2 s              yes       
1000000     random    MergeSorter                     36.2 s              yes       
10000       revers    InsertionSorter                 42.6 s              yes       
10000       revers    CocktailSorter                  44.5 s              yes       
10000       revers    BubbleSorter                    46.7 s              yes       
1000000     random    ShellSorter.Hibbard             49.7 s              yes       
1000000     random    ShellSorter.Classic             52.5 s              yes       
100000      digits    QuickSorter                     2 m 28.2 s          yes       
100000      random    InsertionShiftSorter            10 m 36.2 s         yes       
100000      random    BinaryInsertionSorter           12 m 37.8 s         yes       
100000      digits    InsertionShiftSorter            21 m 21.6 s         yes       
100000      revers    BinaryInsertionSorter           21 m 51.4 s         yes       
100000      revers    QuickSorter                     22 m 3.9 s          yes       
100000      digits    BinaryInsertionSorter           22 m 57.1 s         yes       
100000      digits    InsertionSorter                 24 m 26.8 s         yes       
100000      digits    BubbleSorter                    25 m 45.2 s         yes       
100000      random    InsertionSorter                 26 m 14.8 s         yes       
100000      revers    InsertionShiftSorter            26 m 38.8 s         yes       
100000      random    CocktailSorter                  30 m 24.3 s         yes       
100000      digits    CocktailSorter                  30 m 32.5 s         yes       
100000      random    BubbleSorter                    36 m 31.5 s         yes       
100000      revers    InsertionSorter                 42 m 28.5 s         yes       
100000      revers    BubbleSorter                    44 m 50.9 s         yes       
100000      revers    CocktailSorter                  46 m 35.7 s         yes       
100000      sorted    BinaryInsertionSorter           timeout             no        
100000      sorted    BubbleSorter                    timeout             no        
100000      sorted    CocktailSorter                  timeout             no        
100000      sorted    InsertionShiftSorter            timeout             no        
100000      sorted    InsertionSorter                 timeout             no        
100000      sorted    MergeSorter                     timeout             no        
100000      sorted    QuickSorter                     timeout             no        
100000      sorted    ShellSorter.Classic             timeout             no        
100000      sorted    ShellSorter.Hibbard             timeout             no        
100000      sorted    ShellSorter.Sedgewick           timeout             no        
1000000     digits    BinaryInsertionSorter           timeout             no        
1000000     digits    BubbleSorter                    timeout             no        
1000000     digits    CocktailSorter                  timeout             no        
1000000     digits    InsertionShiftSorter            timeout             no        
1000000     digits    InsertionSorter                 timeout             no        
1000000     digits    QuickSorter                     timeout             no        
1000000     random    BinaryInsertionSorter           timeout             no        
1000000     random    BubbleSorter                    timeout             no        
1000000     random    CocktailSorter                  timeout             no        
1000000     random    InsertionShiftSorter            timeout             no        
1000000     random    InsertionSorter                 timeout             no        
1000000     revers    BinaryInsertionSorter           timeout             no        
1000000     revers    BubbleSorter                    timeout             no        
1000000     revers    CocktailSorter                  timeout             no        
1000000     revers    InsertionShiftSorter            timeout             no        
1000000     revers    InsertionSorter                 timeout             no        
1000000     revers    QuickSorter                     timeout             no        
1000000     sorted    BinaryInsertionSorter           timeout             no        
1000000     sorted    BubbleSorter                    timeout             no        
1000000     sorted    CocktailSorter                  timeout             no        
1000000     sorted    InsertionShiftSorter            timeout             no        
1000000     sorted    InsertionSorter                 timeout             no        
1000000     sorted    MergeSorter                     timeout             no        
1000000     sorted    QuickSorter                     timeout             no        
1000000     sorted    ShellSorter.Classic             timeout             no        
1000000     sorted    ShellSorter.Hibbard             timeout             no        
1000000     sorted    ShellSorter.Sedgewick           timeout             no 