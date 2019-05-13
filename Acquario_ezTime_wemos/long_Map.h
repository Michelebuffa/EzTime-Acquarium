#ifndef longMap_h
#define longMap_h

static inline __attribute__((always_inline)) unsigned long long_Map(const unsigned long x, const unsigned long in_min, const unsigned long in_max,const unsigned long out_min,const unsigned long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

#endif
