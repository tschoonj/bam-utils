#include "bam_math.h"
#include <iostream>
#include <cmath>

int main(int argc, char *argv[]) {

  std::vector<double> x(100), y(100);

  for (unsigned int i = 0 ; i < 100 ; i++) {
    x[i] = double(i) * M_PI/99.0;
    y[i] = sin(x[i]);
  }

  std::vector<double> derived = BAM::deriv(x, y);

  for (unsigned int i = 0 ; i < 100 ; i++) {
    std::cout << x[i] << " " << y[i] << " " << derived[i] << std::endl;
  }
  return 0;
}
