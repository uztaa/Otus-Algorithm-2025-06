#pragma once

#include "Record.h"
#include <vector>
#include <random>

/** 
 * @brief generator ramdom values for test
 * 
 * @return std::vector<Record> 
 */
class RandomArrayGenerator {
private:
    std::mt19937 rng_;
public:
    RandomArrayGenerator();

   /**
   * @brief generate random vector
   * @param size size of vector.
   * @return std::vector<Record>
   */
    std::vector<Record> generate(std::size_t size);
};
