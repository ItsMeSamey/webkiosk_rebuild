#ifdef __cplusplus // if c++
extern "C" {
#endif // end if c++

#if !defined(UNDEFINE_DYNAMIC_ARRAY)
#include <stdlib.h>
#include <stddef.h>

#define __DARRAY(array) array // does nothing; just for signify return type

#define DARRAY_MAKE(type, array) {\
  uint32_t*__inerenal_variable_1 = (uint32_t *)malloc(2*sizeof(uint32_t) + 3*sizeof(type));\
  __inerenal_variable_1[0] = 0;\
  __inerenal_variable_1[1] = 3;\
  (array) = (type *)(__inerenal_variable_1+2);\
}

#define DARRAY_SIZE(array) ((uint32_t *)(array)-2)

#define DARRAY_ADD(type, array, value) {\
  uint32_t* __inerenal_variable_1 = DARRAY_SIZE(array);\
  if (__inerenal_variable_1[0] == __inerenal_variable_1[1]) {\
    __inerenal_variable_1[1] = (__inerenal_variable_1[1]<<1) | 1;\
    (array) = (type *)((uint32_t *)realloc((void *)__inerenal_variable_1, __inerenal_variable_1[1]*sizeof(type) + 2*sizeof(uint32_t)) + 2);\
  }\
  (array)[__inerenal_variable_1[0]++] = (value);\
}

#define DARRAY_RESIZE(type, array, capacity) {\
  DARRAY_SIZE(array)[1] = capacity;\
  (array) = (type *)((uint32_t *)realloc((void *)DARRAY_SIZE(array), (capacity)*sizeof(type) + 2*sizeof(uint32_t)) + 2);\
}

#define DARRAY_FREE(array) free((void *)DARRAY_SIZE(array))

#else
#undef UNDEFINE_DYNAMIC_ARRAY
#undef DYNAMIC_ARRAY_HEADER
#undef __DARRAY
#undef DARRAY_MAKE
#undef DARRAY_SIZE
#undef DARRAY_ADD
#undef DARRAY_RESIZE
#undef DARRAY_FREE
#endif // end if UNDEFINE_DYNAMIC_ARRAY

#ifdef __cplusplus // if c++
}; // namespace arg_parser
#endif // end if c++

