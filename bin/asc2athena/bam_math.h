#ifndef BAM_MATH_H
#define BAM_MATH_H

#include <valarray>
#include <vector>

namespace BAM {

  std::valarray<double> deriv(const std::valarray<double> &x, const std::valarray<double> &y);
  std::valarray<double> deriv(const std::valarray<double> &y);

  std::vector<double> deriv(const std::vector<double> &x, const std::vector<double> &y);
  std::vector<double> deriv(const std::vector<double> &y);

}
#endif
