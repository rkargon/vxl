// Instantiation of vpdl_distribution<double,1>
#include <vpdl/vpdl_distribution.hxx>
VPDL_DISTRIBUTION_INSTANTIATE(double,1);
// also instantiate CDF inversion since n==1
VPDL_INVERSE_CDF_INSTANTIATE(double);
