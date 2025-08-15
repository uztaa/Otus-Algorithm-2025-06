#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include "SingleArray.h"
#include "VectorArray.h"
#include "FactorArray.h"
#include "MatrixArray.h"
#include "LinkedList.h"
#include "DoublyLinkedList.h"

using namespace std;
using namespace std::chrono;

// Benchmarking results structure
struct BenchmarkResult {
    std::string name;
    long long add_front, add_middle, add_end;
    long long get_front, get_middle, get_end;
    long long remove_front, remove_middle, remove_end;
};

// Function to benchmark an array-like structure
template<typename ArrayType>
BenchmarkResult benchmark_array(const std::string& name, int N) {
    ArrayType arr;
    BenchmarkResult result;
    result.name = name;

    // add() benchmarks
    auto t1 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) arr.add(i, 0);
    auto t2 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) arr.add(i, arr.size() / 2);
    auto t3 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) arr.add(i, arr.size());
    auto t4 = high_resolution_clock::now();

    result.add_front  = duration_cast<milliseconds>(t2 - t1).count();
    result.add_middle = duration_cast<milliseconds>(t3 - t2).count();
    result.add_end    = duration_cast<milliseconds>(t4 - t3).count();

    // get() benchmarks
    volatile int dummy = 0;
    auto t5 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) dummy += arr.get(0);
    auto t6 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) dummy += arr.get(arr.size() / 2);
    auto t7 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) dummy += arr.get(arr.size() - 1);
    auto t8 = high_resolution_clock::now();

    result.get_front  = duration_cast<milliseconds>(t6 - t5).count();
    result.get_middle = duration_cast<milliseconds>(t7 - t6).count();
    result.get_end    = duration_cast<milliseconds>(t8 - t7).count();

    // remove() benchmarks
    auto t9 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) arr.remove(0);
    auto t10 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) arr.remove(arr.size() / 2);
    auto t11 = high_resolution_clock::now();
    for (int i = 0; i < N && arr.size() > 0; ++i) arr.remove(arr.size() - 1);
    auto t12 = high_resolution_clock::now();

    result.remove_front  = duration_cast<milliseconds>(t10 - t9).count();
    result.remove_middle = duration_cast<milliseconds>(t11 - t10).count();
    result.remove_end    = duration_cast<milliseconds>(t12 - t11).count();

    return result;
}

void write_html_report(const std::vector<BenchmarkResult>& results, const std::string& filename, int N) {
    std::ofstream out(filename);
    out << "<!DOCTYPE html>\n<html><head><meta charset='utf-8'><title>Benchmark Report</title>\n";
    out << R"(<style>
body { font-family: 'Segoe UI', Arial, sans-serif; background: #f6f8fa; }
table {
  border-collapse: separate;
  border-spacing: 0;
  width: 90%;
  margin: 32px auto;
  background: #fff;
  box-shadow: 0 2px 8px rgba(0,0,0,0.08);
  border-radius: 12px;
  overflow: hidden;
}
th, td {
  padding: 10px 16px;
  text-align: center;
}
th {
  background: #2d6cdf;
  color: #fff;
  font-weight: 600;
  font-size: 1.05em;
  border-bottom: 2px solid #1a4ca0;
}
tr:nth-child(even) td {
  background: #f0f4fa;
}
tr:hover td {
  background: #e2eaff;
  transition: background 0.2s;
}
td {
  border-bottom: 1px solid #e5e5e5;
  font-size: 1em;
}
tr:last-child td {
  border-bottom: none;
}
</style>
)";
    out << "</head><body>\n";
    out << "<h2>Data Structures Comparison Report (N = " << N << ")</h2>\n";
    out << "<table>\n";
    out << "<tr><th>Type</th>"
        << "<th>add(0)</th><th>add(mid)</th><th>add(end)</th>"
        << "<th>get(0)</th><th>get(mid)</th><th>get(end)</th>"
        << "<th>remove(0)</th><th>remove(mid)</th><th>remove(end)</th></tr>\n";
    for (const auto& r : results) {
        out << "<tr><td>" << r.name << "</td>"
            << "<td>" << r.add_front << "</td>"
            << "<td>" << r.add_middle << "</td>"
            << "<td>" << r.add_end << "</td>"
            << "<td>" << r.get_front << "</td>"
            << "<td>" << r.get_middle << "</td>"
            << "<td>" << r.get_end << "</td>"
            << "<td>" << r.remove_front << "</td>"
            << "<td>" << r.remove_middle << "</td>"
            << "<td>" << r.remove_end << "</td>"
            << "</tr>\n";
    }
    out << "</table>\n</body></html>\n";
    out.close();
}

// Main function to run benchmarks and generate report
int main() {
    const int N = 10000;

    std::cout << "benchmark running..." << std::endl;

    std::vector<BenchmarkResult> results;

    results.push_back(benchmark_array<SingleArray<int>>("SingleArray", N));
    results.push_back(benchmark_array<VectorArray<int>>("VectorArray", N));
    results.push_back(benchmark_array<FactorArray<int>>("FactorArray", N));
    results.push_back(benchmark_array<MatrixArray<int>>("MatrixArray", N));
    results.push_back(benchmark_array<LinkedList<int>>("LinkedList", N));
    results.push_back(benchmark_array<DoublyLinkedList<int>>("DoublyLinkedList", N));

    write_html_report(results, "benchmark_report.html", N);

    std::cout << "Results save into benchmark_report.html\n";
    return 0;
}