#include "bam_math.h"
#include "bam_exception.h"

std::valarray<double> BAM::deriv(const std::valarray<double> &x,
                            const std::valarray<double> &y) {
  if (x.size() != y.size()) {
    throw BAM::Exception("BAM::deriv -> x and y must have the same number of elements!");
  }

  std::valarray<double> x0 = x.cshift(1);
  std::valarray<double> x2 = x.cshift(-1);
  std::valarray<double> x01 = x0 - x;
  std::valarray<double> x02 = x0 - x2;
  std::valarray<double> x12 = x - x2;

  std::valarray<double> d = y.cshift(1) * (x12 / (x01*x02)) +
    y * (1./x12 - 1./x01) -
    y.cshift(-1) * (x01 / (x02*x12));

  unsigned int n = d.size();

  d[0] = y[0] * (x01[1]+x02[1])/(x01[1]*x02[1]) -
    y[1] * x02[1]/(x01[1]*x12[1]) +
    y[2] * x01[1]/(x02[1]*x12[1]);

  d[n-1] = -y[n-3] * x12[n-2]/(x01[n-2]*x02[n-2]) +
    y[n-2] * x02[n-2]/(x01[n-2]*x12[n-2]) -
    y[n-1] * (x02[n-2]+x12[n-2]) / (x02[n-2]*x12[n-2]);
  return d;
}

std::vector<double> BAM::deriv(const std::vector<double> &x,
                            const std::vector<double> &y) {
  std::valarray<double> res = BAM::deriv(std::valarray<double>(x.data(), x.size()),
                              std::valarray<double>(y.data(), y.size()));
  return std::vector<double>(std::begin(res), std::end(res));
}

std::valarray<double> BAM::deriv(const std::valarray<double> &y) {
  std::valarray<double> d = (y.cshift(-1) - y.cshift(1))/2.;
  unsigned int n = d.size();
  d[0] = (-3.0*y[0] + 4.0*y[1] - y[2])/2.;
  d[n-1] = (3.*y[n-1] - 4.*y[n-2] + y[n-3])/2.;
  return d;
}

std::vector<double> BAM::deriv(const std::vector<double> &y) {
  std::valarray<double> res = BAM::deriv(std::valarray<double>(y.data(), y.size()));
  return std::vector<double>(std::begin(res), std::end(res));
}
