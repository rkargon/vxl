#ifndef vipl_accessors_vbl_array_2d_txx_
#define vipl_accessors_vbl_array_2d_txx_

#include "vipl_accessors_vbl_array_2d.h"

template <class DataType>
DataType fgetpixel(vbl_array_2d<DataType> const& i, int x, int y, DataType /* dummy */) {
  return i(x,y);
}
template <class DataType>
void fsetpixel(vbl_array_2d<DataType>& i, int x, int y, DataType e) {
  i(x,y) = e;
}
template <class DataType>
DataType getpixel(vbl_array_2d<DataType> const& i, int x, int y, DataType /* dummy */) {
  if (x<0 || y<0 || x>=i.columns() || y>=i.rows()) return DataType();
  return i(x,y);
}
template <class DataType>
void setpixel(vbl_array_2d<DataType>& i, int x, int y, DataType e) {
  if (x<0 || y<0 || x>=i.columns() || y>=i.rows()) return;
  i(x,y) = e;
}

#define VIPL_INSTANTIATE_ACCESSORS(T) \
template T fgetpixel(vbl_array_2d<T > const&, int, int, T);\
template void fsetpixel(vbl_array_2d<T >&, int, int, T);\
template T getpixel(vbl_array_2d<T > const&, int, int, T);\
template void setpixel(vbl_array_2d<T >&, int, int, T)

#endif // vipl_accessors_vbl_array_2d_txx_
