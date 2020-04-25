#include <inttypes.h>

uint64_t atomic_exchange(volatile uint64_t *ptr, uint64_t newVal);

#define __ATOMIC_EXC_CASE(w, suffix, name, size, memBarrier, acq, rel, clobber)                                         \
	static inline uint##size##_t atomic_exchange_##name##u##size(volatile uint##size##_t *ptr, uint##size##_t newVal) { \
		uint##size##_t tmp;                                                                                             \
		uint##size##_t oldVal;                                                                                          \
                                                                                                                        \
		asm volatile(                                                                                                   \
		    "    prfm    pstl1strm, %[val]\n"                                                                           \
		    "1:  ld" #acq "xr" #suffix "    %" #w "[old], %[val]\n"                                                     \
		    "    st" #rel "xr" #suffix "    %w[tmp], %" #w "[new], %[val]\n"                                            \
		    "    cbnz    %w[tmp], 1b\n"                                                                                 \
		    "	" #memBarrier "\n"                                                                                      \
		    : [ tmp ] "=&r"(tmp), [ old ] "=&r"(oldVal), [ val ] "+Q"(*(uint##size##_t *)ptr)                           \
		    : [ new ] "r"(newVal)                                                                                       \
		    : clobber);                                                                                                 \
                                                                                                                        \
		return oldVal;                                                                                                  \
	}

// clang-format off
__ATOMIC_EXC_CASE(w, b,     ,  8,        ,  ,  ,         )
__ATOMIC_EXC_CASE(w, h,     , 16,        ,  ,  ,         )
__ATOMIC_EXC_CASE(w,  ,     , 32,        ,  ,  ,         )
__ATOMIC_EXC_CASE( ,  ,     , 64,        ,  ,  ,         )
__ATOMIC_EXC_CASE(w, b, acq_,  8,        , a,  , "memory")
__ATOMIC_EXC_CASE(w, h, acq_, 16,        , a,  , "memory")
__ATOMIC_EXC_CASE(w,  , acq_, 32,        , a,  , "memory")
__ATOMIC_EXC_CASE( ,  , acq_, 64,        , a,  , "memory")
__ATOMIC_EXC_CASE(w, b, rel_,  8,        ,  , l, "memory")
__ATOMIC_EXC_CASE(w, h, rel_, 16,        ,  , l, "memory")
__ATOMIC_EXC_CASE(w,  , rel_, 32,        ,  , l, "memory")
__ATOMIC_EXC_CASE( ,  , rel_, 64,        ,  , l, "memory")
__ATOMIC_EXC_CASE(w, b,  mb_,  8, dmb ish,  , l, "memory")
__ATOMIC_EXC_CASE(w, h,  mb_, 16, dmb ish,  , l, "memory")
__ATOMIC_EXC_CASE(w,  ,  mb_, 32, dmb ish,  , l, "memory")
__ATOMIC_EXC_CASE( ,  ,  mb_, 64, dmb ish,  , l, "memory")
// clang-format on

#define __ATOMIC_CMP_EXC_CASE(w, suffix, name, size, memBarrier, acq, rel, clobber, compareConstraint)                                                     \
	static inline uint##size##_t atomic_compare_exchange_##name##u##size(volatile uint##size##_t *ptr, uint##size##_t compareVal, uint##size##_t newVal) { \
		uint##size##_t tmp;                                                                                                                                \
		uint##size##_t oldVal;                                                                                                                             \
                                                                                                                                                           \
		asm volatile(                                                                                                                                      \
		    "    prfm    pstl1strm, %[val]\n"                                                                                                              \
		    "1:  ld" #acq "xr" #suffix "    %" #w "[old], %[val]\n"                                                                                        \
		    "    eor     %" #w "[tmp], %" #w "[old], %" #w "[compare]\n"                                                                                   \
		    "    cbnz    %" #w "[tmp], 2f\n"                                                                                                               \
		    "    st" #rel "xr" #suffix "    %w[tmp], %" #w "[new], %[val]\n"                                                                               \
		    "    cbnz    %w[tmp], 1b\n"                                                                                                                    \
		    "	" #memBarrier "\n"                                                                                                                         \
		    "2:"                                                                                                                                           \
		    : [ tmp ] "=&r"(tmp), [ old ] "=&r"(oldVal), [ val ] "+Q"(*(uint##size##_t *)ptr)                                                              \
		    : [ compare ] #compareConstraint "r"(compareVal), [ new ] "r"(newVal)                                                                          \
		    : clobber);                                                                                                                                    \
                                                                                                                                                           \
		return oldVal;                                                                                                                                     \
	}

// clang-format off
__ATOMIC_CMP_EXC_CASE(w, b,     ,  8,        ,  ,  ,         , K)
__ATOMIC_CMP_EXC_CASE(w, h,     , 16,        ,  ,  ,         , K)
__ATOMIC_CMP_EXC_CASE(w,  ,     , 32,        ,  ,  ,         , K)
__ATOMIC_CMP_EXC_CASE( ,  ,     , 64,        ,  ,  ,         , L)
__ATOMIC_CMP_EXC_CASE(w, b, acq_,  8,        , a,  , "memory", K)
__ATOMIC_CMP_EXC_CASE(w, h, acq_, 16,        , a,  , "memory", K)
__ATOMIC_CMP_EXC_CASE(w,  , acq_, 32,        , a,  , "memory", K)
__ATOMIC_CMP_EXC_CASE( ,  , acq_, 64,        , a,  , "memory", L)
__ATOMIC_CMP_EXC_CASE(w, b, rel_,  8,        ,  , l, "memory", K)
__ATOMIC_CMP_EXC_CASE(w, h, rel_, 16,        ,  , l, "memory", K)
__ATOMIC_CMP_EXC_CASE(w,  , rel_, 32,        ,  , l, "memory", K)
__ATOMIC_CMP_EXC_CASE( ,  , rel_, 64,        ,  , l, "memory", L)
__ATOMIC_CMP_EXC_CASE(w, b,  mb_,  8, dmb ish,  , l, "memory", K)
__ATOMIC_CMP_EXC_CASE(w, h,  mb_, 16, dmb ish,  , l, "memory", K)
__ATOMIC_CMP_EXC_CASE(w,  ,  mb_, 32, dmb ish,  , l, "memory", K)
__ATOMIC_CMP_EXC_CASE( ,  ,  mb_, 64, dmb ish,  , l, "memory", L)
// clang-format on
