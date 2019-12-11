# 1 "test_dummy_functions_tests.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "test_dummy_functions_tests.c"
# 1 "/usr/include/err.h" 1 3 4
# 22 "/usr/include/err.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 446 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 452 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 453 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 454 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 447 "/usr/include/features.h" 2 3 4
# 470 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 471 "/usr/include/features.h" 2 3 4
# 23 "/usr/include/err.h" 2 3 4


# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdarg.h" 3 4

# 40 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 26 "/usr/include/err.h" 2 3 4








extern void warn (const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));
extern void vwarn (const char *__format, __gnuc_va_list)
     __attribute__ ((__format__ (__printf__, 1, 0)));


extern void warnx (const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));
extern void vwarnx (const char *__format, __gnuc_va_list)
     __attribute__ ((__format__ (__printf__, 1, 0)));


extern void err (int __status, const char *__format, ...)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 3)));
extern void verr (int __status, const char *__format, __gnuc_va_list)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 0)));
extern void errx (int __status, const char *__format, ...)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 3)));
extern void verrx (int __status, const char *, __gnuc_va_list)
     __attribute__ ((__noreturn__, __format__ (__printf__, 2, 0)));


# 2 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/stdlib.h" 1 3 4
# 25 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 26 "/usr/include/stdlib.h" 2 3 4





# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 216 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 328 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 3 4
typedef int wchar_t;
# 32 "/usr/include/stdlib.h" 2 3 4







# 1 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 1 3 4
# 52 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 3 4
typedef enum
{
  P_ALL,
  P_PID,
  P_PGID
} idtype_t;
# 40 "/usr/include/stdlib.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 1 3 4
# 41 "/usr/include/stdlib.h" 2 3 4
# 55 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 1 3 4
# 120 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 2 3 4
# 121 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 2 3 4
# 56 "/usr/include/stdlib.h" 2 3 4


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;





__extension__ typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;
# 97 "/usr/include/stdlib.h" 3 4
extern size_t __ctype_get_mb_cur_max (void) __attribute__ ((__nothrow__ , __leaf__)) ;



extern double atof (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern int atoi (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern long int atol (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;



__extension__ extern long long int atoll (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;



extern double strtod (const char *__restrict __nptr,
        char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern float strtof (const char *__restrict __nptr,
       char **__restrict __endptr) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern long double strtold (const char *__restrict __nptr,
       char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 176 "/usr/include/stdlib.h" 3 4
extern long int strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern unsigned long int strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



__extension__
extern long long int strtoq (const char *__restrict __nptr,
        char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtouq (const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




__extension__
extern long long int strtoll (const char *__restrict __nptr,
         char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtoull (const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 385 "/usr/include/stdlib.h" 3 4
extern char *l64a (long int __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern long int a64l (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;




# 1 "/usr/include/x86_64-linux-gnu/sys/types.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/types.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
# 29 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;






typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;



typedef long int __quad_t;
typedef unsigned long int __u_quad_t;







typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
# 141 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 1 3 4
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/time64.h" 1 3 4
# 143 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;




typedef int __sig_atomic_t;
# 30 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4



typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;


typedef __loff_t loff_t;




typedef __ino_t ino_t;
# 59 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
# 97 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __pid_t pid_t;





typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;




# 1 "/usr/include/x86_64-linux-gnu/bits/types/clock_t.h" 1 3 4






typedef __clock_t clock_t;
# 127 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4

# 1 "/usr/include/x86_64-linux-gnu/bits/types/clockid_t.h" 1 3 4






typedef __clockid_t clockid_t;
# 129 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 1 3 4






typedef __time_t time_t;
# 130 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/timer_t.h" 1 3 4






typedef __timer_t timer_t;
# 131 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
# 144 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 145 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;




# 1 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 3 4
typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;
# 156 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
# 177 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 193 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 36 "/usr/include/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/endian.h" 1 3 4
# 37 "/usr/include/endian.h" 2 3 4
# 60 "/usr/include/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 1 3 4
# 33 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
static __inline __uint16_t
__bswap_16 (__uint16_t __bsx)
{

  return __builtin_bswap16 (__bsx);



}






static __inline __uint32_t
__bswap_32 (__uint32_t __bsx)
{

  return __builtin_bswap32 (__bsx);



}
# 69 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
__extension__ static __inline __uint64_t
__bswap_64 (__uint64_t __bsx)
{

  return __builtin_bswap64 (__bsx);



}
# 61 "/usr/include/endian.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/uintn-identity.h" 1 3 4
# 32 "/usr/include/x86_64-linux-gnu/bits/uintn-identity.h" 3 4
static __inline __uint16_t
__uint16_identity (__uint16_t __x)
{
  return __x;
}

static __inline __uint32_t
__uint32_identity (__uint32_t __x)
{
  return __x;
}

static __inline __uint64_t
__uint64_identity (__uint64_t __x)
{
  return __x;
}
# 62 "/usr/include/endian.h" 2 3 4
# 194 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/sys/select.h" 1 3 4
# 30 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/select.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/select.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/select.h" 2 3 4
# 31 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/types/sigset_t.h" 1 3 4



# 1 "/usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h" 1 3 4




typedef struct
{
  unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
} __sigset_t;
# 5 "/usr/include/x86_64-linux-gnu/bits/types/sigset_t.h" 2 3 4


typedef __sigset_t sigset_t;
# 34 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4



# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h" 1 3 4







struct timeval
{
  __time_t tv_sec;
  __suseconds_t tv_usec;
};
# 38 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4

# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h" 1 3 4
# 9 "/usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h" 3 4
struct timespec
{
  __time_t tv_sec;
  __syscall_slong_t tv_nsec;
};
# 40 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4



typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
# 59 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
typedef struct
  {






    __fd_mask __fds_bits[1024 / (8 * (int) sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 91 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4

# 101 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 113 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);
# 126 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4

# 197 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4





typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 244 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/thread-shared-types.h" 1 3 4
# 77 "/usr/include/x86_64-linux-gnu/bits/thread-shared-types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h" 1 3 4
# 21 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h" 2 3 4
# 65 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h" 3 4
struct __pthread_rwlock_arch_t
{
  unsigned int __readers;
  unsigned int __writers;
  unsigned int __wrphase_futex;
  unsigned int __writers_futex;
  unsigned int __pad3;
  unsigned int __pad4;

  int __cur_writer;
  int __shared;
  signed char __rwelision;




  unsigned char __pad1[7];


  unsigned long int __pad2;


  unsigned int __flags;
# 99 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h" 3 4
};
# 78 "/usr/include/x86_64-linux-gnu/bits/thread-shared-types.h" 2 3 4




typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;
# 118 "/usr/include/x86_64-linux-gnu/bits/thread-shared-types.h" 3 4
struct __pthread_mutex_s
{
  int __lock ;
  unsigned int __count;
  int __owner;

  unsigned int __nusers;
# 148 "/usr/include/x86_64-linux-gnu/bits/thread-shared-types.h" 3 4
  int __kind;
 




  short __spins; short __elision;
  __pthread_list_t __list;
# 165 "/usr/include/x86_64-linux-gnu/bits/thread-shared-types.h" 3 4
 
};




struct __pthread_cond_s
{
  __extension__ union
  {
    __extension__ unsigned long long int __wseq;
    struct
    {
      unsigned int __low;
      unsigned int __high;
    } __wseq32;
  };
  __extension__ union
  {
    __extension__ unsigned long long int __g1_start;
    struct
    {
      unsigned int __low;
      unsigned int __high;
    } __g1_start32;
  };
  unsigned int __g_refs[2] ;
  unsigned int __g_size[2];
  unsigned int __g1_orig_size;
  unsigned int __wrefs;
  unsigned int __g_signals[2];
};
# 24 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 2 3 4



typedef unsigned long int pthread_t;




typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;


union pthread_attr_t
{
  char __size[56];
  long int __align;
};

typedef union pthread_attr_t pthread_attr_t;




typedef union
{
  struct __pthread_mutex_s __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;


typedef union
{
  struct __pthread_cond_s __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;





typedef union
{
  struct __pthread_rwlock_arch_t __data;
  char __size[56];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[32];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 245 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4



# 395 "/usr/include/stdlib.h" 2 3 4






extern long int random (void) __attribute__ ((__nothrow__ , __leaf__));


extern void srandom (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));





extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));



extern char *setstate (char *__statebuf) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int srandom_r (unsigned int __seed, struct random_data *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));

extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));





extern int rand (void) __attribute__ ((__nothrow__ , __leaf__));

extern void srand (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));



extern int rand_r (unsigned int *__seed) __attribute__ ((__nothrow__ , __leaf__));







extern double drand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern double erand48 (unsigned short int __xsubi[3]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int lrand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern long int nrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int mrand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern long int jrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void srand48 (long int __seedval) __attribute__ ((__nothrow__ , __leaf__));
extern unsigned short int *seed48 (unsigned short int __seed16v[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern void lcong48 (unsigned short int __param[7]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    __extension__ unsigned long long int __a;

  };


extern int drand48_r (struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int erand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int lrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int nrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int mrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int jrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int seed48_r (unsigned short int __seed16v[3],
       struct drand48_data *__buffer) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int lcong48_r (unsigned short int __param[7],
        struct drand48_data *__buffer)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));




extern void *malloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;

extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;






extern void *realloc (void *__ptr, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__warn_unused_result__));







extern void *reallocarray (void *__ptr, size_t __nmemb, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__warn_unused_result__));



extern void free (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));


# 1 "/usr/include/alloca.h" 1 3 4
# 24 "/usr/include/alloca.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 25 "/usr/include/alloca.h" 2 3 4







extern void *alloca (size_t __size) __attribute__ ((__nothrow__ , __leaf__));






# 567 "/usr/include/stdlib.h" 2 3 4





extern void *valloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;




extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;




extern void *aligned_alloc (size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__alloc_size__ (2))) ;



extern void abort (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern int at_quick_exit (void (*__func) (void)) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern void exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));





extern void quick_exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));





extern void _Exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));




extern char *getenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 644 "/usr/include/stdlib.h" 3 4
extern int putenv (char *__string) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern int setenv (const char *__name, const char *__value, int __replace)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));


extern int unsetenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern int clearenv (void) __attribute__ ((__nothrow__ , __leaf__));
# 672 "/usr/include/stdlib.h" 3 4
extern char *mktemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 685 "/usr/include/stdlib.h" 3 4
extern int mkstemp (char *__template) __attribute__ ((__nonnull__ (1))) ;
# 707 "/usr/include/stdlib.h" 3 4
extern int mkstemps (char *__template, int __suffixlen) __attribute__ ((__nonnull__ (1))) ;
# 728 "/usr/include/stdlib.h" 3 4
extern char *mkdtemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 781 "/usr/include/stdlib.h" 3 4
extern int system (const char *__command) ;
# 797 "/usr/include/stdlib.h" 3 4
extern char *realpath (const char *__restrict __name,
         char *__restrict __resolved) __attribute__ ((__nothrow__ , __leaf__)) ;






typedef int (*__compar_fn_t) (const void *, const void *);
# 817 "/usr/include/stdlib.h" 3 4
extern void *bsearch (const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar)
     __attribute__ ((__nonnull__ (1, 2, 5))) ;







extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar) __attribute__ ((__nonnull__ (1, 4)));
# 837 "/usr/include/stdlib.h" 3 4
extern int abs (int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern long int labs (long int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;


__extension__ extern long long int llabs (long long int __x)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;






extern div_t div (int __numer, int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;


__extension__ extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
# 869 "/usr/include/stdlib.h" 3 4
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;




extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;




extern char *gcvt (double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3))) ;




extern char *qecvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;
extern char *qfcvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3))) ;




extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));

extern int qecvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int qfcvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));





extern int mblen (const char *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));


extern int mbtowc (wchar_t *__restrict __pwc,
     const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));


extern int wctomb (char *__s, wchar_t __wchar) __attribute__ ((__nothrow__ , __leaf__));



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));

extern size_t wcstombs (char *__restrict __s,
   const wchar_t *__restrict __pwcs, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__));







extern int rpmatch (const char *__response) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 954 "/usr/include/stdlib.h" 3 4
extern int getsubopt (char **__restrict __optionp,
        char *const *__restrict __tokens,
        char **__restrict __valuep)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2, 3))) ;
# 1000 "/usr/include/stdlib.h" 3 4
extern int getloadavg (double __loadavg[], int __nelem)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 1010 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/stdlib-float.h" 1 3 4
# 1011 "/usr/include/stdlib.h" 2 3 4
# 1020 "/usr/include/stdlib.h" 3 4

# 3 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/errno.h" 1 3 4
# 28 "/usr/include/errno.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/errno.h" 1 3 4
# 26 "/usr/include/x86_64-linux-gnu/bits/errno.h" 3 4
# 1 "/usr/include/linux/errno.h" 1 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/errno.h" 1 3 4
# 1 "/usr/include/asm-generic/errno.h" 1 3 4




# 1 "/usr/include/asm-generic/errno-base.h" 1 3 4
# 6 "/usr/include/asm-generic/errno.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/errno.h" 2 3 4
# 1 "/usr/include/linux/errno.h" 2 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/errno.h" 2 3 4
# 29 "/usr/include/errno.h" 2 3 4








extern int *__errno_location (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 52 "/usr/include/errno.h" 3 4

# 4 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/x86_64-linux-gnu/sys/mount.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/sys/mount.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/ioctl.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/sys/ioctl.h" 3 4



# 1 "/usr/include/x86_64-linux-gnu/bits/ioctls.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/ioctls.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/ioctls.h" 1 3 4
# 1 "/usr/include/asm-generic/ioctls.h" 1 3 4




# 1 "/usr/include/linux/ioctl.h" 1 3 4




# 1 "/usr/include/x86_64-linux-gnu/asm/ioctl.h" 1 3 4
# 1 "/usr/include/asm-generic/ioctl.h" 1 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/ioctl.h" 2 3 4
# 6 "/usr/include/linux/ioctl.h" 2 3 4
# 6 "/usr/include/asm-generic/ioctls.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/ioctls.h" 2 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/ioctls.h" 2 3 4
# 27 "/usr/include/x86_64-linux-gnu/sys/ioctl.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/ioctl-types.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/ioctl-types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/ioctls.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/bits/ioctl-types.h" 2 3 4


struct winsize
  {
    unsigned short int ws_row;
    unsigned short int ws_col;
    unsigned short int ws_xpixel;
    unsigned short int ws_ypixel;
  };


struct termio
  {
    unsigned short int c_iflag;
    unsigned short int c_oflag;
    unsigned short int c_cflag;
    unsigned short int c_lflag;
    unsigned char c_line;
    unsigned char c_cc[8];
};
# 30 "/usr/include/x86_64-linux-gnu/sys/ioctl.h" 2 3 4






# 1 "/usr/include/x86_64-linux-gnu/sys/ttydefaults.h" 1 3 4
# 37 "/usr/include/x86_64-linux-gnu/sys/ioctl.h" 2 3 4




extern int ioctl (int __fd, unsigned long int __request, ...) __attribute__ ((__nothrow__ , __leaf__));


# 26 "/usr/include/x86_64-linux-gnu/sys/mount.h" 2 3 4







enum
{
  MS_RDONLY = 1,

  MS_NOSUID = 2,

  MS_NODEV = 4,

  MS_NOEXEC = 8,

  MS_SYNCHRONOUS = 16,

  MS_REMOUNT = 32,

  MS_MANDLOCK = 64,

  MS_DIRSYNC = 128,

  MS_NOATIME = 1024,

  MS_NODIRATIME = 2048,

  MS_BIND = 4096,

  MS_MOVE = 8192,

  MS_REC = 16384,

  MS_SILENT = 32768,

  MS_POSIXACL = 1 << 16,

  MS_UNBINDABLE = 1 << 17,

  MS_PRIVATE = 1 << 18,

  MS_SLAVE = 1 << 19,

  MS_SHARED = 1 << 20,

  MS_RELATIME = 1 << 21,

  MS_KERNMOUNT = 1 << 22,

  MS_I_VERSION = 1 << 23,

  MS_STRICTATIME = 1 << 24,

  MS_LAZYTIME = 1 << 25,

  MS_ACTIVE = 1 << 30,

  MS_NOUSER = 1 << 31

};
# 122 "/usr/include/x86_64-linux-gnu/sys/mount.h" 3 4
enum
{
  MNT_FORCE = 1,

  MNT_DETACH = 2,

  MNT_EXPIRE = 4,

  UMOUNT_NOFOLLOW = 8

};





extern int mount (const char *__special_file, const char *__dir,
    const char *__fstype, unsigned long int __rwflag,
    const void *__data) __attribute__ ((__nothrow__ , __leaf__));


extern int umount (const char *__special_file) __attribute__ ((__nothrow__ , __leaf__));


extern int umount2 (const char *__special_file, int __flags) __attribute__ ((__nothrow__ , __leaf__));


# 5 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/x86_64-linux-gnu/sys/timerfd.h" 1 3 4
# 21 "/usr/include/x86_64-linux-gnu/sys/timerfd.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 29 "/usr/include/time.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 30 "/usr/include/time.h" 2 3 4



# 1 "/usr/include/x86_64-linux-gnu/bits/time.h" 1 3 4
# 34 "/usr/include/time.h" 2 3 4





# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_tm.h" 1 3 4






struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;


  long int tm_gmtoff;
  const char *tm_zone;




};
# 40 "/usr/include/time.h" 2 3 4
# 48 "/usr/include/time.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h" 1 3 4







struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };
# 49 "/usr/include/time.h" 2 3 4
struct sigevent;
# 60 "/usr/include/time.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__locale_t.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/types/__locale_t.h" 3 4
struct __locale_struct
{

  struct __locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
};

typedef struct __locale_struct *__locale_t;
# 23 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 2 3 4

typedef __locale_t locale_t;
# 61 "/usr/include/time.h" 2 3 4











extern clock_t clock (void) __attribute__ ((__nothrow__ , __leaf__));


extern time_t time (time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));


extern double difftime (time_t __time1, time_t __time0)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));


extern time_t mktime (struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));





extern size_t strftime (char *__restrict __s, size_t __maxsize,
   const char *__restrict __format,
   const struct tm *__restrict __tp) __attribute__ ((__nothrow__ , __leaf__));
# 104 "/usr/include/time.h" 3 4
extern size_t strftime_l (char *__restrict __s, size_t __maxsize,
     const char *__restrict __format,
     const struct tm *__restrict __tp,
     locale_t __loc) __attribute__ ((__nothrow__ , __leaf__));
# 119 "/usr/include/time.h" 3 4
extern struct tm *gmtime (const time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));



extern struct tm *localtime (const time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));




extern struct tm *gmtime_r (const time_t *__restrict __timer,
       struct tm *__restrict __tp) __attribute__ ((__nothrow__ , __leaf__));



extern struct tm *localtime_r (const time_t *__restrict __timer,
          struct tm *__restrict __tp) __attribute__ ((__nothrow__ , __leaf__));




extern char *asctime (const struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern char *ctime (const time_t *__timer) __attribute__ ((__nothrow__ , __leaf__));






extern char *asctime_r (const struct tm *__restrict __tp,
   char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));


extern char *ctime_r (const time_t *__restrict __timer,
        char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));




extern char *__tzname[2];
extern int __daylight;
extern long int __timezone;




extern char *tzname[2];



extern void tzset (void) __attribute__ ((__nothrow__ , __leaf__));



extern int daylight;
extern long int timezone;





extern int stime (const time_t *__when) __attribute__ ((__nothrow__ , __leaf__));
# 196 "/usr/include/time.h" 3 4
extern time_t timegm (struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern time_t timelocal (struct tm *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern int dysize (int __year) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 211 "/usr/include/time.h" 3 4
extern int nanosleep (const struct timespec *__requested_time,
        struct timespec *__remaining);



extern int clock_getres (clockid_t __clock_id, struct timespec *__res) __attribute__ ((__nothrow__ , __leaf__));


extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) __attribute__ ((__nothrow__ , __leaf__));


extern int clock_settime (clockid_t __clock_id, const struct timespec *__tp)
     __attribute__ ((__nothrow__ , __leaf__));






extern int clock_nanosleep (clockid_t __clock_id, int __flags,
       const struct timespec *__req,
       struct timespec *__rem);


extern int clock_getcpuclockid (pid_t __pid, clockid_t *__clock_id) __attribute__ ((__nothrow__ , __leaf__));




extern int timer_create (clockid_t __clock_id,
    struct sigevent *__restrict __evp,
    timer_t *__restrict __timerid) __attribute__ ((__nothrow__ , __leaf__));


extern int timer_delete (timer_t __timerid) __attribute__ ((__nothrow__ , __leaf__));


extern int timer_settime (timer_t __timerid, int __flags,
     const struct itimerspec *__restrict __value,
     struct itimerspec *__restrict __ovalue) __attribute__ ((__nothrow__ , __leaf__));


extern int timer_gettime (timer_t __timerid, struct itimerspec *__value)
     __attribute__ ((__nothrow__ , __leaf__));


extern int timer_getoverrun (timer_t __timerid) __attribute__ ((__nothrow__ , __leaf__));





extern int timespec_get (struct timespec *__ts, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 307 "/usr/include/time.h" 3 4

# 22 "/usr/include/x86_64-linux-gnu/sys/timerfd.h" 2 3 4



# 1 "/usr/include/x86_64-linux-gnu/bits/timerfd.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/timerfd.h" 3 4
enum
  {
    TFD_CLOEXEC = 02000000,

    TFD_NONBLOCK = 00004000

  };
# 26 "/usr/include/x86_64-linux-gnu/sys/timerfd.h" 2 3 4



enum
  {
    TFD_TIMER_ABSTIME = 1 << 0,

    TFD_TIMER_CANCEL_ON_SET = 1 << 1

  };





extern int timerfd_create (__clockid_t __clock_id, int __flags) __attribute__ ((__nothrow__ , __leaf__));




extern int timerfd_settime (int __ufd, int __flags,
       const struct itimerspec *__utmr,
       struct itimerspec *__otmr) __attribute__ ((__nothrow__ , __leaf__));


extern int timerfd_gettime (int __ufd, struct itimerspec *__otmr) __attribute__ ((__nothrow__ , __leaf__));


# 6 "test_dummy_functions_tests.c" 2

# 1 "/usr/include/x86_64-linux-gnu/sys/socket.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h" 2 3 4


struct iovec
  {
    void *iov_base;
    size_t iov_len;
  };
# 27 "/usr/include/x86_64-linux-gnu/sys/socket.h" 2 3 4

# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 29 "/usr/include/x86_64-linux-gnu/sys/socket.h" 2 3 4




# 1 "/usr/include/x86_64-linux-gnu/bits/socket.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/socket.h" 2 3 4





typedef __socklen_t socklen_t;




# 1 "/usr/include/x86_64-linux-gnu/bits/socket_type.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/socket_type.h" 3 4
enum __socket_type
{
  SOCK_STREAM = 1,


  SOCK_DGRAM = 2,


  SOCK_RAW = 3,

  SOCK_RDM = 4,

  SOCK_SEQPACKET = 5,


  SOCK_DCCP = 6,

  SOCK_PACKET = 10,







  SOCK_CLOEXEC = 02000000,


  SOCK_NONBLOCK = 00004000


};
# 39 "/usr/include/x86_64-linux-gnu/bits/socket.h" 2 3 4
# 175 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/sockaddr.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/sockaddr.h" 3 4
typedef unsigned short int sa_family_t;
# 176 "/usr/include/x86_64-linux-gnu/bits/socket.h" 2 3 4


struct sockaddr
  {
    sa_family_t sa_family;
    char sa_data[14];
  };
# 191 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
struct sockaddr_storage
  {
    sa_family_t ss_family;
    char __ss_padding[(128 - (sizeof (unsigned short int)) - sizeof (unsigned long int))];
    unsigned long int __ss_align;
  };



enum
  {
    MSG_OOB = 0x01,

    MSG_PEEK = 0x02,

    MSG_DONTROUTE = 0x04,






    MSG_CTRUNC = 0x08,

    MSG_PROXY = 0x10,

    MSG_TRUNC = 0x20,

    MSG_DONTWAIT = 0x40,

    MSG_EOR = 0x80,

    MSG_WAITALL = 0x100,

    MSG_FIN = 0x200,

    MSG_SYN = 0x400,

    MSG_CONFIRM = 0x800,

    MSG_RST = 0x1000,

    MSG_ERRQUEUE = 0x2000,

    MSG_NOSIGNAL = 0x4000,

    MSG_MORE = 0x8000,

    MSG_WAITFORONE = 0x10000,

    MSG_BATCH = 0x40000,

    MSG_ZEROCOPY = 0x4000000,

    MSG_FASTOPEN = 0x20000000,


    MSG_CMSG_CLOEXEC = 0x40000000



  };




struct msghdr
  {
    void *msg_name;
    socklen_t msg_namelen;

    struct iovec *msg_iov;
    size_t msg_iovlen;

    void *msg_control;
    size_t msg_controllen;




    int msg_flags;
  };


struct cmsghdr
  {
    size_t cmsg_len;




    int cmsg_level;
    int cmsg_type;

    __extension__ unsigned char __cmsg_data [];

  };
# 305 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
extern struct cmsghdr *__cmsg_nxthdr (struct msghdr *__mhdr,
          struct cmsghdr *__cmsg) __attribute__ ((__nothrow__ , __leaf__));
# 332 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
enum
  {
    SCM_RIGHTS = 0x01





  };
# 393 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/socket.h" 1 3 4
# 1 "/usr/include/asm-generic/socket.h" 1 3 4




# 1 "/usr/include/x86_64-linux-gnu/asm/sockios.h" 1 3 4
# 1 "/usr/include/asm-generic/sockios.h" 1 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/sockios.h" 2 3 4
# 6 "/usr/include/asm-generic/socket.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/socket.h" 2 3 4
# 394 "/usr/include/x86_64-linux-gnu/bits/socket.h" 2 3 4
# 447 "/usr/include/x86_64-linux-gnu/bits/socket.h" 3 4
struct linger
  {
    int l_onoff;
    int l_linger;
  };
# 34 "/usr/include/x86_64-linux-gnu/sys/socket.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_osockaddr.h" 1 3 4





struct osockaddr
{
  unsigned short int sa_family;
  unsigned char sa_data[14];
};
# 37 "/usr/include/x86_64-linux-gnu/sys/socket.h" 2 3 4




enum
{
  SHUT_RD = 0,

  SHUT_WR,

  SHUT_RDWR

};
# 102 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern int socket (int __domain, int __type, int __protocol) __attribute__ ((__nothrow__ , __leaf__));





extern int socketpair (int __domain, int __type, int __protocol,
         int __fds[2]) __attribute__ ((__nothrow__ , __leaf__));


extern int bind (int __fd, const struct sockaddr * __addr, socklen_t __len)
     __attribute__ ((__nothrow__ , __leaf__));


extern int getsockname (int __fd, struct sockaddr *__restrict __addr,
   socklen_t *__restrict __len) __attribute__ ((__nothrow__ , __leaf__));
# 126 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern int connect (int __fd, const struct sockaddr * __addr, socklen_t __len);



extern int getpeername (int __fd, struct sockaddr *__restrict __addr,
   socklen_t *__restrict __len) __attribute__ ((__nothrow__ , __leaf__));






extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);






extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);






extern ssize_t sendto (int __fd, const void *__buf, size_t __n,
         int __flags, const struct sockaddr * __addr,
         socklen_t __addr_len);
# 163 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n,
    int __flags, struct sockaddr *__restrict __addr,
    socklen_t *__restrict __addr_len);







extern ssize_t sendmsg (int __fd, const struct msghdr *__message,
   int __flags);
# 191 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags);
# 208 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern int getsockopt (int __fd, int __level, int __optname,
         void *__restrict __optval,
         socklen_t *__restrict __optlen) __attribute__ ((__nothrow__ , __leaf__));




extern int setsockopt (int __fd, int __level, int __optname,
         const void *__optval, socklen_t __optlen) __attribute__ ((__nothrow__ , __leaf__));





extern int listen (int __fd, int __n) __attribute__ ((__nothrow__ , __leaf__));
# 232 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern int accept (int __fd, struct sockaddr *__restrict __addr,
     socklen_t *__restrict __addr_len);
# 250 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4
extern int shutdown (int __fd, int __how) __attribute__ ((__nothrow__ , __leaf__));




extern int sockatmark (int __fd) __attribute__ ((__nothrow__ , __leaf__));







extern int isfdtype (int __fd, int __fdtype) __attribute__ ((__nothrow__ , __leaf__));
# 272 "/usr/include/x86_64-linux-gnu/sys/socket.h" 3 4

# 8 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/x86_64-linux-gnu/sys/statvfs.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/sys/statvfs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/statvfs.h" 1 3 4
# 29 "/usr/include/x86_64-linux-gnu/bits/statvfs.h" 3 4
struct statvfs
  {
    unsigned long int f_bsize;
    unsigned long int f_frsize;

    __fsblkcnt_t f_blocks;
    __fsblkcnt_t f_bfree;
    __fsblkcnt_t f_bavail;
    __fsfilcnt_t f_files;
    __fsfilcnt_t f_ffree;
    __fsfilcnt_t f_favail;
# 48 "/usr/include/x86_64-linux-gnu/bits/statvfs.h" 3 4
    unsigned long int f_fsid;



    unsigned long int f_flag;
    unsigned long int f_namemax;
    int __f_spare[6];
  };
# 80 "/usr/include/x86_64-linux-gnu/bits/statvfs.h" 3 4
enum
{
  ST_RDONLY = 1,

  ST_NOSUID = 2
# 109 "/usr/include/x86_64-linux-gnu/bits/statvfs.h" 3 4
};
# 26 "/usr/include/x86_64-linux-gnu/sys/statvfs.h" 2 3 4
# 47 "/usr/include/x86_64-linux-gnu/sys/statvfs.h" 3 4




extern int statvfs (const char *__restrict __file,
      struct statvfs *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 73 "/usr/include/x86_64-linux-gnu/sys/statvfs.h" 3 4
extern int fstatvfs (int __fildes, struct statvfs *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));
# 88 "/usr/include/x86_64-linux-gnu/sys/statvfs.h" 3 4

# 9 "test_dummy_functions_tests.c" 2

# 1 "/usr/include/unistd.h" 1 3 4
# 27 "/usr/include/unistd.h" 3 4

# 202 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/posix_opt.h" 1 3 4
# 203 "/usr/include/unistd.h" 2 3 4



# 1 "/usr/include/x86_64-linux-gnu/bits/environments.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/environments.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/environments.h" 2 3 4
# 207 "/usr/include/unistd.h" 2 3 4
# 226 "/usr/include/unistd.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 227 "/usr/include/unistd.h" 2 3 4
# 255 "/usr/include/unistd.h" 3 4
typedef __useconds_t useconds_t;
# 267 "/usr/include/unistd.h" 3 4
typedef __intptr_t intptr_t;
# 287 "/usr/include/unistd.h" 3 4
extern int access (const char *__name, int __type) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 304 "/usr/include/unistd.h" 3 4
extern int faccessat (int __fd, const char *__file, int __type, int __flag)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2))) ;
# 334 "/usr/include/unistd.h" 3 4
extern __off_t lseek (int __fd, __off_t __offset, int __whence) __attribute__ ((__nothrow__ , __leaf__));
# 353 "/usr/include/unistd.h" 3 4
extern int close (int __fd);






extern ssize_t read (int __fd, void *__buf, size_t __nbytes) ;





extern ssize_t write (int __fd, const void *__buf, size_t __n) ;
# 376 "/usr/include/unistd.h" 3 4
extern ssize_t pread (int __fd, void *__buf, size_t __nbytes,
        __off_t __offset) ;






extern ssize_t pwrite (int __fd, const void *__buf, size_t __n,
         __off_t __offset) ;
# 417 "/usr/include/unistd.h" 3 4
extern int pipe (int __pipedes[2]) __attribute__ ((__nothrow__ , __leaf__)) ;
# 432 "/usr/include/unistd.h" 3 4
extern unsigned int alarm (unsigned int __seconds) __attribute__ ((__nothrow__ , __leaf__));
# 444 "/usr/include/unistd.h" 3 4
extern unsigned int sleep (unsigned int __seconds);







extern __useconds_t ualarm (__useconds_t __value, __useconds_t __interval)
     __attribute__ ((__nothrow__ , __leaf__));






extern int usleep (__useconds_t __useconds);
# 469 "/usr/include/unistd.h" 3 4
extern int pause (void);



extern int chown (const char *__file, __uid_t __owner, __gid_t __group)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;



extern int fchown (int __fd, __uid_t __owner, __gid_t __group) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int lchown (const char *__file, __uid_t __owner, __gid_t __group)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;






extern int fchownat (int __fd, const char *__file, __uid_t __owner,
       __gid_t __group, int __flag)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2))) ;



extern int chdir (const char *__path) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;



extern int fchdir (int __fd) __attribute__ ((__nothrow__ , __leaf__)) ;
# 511 "/usr/include/unistd.h" 3 4
extern char *getcwd (char *__buf, size_t __size) __attribute__ ((__nothrow__ , __leaf__)) ;
# 525 "/usr/include/unistd.h" 3 4
extern char *getwd (char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__deprecated__)) ;




extern int dup (int __fd) __attribute__ ((__nothrow__ , __leaf__)) ;


extern int dup2 (int __fd, int __fd2) __attribute__ ((__nothrow__ , __leaf__));
# 543 "/usr/include/unistd.h" 3 4
extern char **__environ;







extern int execve (const char *__path, char *const __argv[],
     char *const __envp[]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));




extern int fexecve (int __fd, char *const __argv[], char *const __envp[])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));




extern int execv (const char *__path, char *const __argv[])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));



extern int execle (const char *__path, const char *__arg, ...)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));



extern int execl (const char *__path, const char *__arg, ...)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));



extern int execvp (const char *__file, char *const __argv[])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));




extern int execlp (const char *__file, const char *__arg, ...)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 598 "/usr/include/unistd.h" 3 4
extern int nice (int __inc) __attribute__ ((__nothrow__ , __leaf__)) ;




extern void _exit (int __status) __attribute__ ((__noreturn__));





# 1 "/usr/include/x86_64-linux-gnu/bits/confname.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/confname.h" 3 4
enum
  {
    _PC_LINK_MAX,

    _PC_MAX_CANON,

    _PC_MAX_INPUT,

    _PC_NAME_MAX,

    _PC_PATH_MAX,

    _PC_PIPE_BUF,

    _PC_CHOWN_RESTRICTED,

    _PC_NO_TRUNC,

    _PC_VDISABLE,

    _PC_SYNC_IO,

    _PC_ASYNC_IO,

    _PC_PRIO_IO,

    _PC_SOCK_MAXBUF,

    _PC_FILESIZEBITS,

    _PC_REC_INCR_XFER_SIZE,

    _PC_REC_MAX_XFER_SIZE,

    _PC_REC_MIN_XFER_SIZE,

    _PC_REC_XFER_ALIGN,

    _PC_ALLOC_SIZE_MIN,

    _PC_SYMLINK_MAX,

    _PC_2_SYMLINKS

  };


enum
  {
    _SC_ARG_MAX,

    _SC_CHILD_MAX,

    _SC_CLK_TCK,

    _SC_NGROUPS_MAX,

    _SC_OPEN_MAX,

    _SC_STREAM_MAX,

    _SC_TZNAME_MAX,

    _SC_JOB_CONTROL,

    _SC_SAVED_IDS,

    _SC_REALTIME_SIGNALS,

    _SC_PRIORITY_SCHEDULING,

    _SC_TIMERS,

    _SC_ASYNCHRONOUS_IO,

    _SC_PRIORITIZED_IO,

    _SC_SYNCHRONIZED_IO,

    _SC_FSYNC,

    _SC_MAPPED_FILES,

    _SC_MEMLOCK,

    _SC_MEMLOCK_RANGE,

    _SC_MEMORY_PROTECTION,

    _SC_MESSAGE_PASSING,

    _SC_SEMAPHORES,

    _SC_SHARED_MEMORY_OBJECTS,

    _SC_AIO_LISTIO_MAX,

    _SC_AIO_MAX,

    _SC_AIO_PRIO_DELTA_MAX,

    _SC_DELAYTIMER_MAX,

    _SC_MQ_OPEN_MAX,

    _SC_MQ_PRIO_MAX,

    _SC_VERSION,

    _SC_PAGESIZE,


    _SC_RTSIG_MAX,

    _SC_SEM_NSEMS_MAX,

    _SC_SEM_VALUE_MAX,

    _SC_SIGQUEUE_MAX,

    _SC_TIMER_MAX,




    _SC_BC_BASE_MAX,

    _SC_BC_DIM_MAX,

    _SC_BC_SCALE_MAX,

    _SC_BC_STRING_MAX,

    _SC_COLL_WEIGHTS_MAX,

    _SC_EQUIV_CLASS_MAX,

    _SC_EXPR_NEST_MAX,

    _SC_LINE_MAX,

    _SC_RE_DUP_MAX,

    _SC_CHARCLASS_NAME_MAX,


    _SC_2_VERSION,

    _SC_2_C_BIND,

    _SC_2_C_DEV,

    _SC_2_FORT_DEV,

    _SC_2_FORT_RUN,

    _SC_2_SW_DEV,

    _SC_2_LOCALEDEF,


    _SC_PII,

    _SC_PII_XTI,

    _SC_PII_SOCKET,

    _SC_PII_INTERNET,

    _SC_PII_OSI,

    _SC_POLL,

    _SC_SELECT,

    _SC_UIO_MAXIOV,

    _SC_IOV_MAX = _SC_UIO_MAXIOV,

    _SC_PII_INTERNET_STREAM,

    _SC_PII_INTERNET_DGRAM,

    _SC_PII_OSI_COTS,

    _SC_PII_OSI_CLTS,

    _SC_PII_OSI_M,

    _SC_T_IOV_MAX,



    _SC_THREADS,

    _SC_THREAD_SAFE_FUNCTIONS,

    _SC_GETGR_R_SIZE_MAX,

    _SC_GETPW_R_SIZE_MAX,

    _SC_LOGIN_NAME_MAX,

    _SC_TTY_NAME_MAX,

    _SC_THREAD_DESTRUCTOR_ITERATIONS,

    _SC_THREAD_KEYS_MAX,

    _SC_THREAD_STACK_MIN,

    _SC_THREAD_THREADS_MAX,

    _SC_THREAD_ATTR_STACKADDR,

    _SC_THREAD_ATTR_STACKSIZE,

    _SC_THREAD_PRIORITY_SCHEDULING,

    _SC_THREAD_PRIO_INHERIT,

    _SC_THREAD_PRIO_PROTECT,

    _SC_THREAD_PROCESS_SHARED,


    _SC_NPROCESSORS_CONF,

    _SC_NPROCESSORS_ONLN,

    _SC_PHYS_PAGES,

    _SC_AVPHYS_PAGES,

    _SC_ATEXIT_MAX,

    _SC_PASS_MAX,


    _SC_XOPEN_VERSION,

    _SC_XOPEN_XCU_VERSION,

    _SC_XOPEN_UNIX,

    _SC_XOPEN_CRYPT,

    _SC_XOPEN_ENH_I18N,

    _SC_XOPEN_SHM,


    _SC_2_CHAR_TERM,

    _SC_2_C_VERSION,

    _SC_2_UPE,


    _SC_XOPEN_XPG2,

    _SC_XOPEN_XPG3,

    _SC_XOPEN_XPG4,


    _SC_CHAR_BIT,

    _SC_CHAR_MAX,

    _SC_CHAR_MIN,

    _SC_INT_MAX,

    _SC_INT_MIN,

    _SC_LONG_BIT,

    _SC_WORD_BIT,

    _SC_MB_LEN_MAX,

    _SC_NZERO,

    _SC_SSIZE_MAX,

    _SC_SCHAR_MAX,

    _SC_SCHAR_MIN,

    _SC_SHRT_MAX,

    _SC_SHRT_MIN,

    _SC_UCHAR_MAX,

    _SC_UINT_MAX,

    _SC_ULONG_MAX,

    _SC_USHRT_MAX,


    _SC_NL_ARGMAX,

    _SC_NL_LANGMAX,

    _SC_NL_MSGMAX,

    _SC_NL_NMAX,

    _SC_NL_SETMAX,

    _SC_NL_TEXTMAX,


    _SC_XBS5_ILP32_OFF32,

    _SC_XBS5_ILP32_OFFBIG,

    _SC_XBS5_LP64_OFF64,

    _SC_XBS5_LPBIG_OFFBIG,


    _SC_XOPEN_LEGACY,

    _SC_XOPEN_REALTIME,

    _SC_XOPEN_REALTIME_THREADS,


    _SC_ADVISORY_INFO,

    _SC_BARRIERS,

    _SC_BASE,

    _SC_C_LANG_SUPPORT,

    _SC_C_LANG_SUPPORT_R,

    _SC_CLOCK_SELECTION,

    _SC_CPUTIME,

    _SC_THREAD_CPUTIME,

    _SC_DEVICE_IO,

    _SC_DEVICE_SPECIFIC,

    _SC_DEVICE_SPECIFIC_R,

    _SC_FD_MGMT,

    _SC_FIFO,

    _SC_PIPE,

    _SC_FILE_ATTRIBUTES,

    _SC_FILE_LOCKING,

    _SC_FILE_SYSTEM,

    _SC_MONOTONIC_CLOCK,

    _SC_MULTI_PROCESS,

    _SC_SINGLE_PROCESS,

    _SC_NETWORKING,

    _SC_READER_WRITER_LOCKS,

    _SC_SPIN_LOCKS,

    _SC_REGEXP,

    _SC_REGEX_VERSION,

    _SC_SHELL,

    _SC_SIGNALS,

    _SC_SPAWN,

    _SC_SPORADIC_SERVER,

    _SC_THREAD_SPORADIC_SERVER,

    _SC_SYSTEM_DATABASE,

    _SC_SYSTEM_DATABASE_R,

    _SC_TIMEOUTS,

    _SC_TYPED_MEMORY_OBJECTS,

    _SC_USER_GROUPS,

    _SC_USER_GROUPS_R,

    _SC_2_PBS,

    _SC_2_PBS_ACCOUNTING,

    _SC_2_PBS_LOCATE,

    _SC_2_PBS_MESSAGE,

    _SC_2_PBS_TRACK,

    _SC_SYMLOOP_MAX,

    _SC_STREAMS,

    _SC_2_PBS_CHECKPOINT,


    _SC_V6_ILP32_OFF32,

    _SC_V6_ILP32_OFFBIG,

    _SC_V6_LP64_OFF64,

    _SC_V6_LPBIG_OFFBIG,


    _SC_HOST_NAME_MAX,

    _SC_TRACE,

    _SC_TRACE_EVENT_FILTER,

    _SC_TRACE_INHERIT,

    _SC_TRACE_LOG,


    _SC_LEVEL1_ICACHE_SIZE,

    _SC_LEVEL1_ICACHE_ASSOC,

    _SC_LEVEL1_ICACHE_LINESIZE,

    _SC_LEVEL1_DCACHE_SIZE,

    _SC_LEVEL1_DCACHE_ASSOC,

    _SC_LEVEL1_DCACHE_LINESIZE,

    _SC_LEVEL2_CACHE_SIZE,

    _SC_LEVEL2_CACHE_ASSOC,

    _SC_LEVEL2_CACHE_LINESIZE,

    _SC_LEVEL3_CACHE_SIZE,

    _SC_LEVEL3_CACHE_ASSOC,

    _SC_LEVEL3_CACHE_LINESIZE,

    _SC_LEVEL4_CACHE_SIZE,

    _SC_LEVEL4_CACHE_ASSOC,

    _SC_LEVEL4_CACHE_LINESIZE,



    _SC_IPV6 = _SC_LEVEL1_ICACHE_SIZE + 50,

    _SC_RAW_SOCKETS,


    _SC_V7_ILP32_OFF32,

    _SC_V7_ILP32_OFFBIG,

    _SC_V7_LP64_OFF64,

    _SC_V7_LPBIG_OFFBIG,


    _SC_SS_REPL_MAX,


    _SC_TRACE_EVENT_NAME_MAX,

    _SC_TRACE_NAME_MAX,

    _SC_TRACE_SYS_MAX,

    _SC_TRACE_USER_EVENT_MAX,


    _SC_XOPEN_STREAMS,


    _SC_THREAD_ROBUST_PRIO_INHERIT,

    _SC_THREAD_ROBUST_PRIO_PROTECT

  };


enum
  {
    _CS_PATH,


    _CS_V6_WIDTH_RESTRICTED_ENVS,



    _CS_GNU_LIBC_VERSION,

    _CS_GNU_LIBPTHREAD_VERSION,


    _CS_V5_WIDTH_RESTRICTED_ENVS,



    _CS_V7_WIDTH_RESTRICTED_ENVS,



    _CS_LFS_CFLAGS = 1000,

    _CS_LFS_LDFLAGS,

    _CS_LFS_LIBS,

    _CS_LFS_LINTFLAGS,

    _CS_LFS64_CFLAGS,

    _CS_LFS64_LDFLAGS,

    _CS_LFS64_LIBS,

    _CS_LFS64_LINTFLAGS,


    _CS_XBS5_ILP32_OFF32_CFLAGS = 1100,

    _CS_XBS5_ILP32_OFF32_LDFLAGS,

    _CS_XBS5_ILP32_OFF32_LIBS,

    _CS_XBS5_ILP32_OFF32_LINTFLAGS,

    _CS_XBS5_ILP32_OFFBIG_CFLAGS,

    _CS_XBS5_ILP32_OFFBIG_LDFLAGS,

    _CS_XBS5_ILP32_OFFBIG_LIBS,

    _CS_XBS5_ILP32_OFFBIG_LINTFLAGS,

    _CS_XBS5_LP64_OFF64_CFLAGS,

    _CS_XBS5_LP64_OFF64_LDFLAGS,

    _CS_XBS5_LP64_OFF64_LIBS,

    _CS_XBS5_LP64_OFF64_LINTFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_CFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_LDFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_LIBS,

    _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS,


    _CS_POSIX_V6_ILP32_OFF32_CFLAGS,

    _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,

    _CS_POSIX_V6_ILP32_OFF32_LIBS,

    _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_LIBS,

    _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS,

    _CS_POSIX_V6_LP64_OFF64_CFLAGS,

    _CS_POSIX_V6_LP64_OFF64_LDFLAGS,

    _CS_POSIX_V6_LP64_OFF64_LIBS,

    _CS_POSIX_V6_LP64_OFF64_LINTFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS,


    _CS_POSIX_V7_ILP32_OFF32_CFLAGS,

    _CS_POSIX_V7_ILP32_OFF32_LDFLAGS,

    _CS_POSIX_V7_ILP32_OFF32_LIBS,

    _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_LIBS,

    _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS,

    _CS_POSIX_V7_LP64_OFF64_CFLAGS,

    _CS_POSIX_V7_LP64_OFF64_LDFLAGS,

    _CS_POSIX_V7_LP64_OFF64_LIBS,

    _CS_POSIX_V7_LP64_OFF64_LINTFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LIBS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS,


    _CS_V6_ENV,

    _CS_V7_ENV

  };
# 610 "/usr/include/unistd.h" 2 3 4


extern long int pathconf (const char *__path, int __name)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int fpathconf (int __fd, int __name) __attribute__ ((__nothrow__ , __leaf__));


extern long int sysconf (int __name) __attribute__ ((__nothrow__ , __leaf__));



extern size_t confstr (int __name, char *__buf, size_t __len) __attribute__ ((__nothrow__ , __leaf__));




extern __pid_t getpid (void) __attribute__ ((__nothrow__ , __leaf__));


extern __pid_t getppid (void) __attribute__ ((__nothrow__ , __leaf__));


extern __pid_t getpgrp (void) __attribute__ ((__nothrow__ , __leaf__));


extern __pid_t __getpgid (__pid_t __pid) __attribute__ ((__nothrow__ , __leaf__));

extern __pid_t getpgid (__pid_t __pid) __attribute__ ((__nothrow__ , __leaf__));






extern int setpgid (__pid_t __pid, __pid_t __pgid) __attribute__ ((__nothrow__ , __leaf__));
# 660 "/usr/include/unistd.h" 3 4
extern int setpgrp (void) __attribute__ ((__nothrow__ , __leaf__));






extern __pid_t setsid (void) __attribute__ ((__nothrow__ , __leaf__));



extern __pid_t getsid (__pid_t __pid) __attribute__ ((__nothrow__ , __leaf__));



extern __uid_t getuid (void) __attribute__ ((__nothrow__ , __leaf__));


extern __uid_t geteuid (void) __attribute__ ((__nothrow__ , __leaf__));


extern __gid_t getgid (void) __attribute__ ((__nothrow__ , __leaf__));


extern __gid_t getegid (void) __attribute__ ((__nothrow__ , __leaf__));




extern int getgroups (int __size, __gid_t __list[]) __attribute__ ((__nothrow__ , __leaf__)) ;
# 700 "/usr/include/unistd.h" 3 4
extern int setuid (__uid_t __uid) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int setreuid (__uid_t __ruid, __uid_t __euid) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int seteuid (__uid_t __uid) __attribute__ ((__nothrow__ , __leaf__)) ;






extern int setgid (__gid_t __gid) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int setregid (__gid_t __rgid, __gid_t __egid) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int setegid (__gid_t __gid) __attribute__ ((__nothrow__ , __leaf__)) ;
# 756 "/usr/include/unistd.h" 3 4
extern __pid_t fork (void) __attribute__ ((__nothrow__));







extern __pid_t vfork (void) __attribute__ ((__nothrow__ , __leaf__));





extern char *ttyname (int __fd) __attribute__ ((__nothrow__ , __leaf__));



extern int ttyname_r (int __fd, char *__buf, size_t __buflen)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2))) ;



extern int isatty (int __fd) __attribute__ ((__nothrow__ , __leaf__));




extern int ttyslot (void) __attribute__ ((__nothrow__ , __leaf__));




extern int link (const char *__from, const char *__to)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2))) ;




extern int linkat (int __fromfd, const char *__from, int __tofd,
     const char *__to, int __flags)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4))) ;




extern int symlink (const char *__from, const char *__to)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2))) ;




extern ssize_t readlink (const char *__restrict __path,
    char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2))) ;




extern int symlinkat (const char *__from, int __tofd,
        const char *__to) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 3))) ;


extern ssize_t readlinkat (int __fd, const char *__restrict __path,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3))) ;



extern int unlink (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern int unlinkat (int __fd, const char *__name, int __flag)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));



extern int rmdir (const char *__path) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern __pid_t tcgetpgrp (int __fd) __attribute__ ((__nothrow__ , __leaf__));


extern int tcsetpgrp (int __fd, __pid_t __pgrp_id) __attribute__ ((__nothrow__ , __leaf__));






extern char *getlogin (void);







extern int getlogin_r (char *__name, size_t __name_len) __attribute__ ((__nonnull__ (1)));




extern int setlogin (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







# 1 "/usr/include/x86_64-linux-gnu/bits/getopt_posix.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/getopt_posix.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/getopt_core.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/getopt_core.h" 3 4








extern char *optarg;
# 50 "/usr/include/x86_64-linux-gnu/bits/getopt_core.h" 3 4
extern int optind;




extern int opterr;



extern int optopt;
# 91 "/usr/include/x86_64-linux-gnu/bits/getopt_core.h" 3 4
extern int getopt (int ___argc, char *const *___argv, const char *__shortopts)
       __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));


# 28 "/usr/include/x86_64-linux-gnu/bits/getopt_posix.h" 2 3 4


# 49 "/usr/include/x86_64-linux-gnu/bits/getopt_posix.h" 3 4

# 870 "/usr/include/unistd.h" 2 3 4







extern int gethostname (char *__name, size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern int sethostname (const char *__name, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;



extern int sethostid (long int __id) __attribute__ ((__nothrow__ , __leaf__)) ;





extern int getdomainname (char *__name, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
extern int setdomainname (const char *__name, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;





extern int vhangup (void) __attribute__ ((__nothrow__ , __leaf__));


extern int revoke (const char *__file) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;







extern int profil (unsigned short int *__sample_buffer, size_t __size,
     size_t __offset, unsigned int __scale)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern int acct (const char *__name) __attribute__ ((__nothrow__ , __leaf__));



extern char *getusershell (void) __attribute__ ((__nothrow__ , __leaf__));
extern void endusershell (void) __attribute__ ((__nothrow__ , __leaf__));
extern void setusershell (void) __attribute__ ((__nothrow__ , __leaf__));





extern int daemon (int __nochdir, int __noclose) __attribute__ ((__nothrow__ , __leaf__)) ;






extern int chroot (const char *__path) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;



extern char *getpass (const char *__prompt) __attribute__ ((__nonnull__ (1)));







extern int fsync (int __fd);
# 967 "/usr/include/unistd.h" 3 4
extern long int gethostid (void);


extern void sync (void) __attribute__ ((__nothrow__ , __leaf__));





extern int getpagesize (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));




extern int getdtablesize (void) __attribute__ ((__nothrow__ , __leaf__));
# 991 "/usr/include/unistd.h" 3 4
extern int truncate (const char *__file, __off_t __length)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 1014 "/usr/include/unistd.h" 3 4
extern int ftruncate (int __fd, __off_t __length) __attribute__ ((__nothrow__ , __leaf__)) ;
# 1035 "/usr/include/unistd.h" 3 4
extern int brk (void *__addr) __attribute__ ((__nothrow__ , __leaf__)) ;





extern void *sbrk (intptr_t __delta) __attribute__ ((__nothrow__ , __leaf__));
# 1056 "/usr/include/unistd.h" 3 4
extern long int syscall (long int __sysno, ...) __attribute__ ((__nothrow__ , __leaf__));
# 1079 "/usr/include/unistd.h" 3 4
extern int lockf (int __fd, int __cmd, __off_t __len) ;
# 1115 "/usr/include/unistd.h" 3 4
extern int fdatasync (int __fildes);
# 1124 "/usr/include/unistd.h" 3 4
extern char *crypt (const char *__key, const char *__salt)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 1161 "/usr/include/unistd.h" 3 4
int getentropy (void *__buffer, size_t __length) ;








# 11 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/poll.h" 1 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/poll.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/sys/poll.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/poll.h" 1 3 4
# 26 "/usr/include/x86_64-linux-gnu/sys/poll.h" 2 3 4







typedef unsigned long int nfds_t;


struct pollfd
  {
    int fd;
    short int events;
    short int revents;
  };



# 54 "/usr/include/x86_64-linux-gnu/sys/poll.h" 3 4
extern int poll (struct pollfd *__fds, nfds_t __nfds, int __timeout);
# 68 "/usr/include/x86_64-linux-gnu/sys/poll.h" 3 4

# 1 "/usr/include/poll.h" 2 3 4
# 12 "test_dummy_functions_tests.c" 2
# 1 "/usr/include/mntent.h" 1 3 4
# 23 "/usr/include/mntent.h" 3 4
# 1 "/usr/include/paths.h" 1 3 4
# 24 "/usr/include/mntent.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h" 1 3 4



struct _IO_FILE;


typedef struct _IO_FILE FILE;
# 25 "/usr/include/mntent.h" 2 3 4
# 48 "/usr/include/mntent.h" 3 4



struct mntent
  {
    char *mnt_fsname;
    char *mnt_dir;
    char *mnt_type;
    char *mnt_opts;
    int mnt_freq;
    int mnt_passno;
  };




extern FILE *setmntent (const char *__file, const char *__mode) __attribute__ ((__nothrow__ , __leaf__));




extern struct mntent *getmntent (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));



extern struct mntent *getmntent_r (FILE *__restrict __stream,
       struct mntent *__restrict __result,
       char *__restrict __buffer,
       int __bufsize) __attribute__ ((__nothrow__ , __leaf__));




extern int addmntent (FILE *__restrict __stream,
        const struct mntent *__restrict __mnt) __attribute__ ((__nothrow__ , __leaf__));


extern int endmntent (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));



extern char *hasmntopt (const struct mntent *__mnt,
   const char *__opt) __attribute__ ((__nothrow__ , __leaf__));



# 13 "test_dummy_functions_tests.c" 2

# 1 "../../nala/dist/nala.h" 1



# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdbool.h" 1 3 4
# 5 "../../nala/dist/nala.h" 2
# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdint.h" 3 4
# 1 "/usr/include/stdint.h" 1 3 4
# 26 "/usr/include/stdint.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 27 "/usr/include/stdint.h" 2 3 4

# 1 "/usr/include/x86_64-linux-gnu/bits/wchar.h" 1 3 4
# 29 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 30 "/usr/include/stdint.h" 2 3 4







# 1 "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h" 3 4
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
# 38 "/usr/include/stdint.h" 2 3 4





typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least32_t int_least32_t;
typedef __int_least64_t int_least64_t;


typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least64_t uint_least64_t;





typedef signed char int_fast8_t;

typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;
# 71 "/usr/include/stdint.h" 3 4
typedef unsigned char uint_fast8_t;

typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long int uint_fast64_t;
# 90 "/usr/include/stdint.h" 3 4
typedef unsigned long int uintptr_t;
# 101 "/usr/include/stdint.h" 3 4
typedef __intmax_t intmax_t;
typedef __uintmax_t uintmax_t;
# 10 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdint.h" 2 3 4
# 6 "../../nala/dist/nala.h" 2
# 1 "/usr/include/stdio.h" 1 3 4
# 27 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 28 "/usr/include/stdio.h" 2 3 4





# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stddef.h" 1 3 4
# 34 "/usr/include/stdio.h" 2 3 4


# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdarg.h" 1 3 4
# 37 "/usr/include/stdio.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 1 3 4




# 1 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 1 3 4
# 13 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 3 4
typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
# 6 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 2 3 4




typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;
# 40 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4
typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;
# 41 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__FILE.h" 1 3 4



struct _IO_FILE;
typedef struct _IO_FILE __FILE;
# 42 "/usr/include/stdio.h" 2 3 4

# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 1 3 4
# 35 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;




typedef void _IO_lock_t;





struct _IO_FILE
{
  int _flags;


  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;


  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset;


  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;







  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;

  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
};
# 44 "/usr/include/stdio.h" 2 3 4
# 52 "/usr/include/stdio.h" 3 4
typedef __gnuc_va_list va_list;
# 84 "/usr/include/stdio.h" 3 4
typedef __fpos_t fpos_t;
# 133 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 1 3 4
# 134 "/usr/include/stdio.h" 2 3 4



extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;






extern int remove (const char *__filename) __attribute__ ((__nothrow__ , __leaf__));

extern int rename (const char *__old, const char *__new) __attribute__ ((__nothrow__ , __leaf__));



extern int renameat (int __oldfd, const char *__old, int __newfd,
       const char *__new) __attribute__ ((__nothrow__ , __leaf__));
# 173 "/usr/include/stdio.h" 3 4
extern FILE *tmpfile (void) ;
# 187 "/usr/include/stdio.h" 3 4
extern char *tmpnam (char *__s) __attribute__ ((__nothrow__ , __leaf__)) ;




extern char *tmpnam_r (char *__s) __attribute__ ((__nothrow__ , __leaf__)) ;
# 204 "/usr/include/stdio.h" 3 4
extern char *tempnam (const char *__dir, const char *__pfx)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;







extern int fclose (FILE *__stream);




extern int fflush (FILE *__stream);
# 227 "/usr/include/stdio.h" 3 4
extern int fflush_unlocked (FILE *__stream);
# 246 "/usr/include/stdio.h" 3 4
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes) ;




extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream) ;
# 279 "/usr/include/stdio.h" 3 4
extern FILE *fdopen (int __fd, const char *__modes) __attribute__ ((__nothrow__ , __leaf__)) ;
# 292 "/usr/include/stdio.h" 3 4
extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
  __attribute__ ((__nothrow__ , __leaf__)) ;




extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __attribute__ ((__nothrow__ , __leaf__)) ;





extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) __attribute__ ((__nothrow__ , __leaf__));




extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) __attribute__ ((__nothrow__ , __leaf__));


extern void setlinebuf (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));







extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...);




extern int printf (const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) __attribute__ ((__nothrow__));





extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg);




extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nothrow__));



extern int snprintf (char *__restrict __s, size_t __maxlen,
       const char *__restrict __format, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 0)));
# 379 "/usr/include/stdio.h" 3 4
extern int vdprintf (int __fd, const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));







extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format, ...) ;




extern int scanf (const char *__restrict __format, ...) ;

extern int sscanf (const char *__restrict __s,
     const char *__restrict __format, ...) __attribute__ ((__nothrow__ , __leaf__));






extern int fscanf (FILE *__restrict __stream, const char *__restrict __format, ...) __asm__ ("" "__isoc99_fscanf")

                               ;
extern int scanf (const char *__restrict __format, ...) __asm__ ("" "__isoc99_scanf")
                              ;
extern int sscanf (const char *__restrict __s, const char *__restrict __format, ...) __asm__ ("" "__isoc99_sscanf") __attribute__ ((__nothrow__ , __leaf__))

                      ;
# 432 "/usr/include/stdio.h" 3 4
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) ;





extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) ;


extern int vsscanf (const char *__restrict __s,
      const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__format__ (__scanf__, 2, 0)));




extern int vfscanf (FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vfscanf")



     __attribute__ ((__format__ (__scanf__, 2, 0))) ;
extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vscanf")

     __attribute__ ((__format__ (__scanf__, 1, 0))) ;
extern int vsscanf (const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vsscanf") __attribute__ ((__nothrow__ , __leaf__))



     __attribute__ ((__format__ (__scanf__, 2, 0)));
# 485 "/usr/include/stdio.h" 3 4
extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);





extern int getchar (void);






extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
# 510 "/usr/include/stdio.h" 3 4
extern int fgetc_unlocked (FILE *__stream);
# 521 "/usr/include/stdio.h" 3 4
extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);





extern int putchar (int __c);
# 537 "/usr/include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream);







extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream);


extern int putw (int __w, FILE *__stream);







extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     ;
# 603 "/usr/include/stdio.h" 3 4
extern __ssize_t __getdelim (char **__restrict __lineptr,
                             size_t *__restrict __n, int __delimiter,
                             FILE *__restrict __stream) ;
extern __ssize_t getdelim (char **__restrict __lineptr,
                           size_t *__restrict __n, int __delimiter,
                           FILE *__restrict __stream) ;







extern __ssize_t getline (char **__restrict __lineptr,
                          size_t *__restrict __n,
                          FILE *__restrict __stream) ;







extern int fputs (const char *__restrict __s, FILE *__restrict __stream);





extern int puts (const char *__s);






extern int ungetc (int __c, FILE *__stream);






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream) ;




extern size_t fwrite (const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s);
# 673 "/usr/include/stdio.h" 3 4
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite_unlocked (const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream);







extern int fseek (FILE *__stream, long int __off, int __whence);




extern long int ftell (FILE *__stream) ;




extern void rewind (FILE *__stream);
# 707 "/usr/include/stdio.h" 3 4
extern int fseeko (FILE *__stream, __off_t __off, int __whence);




extern __off_t ftello (FILE *__stream) ;
# 731 "/usr/include/stdio.h" 3 4
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);




extern int fsetpos (FILE *__stream, const fpos_t *__pos);
# 757 "/usr/include/stdio.h" 3 4
extern void clearerr (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));

extern int feof (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;

extern int ferror (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;



extern void clearerr_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));
extern int feof_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;
extern int ferror_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;







extern void perror (const char *__s);





# 1 "/usr/include/x86_64-linux-gnu/bits/sys_errlist.h" 1 3 4
# 26 "/usr/include/x86_64-linux-gnu/bits/sys_errlist.h" 3 4
extern int sys_nerr;
extern const char *const sys_errlist[];
# 782 "/usr/include/stdio.h" 2 3 4




extern int fileno (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int fileno_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;
# 800 "/usr/include/stdio.h" 3 4
extern FILE *popen (const char *__command, const char *__modes) ;





extern int pclose (FILE *__stream);





extern char *ctermid (char *__s) __attribute__ ((__nothrow__ , __leaf__));
# 840 "/usr/include/stdio.h" 3 4
extern void flockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));



extern int ftrylockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;


extern void funlockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));
# 858 "/usr/include/stdio.h" 3 4
extern int __uflow (FILE *);
extern int __overflow (FILE *, int);
# 873 "/usr/include/stdio.h" 3 4

# 7 "../../nala/dist/nala.h" 2
# 257 "../../nala/dist/nala.h"

# 257 "../../nala/dist/nala.h"
struct nala_test_t {
    const char *name_p;
    const char *file_p;
    int line;
    void (*func)(void);
    void (*before_fork_func)(void);
    int exit_code;
    int signal_number;
    float elapsed_time_ms;
    struct nala_test_t *next_p;
};


# 269 "../../nala/dist/nala.h" 3 4
_Bool 
# 269 "../../nala/dist/nala.h"
    nala_check_string_equal(const char *actual_p, const char *expected_p);

const char *nala_format(const char *format_p, ...);

const char *nala_format_string(const char *format_p, ...);

const char *nala_format_memory(const char *prefix_p,
                               const void *left_p,
                               const void *right_p,
                               size_t size);


# 280 "../../nala/dist/nala.h" 3 4
_Bool 
# 280 "../../nala/dist/nala.h"
    nala_check_substring(const char *string_p, const char *substring_p);


# 282 "../../nala/dist/nala.h" 3 4
_Bool 
# 282 "../../nala/dist/nala.h"
    nala_check_memory(const void *left_p, const void *right_p, size_t size);

void nala_capture_output_start(char **stdout_pp, char **stderr_pp);

void nala_capture_output_stop(void);

__attribute__ ((noreturn)) void nala_test_failure(const char *file_p,
                                                  int line,
                                                  const char *message_p);

void nala_register_test(struct nala_test_t *test_p);

int nala_run_tests(void);

void nala_reset_all_mocks(void);
# 15 "test_dummy_functions_tests.c" 2
# 1 "test_dummy_functions.h" 1



# 1 "/usr/lib/gcc/x86_64-linux-gnu/8/include/stdarg.h" 1 3 4
# 5 "test_dummy_functions.h" 2

typedef struct DummyStruct
{
    int number;
} DummyStruct;

struct struct_param_type
{
    int number;
};

enum enum_param_type
{
    enum_param_type_a = 0
};

union union_type
{
    struct struct_param_type a;
};

typedef union union_type union_type;

typedef struct struct_param_type struct_param_type;

int add(int x, int y);
void output_message(const char *message);
void keep_args_output_message(const char *message);
void print_hello();
void print_world(void);
DummyStruct *edit_number(DummyStruct *dummy_struct, int number);
DummyStruct *add_one(DummyStruct *dummy_struct);
DummyStruct *compose_twice(DummyStruct *dummy_struct,
                           DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
DummyStruct *add_two(DummyStruct *dummy_struct);
DummyStruct *(*return_add_one(void))(DummyStruct *dummy_struct);
int io_control(int kind, ...);
int io_vcontrol(int kind, va_list ap);
void struct_param(struct struct_param_type *data);
void enum_param(enum enum_param_type value);
int call(int (*callback)(int value));
void in_out(int *buf_p);
struct struct_param_type struct_param_and_return_type(struct struct_param_type arg);
union union_type union_param_and_return_type(union union_type arg);
struct_param_type typedef_struct_param_and_return_type(struct_param_type arg);
union_type typedef_union_param_and_return_type(union_type arg);
int double_pointer(int **value_pp);
# 16 "test_dummy_functions_tests.c" 2

static void function_error_in_subprocess(void (*entry)(void *arg_p),
                                         const char *expected_p)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(entry, 
# 22 "test_dummy_functions_tests.c" 3 4
                                            ((void *)0)
# 22 "test_dummy_functions_tests.c"
                                                );
    do { __typeof__(result_p->exit_code) _nala_assert_left = result_p->exit_code; __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), const char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), default: 
# 23 "test_dummy_functions_tests.c" 3 4
   1
# 23 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), const char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), default: 
# 23 "test_dummy_functions_tests.c" 3 4
   1
# 23 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) != (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s == %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 23 "test_dummy_functions_tests.c" 3 4
   _Bool
# 23 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 23 "test_dummy_functions_tests.c" 3 4
   _Bool
# 23 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 23, nala_format(_nala_assert_format, _nala_assert_left, _nala_assert_right)); } } while (0);;

    if (expected_p != 
# 25 "test_dummy_functions_tests.c" 3 4
                     ((void *)0) 
# 25 "test_dummy_functions_tests.c"
                          ) {
        do { __typeof__(result_p->
# 26 "test_dummy_functions_tests.c" 3 4
       stdout
# 26 "test_dummy_functions_tests.c"
       .buf_p) _nala_assert_left = result_p->
# 26 "test_dummy_functions_tests.c" 3 4
       stdout
# 26 "test_dummy_functions_tests.c"
       .buf_p; __typeof__(expected_p) _nala_assert_right = expected_p; if (!nala_check_substring(_nala_assert_left, _nala_assert_right)) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s doesn't contain %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 26 "test_dummy_functions_tests.c" 3 4
       _Bool
# 26 "test_dummy_functions_tests.c"
       : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 26 "test_dummy_functions_tests.c" 3 4
       _Bool
# 26 "test_dummy_functions_tests.c"
       : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 26, nala_format(_nala_assert_format, _nala_assert_left, _nala_assert_right)); } } while (0);;
    }

    subprocess_result_free(result_p);
}

int fake_add(int x, int y)
{
    return (x + y + 1);
}

static void add_function(void); static void add_function_before_fork() {} static struct nala_test_t nala_test_add_function = { .name_p = "add_function", .file_p = "test_dummy_functions_tests.c", .line = 37, .func = add_function, .before_fork_func = add_function_before_fork, .next_p = 
# 37 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 37 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_add_function(void) { nala_register_test(&nala_test_add_function); } static void add_function(void)
{
    add_mock_once(1, 2, 42);
    do { __typeof__(add(1, 2)) _nala_assert_left = add(1, 2); __typeof__(42) _nala_assert_right = 42; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 40 "test_dummy_functions_tests.c" 3 4
   0
# 40 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 40 "test_dummy_functions_tests.c" 3 4
   0
# 40 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 40 "test_dummy_functions_tests.c" 3 4
   _Bool
# 40 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 40 "test_dummy_functions_tests.c" 3 4
   _Bool
# 40 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 40, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    add_mock_disable();
    do { __typeof__(add(1, 2)) _nala_assert_left = add(1, 2); __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 43 "test_dummy_functions_tests.c" 3 4
   0
# 43 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 43 "test_dummy_functions_tests.c" 3 4
   0
# 43 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 43 "test_dummy_functions_tests.c" 3 4
   _Bool
# 43 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 43 "test_dummy_functions_tests.c" 3 4
   _Bool
# 43 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 43, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    add_mock_implementation(fake_add);
    do { __typeof__(add(1, 2)) _nala_assert_left = add(1, 2); __typeof__(4) _nala_assert_right = 4; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 46 "test_dummy_functions_tests.c" 3 4
   0
# 46 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 46 "test_dummy_functions_tests.c" 3 4
   0
# 46 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 46 "test_dummy_functions_tests.c" 3 4
   _Bool
# 46 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 46 "test_dummy_functions_tests.c" 3 4
   _Bool
# 46 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 46, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    add_mock_ignore_in(42);
    do { __typeof__(add(0, 0)) _nala_assert_left = add(0, 0); __typeof__(42) _nala_assert_right = 42; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 49 "test_dummy_functions_tests.c" 3 4
   0
# 49 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 49 "test_dummy_functions_tests.c" 3 4
   0
# 49 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 49 "test_dummy_functions_tests.c" 3 4
   _Bool
# 49 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 49 "test_dummy_functions_tests.c" 3 4
   _Bool
# 49 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 49, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(add(1, 0)) _nala_assert_left = add(1, 0); __typeof__(42) _nala_assert_right = 42; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 50 "test_dummy_functions_tests.c" 3 4
   0
# 50 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 50 "test_dummy_functions_tests.c" 3 4
   0
# 50 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 50 "test_dummy_functions_tests.c" 3 4
   _Bool
# 50 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 50 "test_dummy_functions_tests.c" 3 4
   _Bool
# 50 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 50, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(add(1, 1)) _nala_assert_left = add(1, 1); __typeof__(42) _nala_assert_right = 42; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 51 "test_dummy_functions_tests.c" 3 4
   0
# 51 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 51 "test_dummy_functions_tests.c" 3 4
   0
# 51 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 51 "test_dummy_functions_tests.c" 3 4
   _Bool
# 51 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 51 "test_dummy_functions_tests.c" 3 4
   _Bool
# 51 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 51, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    add_mock_ignore_in_once(41);
    do { __typeof__(add(5, -1)) _nala_assert_left = add(5, -1); __typeof__(41) _nala_assert_right = 41; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 54 "test_dummy_functions_tests.c" 3 4
   0
# 54 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 54 "test_dummy_functions_tests.c" 3 4
   0
# 54 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 54 "test_dummy_functions_tests.c" 3 4
   _Bool
# 54 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 54 "test_dummy_functions_tests.c" 3 4
   _Bool
# 54 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 54, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void add_function_error_wrong_x_entry(void *arg_p)
{
    (void)arg_p;

    add_mock_once(1, 2, 42);
    add(3, 2);
}

static void add_function_error_wrong_x(void); static void add_function_error_wrong_x_before_fork() {} static struct nala_test_t nala_test_add_function_error_wrong_x = { .name_p = "add_function_error_wrong_x", .file_p = "test_dummy_functions_tests.c", .line = 65, .func = add_function_error_wrong_x, .before_fork_func = add_function_error_wrong_x_before_fork, .next_p = 
# 65 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 65 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_add_function_error_wrong_x(void) { nala_register_test(&nala_test_add_function_error_wrong_x); } static void add_function_error_wrong_x(void)
{
    function_error_in_subprocess(add_function_error_wrong_x_entry,
                                 "Mocked add(x): 1 != 3");
}

static int add_function_set_callback_callback_x;
static int add_function_set_callback_callback_y;

static void add_function_set_callback_callback(int x, int y)
{
    add_function_set_callback_callback_x = x;
    add_function_set_callback_callback_y = y;
}

static void add_function_set_callback(void); static void add_function_set_callback_before_fork() {} static struct nala_test_t nala_test_add_function_set_callback = { .name_p = "add_function_set_callback", .file_p = "test_dummy_functions_tests.c", .line = 80, .func = add_function_set_callback, .before_fork_func = add_function_set_callback_before_fork, .next_p = 
# 80 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 80 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_add_function_set_callback(void) { nala_register_test(&nala_test_add_function_set_callback); } static void add_function_set_callback(void)
{
    add_function_set_callback_callback_x = 0;
    add_function_set_callback_callback_y = 0;

    add_mock_once(10, 13, 23);
    add_mock_set_callback(add_function_set_callback_callback);

    do { __typeof__(add(10, 13)) _nala_assert_left = add(10, 13); __typeof__(23) _nala_assert_right = 23; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 88 "test_dummy_functions_tests.c" 3 4
   0
# 88 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 88 "test_dummy_functions_tests.c" 3 4
   0
# 88 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 88 "test_dummy_functions_tests.c" 3 4
   _Bool
# 88 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 88 "test_dummy_functions_tests.c" 3 4
   _Bool
# 88 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 88, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    do { __typeof__(add_function_set_callback_callback_x) _nala_assert_left = add_function_set_callback_callback_x; __typeof__(10) _nala_assert_right = 10; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 90 "test_dummy_functions_tests.c" 3 4
   0
# 90 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 90 "test_dummy_functions_tests.c" 3 4
   0
# 90 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 90 "test_dummy_functions_tests.c" 3 4
   _Bool
# 90 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 90 "test_dummy_functions_tests.c" 3 4
   _Bool
# 90 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 90, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(add_function_set_callback_callback_y) _nala_assert_left = add_function_set_callback_callback_y; __typeof__(13) _nala_assert_right = 13; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 91 "test_dummy_functions_tests.c" 3 4
   0
# 91 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 91 "test_dummy_functions_tests.c" 3 4
   0
# 91 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 91 "test_dummy_functions_tests.c" 3 4
   _Bool
# 91 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 91 "test_dummy_functions_tests.c" 3 4
   _Bool
# 91 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 91, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void output_message_function(void); static void output_message_function_before_fork() {} static struct nala_test_t nala_test_output_message_function = { .name_p = "output_message_function", .file_p = "test_dummy_functions_tests.c", .line = 94, .func = output_message_function, .before_fork_func = output_message_function_before_fork, .next_p = 
# 94 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 94 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_output_message_function(void) { nala_register_test(&nala_test_output_message_function); } static void output_message_function(void)
{
    output_message_mock_once("a");
    output_message("a");
}

static void output_message_function_error_mock_null_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once(
# 104 "test_dummy_functions_tests.c" 3 4
                            ((void *)0)
# 104 "test_dummy_functions_tests.c"
                                );
    output_message("a");
}

static void output_message_function_error_mock_null(void); static void output_message_function_error_mock_null_before_fork() {} static struct nala_test_t nala_test_output_message_function_error_mock_null = { .name_p = "output_message_function_error_mock_null", .file_p = "test_dummy_functions_tests.c", .line = 108, .func = output_message_function_error_mock_null, .before_fork_func = output_message_function_error_mock_null_before_fork, .next_p = 
# 108 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 108 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_output_message_function_error_mock_null(void) { nala_register_test(&nala_test_output_message_function_error_mock_null); } static void output_message_function_error_mock_null(void)
{
    function_error_in_subprocess(
        output_message_function_error_mock_null_entry,
        "Mocked output_message(message): \"(null)\" != \"a\"");
}

static void output_message_function_error_call_null_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("a");
    output_message(
# 120 "test_dummy_functions_tests.c" 3 4
                  ((void *)0)
# 120 "test_dummy_functions_tests.c"
                      );
}

static void output_message_function_error_call_null(void); static void output_message_function_error_call_null_before_fork() {} static struct nala_test_t nala_test_output_message_function_error_call_null = { .name_p = "output_message_function_error_call_null", .file_p = "test_dummy_functions_tests.c", .line = 123, .func = output_message_function_error_call_null, .before_fork_func = output_message_function_error_call_null_before_fork, .next_p = 
# 123 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 123 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_output_message_function_error_call_null(void) { nala_register_test(&nala_test_output_message_function_error_call_null); } static void output_message_function_error_call_null(void)
{
    function_error_in_subprocess(
        output_message_function_error_call_null_entry,
        "Memory mismatch. See diff for details.");
}

static void output_message_function_error_mismatch_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("a");
    output_message("b");
}

static void output_message_function_error_mismatch(void); static void output_message_function_error_mismatch_before_fork() {} static struct nala_test_t nala_test_output_message_function_error_mismatch = { .name_p = "output_message_function_error_mismatch", .file_p = "test_dummy_functions_tests.c", .line = 138, .func = output_message_function_error_mismatch, .before_fork_func = output_message_function_error_mismatch_before_fork, .next_p = 
# 138 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 138 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_output_message_function_error_mismatch(void) { nala_register_test(&nala_test_output_message_function_error_mismatch); } static void output_message_function_error_mismatch(void)
{
    function_error_in_subprocess(
        output_message_function_error_mismatch_entry,
        "Mocked output_message(message): Memory mismatch. See diff for details.");
}

static void time_function(void); static void time_function_before_fork() {} static struct nala_test_t nala_test_time_function = { .name_p = "time_function", .file_p = "test_dummy_functions_tests.c", .line = 145, .func = time_function, .before_fork_func = time_function_before_fork, .next_p = 
# 145 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 145 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_time_function(void) { nala_register_test(&nala_test_time_function); } static void time_function(void)
{
    time_t start = time(
# 147 "test_dummy_functions_tests.c" 3 4
                       ((void *)0)
# 147 "test_dummy_functions_tests.c"
                           );

    time_mock_once(0);
    do { __typeof__(time(
# 150 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 150 "test_dummy_functions_tests.c"
   )) _nala_assert_left = time(
# 150 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 150 "test_dummy_functions_tests.c"
   ); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 150 "test_dummy_functions_tests.c" 3 4
   0
# 150 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 150 "test_dummy_functions_tests.c" 3 4
   0
# 150 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 150 "test_dummy_functions_tests.c" 3 4
   _Bool
# 150 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 150 "test_dummy_functions_tests.c" 3 4
   _Bool
# 150 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 150, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    time_mock_once(0);
    time_mock_set_tloc_in_pointer(
# 153 "test_dummy_functions_tests.c" 3 4
                                 ((void *)0)
# 153 "test_dummy_functions_tests.c"
                                     );
    do { __typeof__(time(
# 154 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 154 "test_dummy_functions_tests.c"
   )) _nala_assert_left = time(
# 154 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 154 "test_dummy_functions_tests.c"
   ); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 154 "test_dummy_functions_tests.c" 3 4
   0
# 154 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 154 "test_dummy_functions_tests.c" 3 4
   0
# 154 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 154 "test_dummy_functions_tests.c" 3 4
   _Bool
# 154 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 154 "test_dummy_functions_tests.c" 3 4
   _Bool
# 154 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 154, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    time_mock_disable();
    do { __typeof__(time(
# 157 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 157 "test_dummy_functions_tests.c"
   )) _nala_assert_left = time(
# 157 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 157 "test_dummy_functions_tests.c"
   ); __typeof__(start) _nala_assert_right = start; if (!((_nala_assert_left) >= (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s < %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 157 "test_dummy_functions_tests.c" 3 4
   _Bool
# 157 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 157 "test_dummy_functions_tests.c" 3 4
   _Bool
# 157 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 157, nala_format(_nala_assert_format, _nala_assert_left, _nala_assert_right)); } } while (0);;
}

static void pipe_function_set_out(void); static void pipe_function_set_out_before_fork() {} static struct nala_test_t nala_test_pipe_function_set_out = { .name_p = "pipe_function_set_out", .file_p = "test_dummy_functions_tests.c", .line = 160, .func = pipe_function_set_out, .before_fork_func = pipe_function_set_out_before_fork, .next_p = 
# 160 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 160 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_pipe_function_set_out(void) { nala_register_test(&nala_test_pipe_function_set_out); } static void pipe_function_set_out(void)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(42);
    pipe_mock_set_pipefd_out(fd, sizeof(fd));
    memset(&fd[0], 0, sizeof(fd));

    res = pipe(fd);

    do { __typeof__(res) _nala_assert_left = res; __typeof__(42) _nala_assert_right = 42; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 171 "test_dummy_functions_tests.c" 3 4
   0
# 171 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 171 "test_dummy_functions_tests.c" 3 4
   0
# 171 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 171 "test_dummy_functions_tests.c" 3 4
   _Bool
# 171 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 171 "test_dummy_functions_tests.c" 3 4
   _Bool
# 171 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 171, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fd[0]) _nala_assert_left = fd[0]; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 172 "test_dummy_functions_tests.c" 3 4
   0
# 172 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 172 "test_dummy_functions_tests.c" 3 4
   0
# 172 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 172 "test_dummy_functions_tests.c" 3 4
   _Bool
# 172 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 172 "test_dummy_functions_tests.c" 3 4
   _Bool
# 172 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 172, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fd[1]) _nala_assert_left = fd[1]; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 173 "test_dummy_functions_tests.c" 3 4
   0
# 173 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 173 "test_dummy_functions_tests.c" 3 4
   0
# 173 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 173 "test_dummy_functions_tests.c" 3 4
   _Bool
# 173 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 173 "test_dummy_functions_tests.c" 3 4
   _Bool
# 173 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 173, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void pipe_function_set_in(void); static void pipe_function_set_in_before_fork() {} static struct nala_test_t nala_test_pipe_function_set_in = { .name_p = "pipe_function_set_in", .file_p = "test_dummy_functions_tests.c", .line = 176, .func = pipe_function_set_in, .before_fork_func = pipe_function_set_in_before_fork, .next_p = 
# 176 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 176 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_pipe_function_set_in(void) { nala_register_test(&nala_test_pipe_function_set_in); } static void pipe_function_set_in(void)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(41);
    pipe_mock_set_pipefd_in(fd, sizeof(fd));

    res = pipe(fd);

    do { __typeof__(res) _nala_assert_left = res; __typeof__(41) _nala_assert_right = 41; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 186 "test_dummy_functions_tests.c" 3 4
   0
# 186 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 186 "test_dummy_functions_tests.c" 3 4
   0
# 186 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 186 "test_dummy_functions_tests.c" 3 4
   _Bool
# 186 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 186 "test_dummy_functions_tests.c" 3 4
   _Bool
# 186 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 186, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fd[0]) _nala_assert_left = fd[0]; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 187 "test_dummy_functions_tests.c" 3 4
   0
# 187 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 187 "test_dummy_functions_tests.c" 3 4
   0
# 187 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 187 "test_dummy_functions_tests.c" 3 4
   _Bool
# 187 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 187 "test_dummy_functions_tests.c" 3 4
   _Bool
# 187 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 187, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fd[1]) _nala_assert_left = fd[1]; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 188 "test_dummy_functions_tests.c" 3 4
   0
# 188 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 188 "test_dummy_functions_tests.c" 3 4
   0
# 188 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 188 "test_dummy_functions_tests.c" 3 4
   _Bool
# 188 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 188 "test_dummy_functions_tests.c" 3 4
   _Bool
# 188 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 188, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void pipe_function_set_errno(void); static void pipe_function_set_errno_before_fork() {} static struct nala_test_t nala_test_pipe_function_set_errno = { .name_p = "pipe_function_set_errno", .file_p = "test_dummy_functions_tests.c", .line = 191, .func = pipe_function_set_errno, .before_fork_func = pipe_function_set_errno_before_fork, .next_p = 
# 191 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 191 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_pipe_function_set_errno(void) { nala_register_test(&nala_test_pipe_function_set_errno); } static void pipe_function_set_errno(void)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(-1);
    pipe_mock_set_errno(
# 197 "test_dummy_functions_tests.c" 3 4
                       12
# 197 "test_dummy_functions_tests.c"
                             );

    res = pipe(fd);

    do { __typeof__(res) _nala_assert_left = res; __typeof__(-1) _nala_assert_right = -1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 201 "test_dummy_functions_tests.c" 3 4
   0
# 201 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 201 "test_dummy_functions_tests.c" 3 4
   0
# 201 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 201 "test_dummy_functions_tests.c" 3 4
   _Bool
# 201 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 201 "test_dummy_functions_tests.c" 3 4
   _Bool
# 201 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 201, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(
# 202 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ())
# 202 "test_dummy_functions_tests.c"
   ) _nala_assert_left = 
# 202 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ())
# 202 "test_dummy_functions_tests.c"
   ; __typeof__(
# 202 "test_dummy_functions_tests.c" 3 4
   12
# 202 "test_dummy_functions_tests.c"
   ) _nala_assert_right = 
# 202 "test_dummy_functions_tests.c" 3 4
   12
# 202 "test_dummy_functions_tests.c"
   ; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 202 "test_dummy_functions_tests.c" 3 4
   0
# 202 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 202 "test_dummy_functions_tests.c" 3 4
   0
# 202 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 202 "test_dummy_functions_tests.c" 3 4
   _Bool
# 202 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 202 "test_dummy_functions_tests.c" 3 4
   _Bool
# 202 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 202, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void mount_function(void); static void mount_function_before_fork() {} static struct nala_test_t nala_test_mount_function = { .name_p = "mount_function", .file_p = "test_dummy_functions_tests.c", .line = 205, .func = mount_function, .before_fork_func = mount_function_before_fork, .next_p = 
# 205 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 205 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_mount_function(void) { nala_register_test(&nala_test_mount_function); } static void mount_function(void)
{
    do { __typeof__(mount("a", "b", "c", 0, "")) _nala_assert_left = mount("a", "b", "c", 0, ""); __typeof__(-1) _nala_assert_right = -1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 207 "test_dummy_functions_tests.c" 3 4
   0
# 207 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 207 "test_dummy_functions_tests.c" 3 4
   0
# 207 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 207 "test_dummy_functions_tests.c" 3 4
   _Bool
# 207 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 207 "test_dummy_functions_tests.c" 3 4
   _Bool
# 207 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 207, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    mount_mock_once("a", "b", "c", 0, 0);
    mount_mock_set_data_in("", 1);

    do { __typeof__(mount("a", "b", "c", 0, "")) _nala_assert_left = mount("a", "b", "c", 0, ""); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 212 "test_dummy_functions_tests.c" 3 4
   0
# 212 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 212 "test_dummy_functions_tests.c" 3 4
   0
# 212 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 212 "test_dummy_functions_tests.c" 3 4
   _Bool
# 212 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 212 "test_dummy_functions_tests.c" 3 4
   _Bool
# 212 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 212, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    mount_mock_once(
# 214 "test_dummy_functions_tests.c" 3 4
                   ((void *)0)
# 214 "test_dummy_functions_tests.c"
                       , "b", "c", 0, 0);
    mount_mock_set_data_in("", 1);

    do { __typeof__(mount(
# 217 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 217 "test_dummy_functions_tests.c"
   , "b", "c", 0, "")) _nala_assert_left = mount(
# 217 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 217 "test_dummy_functions_tests.c"
   , "b", "c", 0, ""); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 217 "test_dummy_functions_tests.c" 3 4
   0
# 217 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 217 "test_dummy_functions_tests.c" 3 4
   0
# 217 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 217 "test_dummy_functions_tests.c" 3 4
   _Bool
# 217 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 217 "test_dummy_functions_tests.c" 3 4
   _Bool
# 217 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 217, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

DummyStruct *fake_edit_number(DummyStruct *dummy_struct, int number)
{
    dummy_struct->number = 2 * number;

    return (dummy_struct + 1);
}

static void implementation(void); static void implementation_before_fork() {} static struct nala_test_t nala_test_implementation = { .name_p = "implementation", .file_p = "test_dummy_functions_tests.c", .line = 227, .func = implementation, .before_fork_func = implementation_before_fork, .next_p = 
# 227 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 227 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_implementation(void) { nala_register_test(&nala_test_implementation); } static void implementation(void)
{
    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };
    DummyStruct *dummy_ptr = edit_number(&array[0], 42);

    do { __typeof__(dummy_ptr) _nala_assert_left = dummy_ptr; __typeof__(&array[0]) _nala_assert_right = &array[0]; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 232 "test_dummy_functions_tests.c" 3 4
   0
# 232 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 232 "test_dummy_functions_tests.c" 3 4
   0
# 232 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 232 "test_dummy_functions_tests.c" 3 4
   _Bool
# 232 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 232 "test_dummy_functions_tests.c" 3 4
   _Bool
# 232 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 232, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(array[0].number) _nala_assert_left = array[0].number; __typeof__(42) _nala_assert_right = 42; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 233 "test_dummy_functions_tests.c" 3 4
   0
# 233 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 233 "test_dummy_functions_tests.c" 3 4
   0
# 233 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 233 "test_dummy_functions_tests.c" 3 4
   _Bool
# 233 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 233 "test_dummy_functions_tests.c" 3 4
   _Bool
# 233 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 233, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    edit_number_mock_implementation(fake_edit_number);

    dummy_ptr = edit_number(&array[0], -1);

    do { __typeof__(dummy_ptr) _nala_assert_left = dummy_ptr; __typeof__(&array[1]) _nala_assert_right = &array[1]; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 239 "test_dummy_functions_tests.c" 3 4
   0
# 239 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 239 "test_dummy_functions_tests.c" 3 4
   0
# 239 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 239 "test_dummy_functions_tests.c" 3 4
   _Bool
# 239 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 239 "test_dummy_functions_tests.c" 3 4
   _Bool
# 239 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 239, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(array[0].number) _nala_assert_left = array[0].number; __typeof__(-2) _nala_assert_right = -2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 240 "test_dummy_functions_tests.c" 3 4
   0
# 240 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 240 "test_dummy_functions_tests.c" 3 4
   0
# 240 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 240 "test_dummy_functions_tests.c" 3 4
   _Bool
# 240 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 240 "test_dummy_functions_tests.c" 3 4
   _Bool
# 240 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 240, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void once_in_and_in_pointer(void); static void once_in_and_in_pointer_before_fork() {} static struct nala_test_t nala_test_once_in_and_in_pointer = { .name_p = "once_in_and_in_pointer", .file_p = "test_dummy_functions_tests.c", .line = 243, .func = once_in_and_in_pointer, .before_fork_func = once_in_and_in_pointer_before_fork, .next_p = 
# 243 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 243 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_once_in_and_in_pointer(void) { nala_register_test(&nala_test_once_in_and_in_pointer); } static void once_in_and_in_pointer(void)
{
    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };
    DummyStruct *dummy_ptr = edit_number(&array[0], 42);

    edit_number_mock_once(5, &array[1]);
    edit_number_mock_set_dummy_struct_in_pointer(&array[0]);
    edit_number_mock_set_dummy_struct_in(&array[0], sizeof(array));

    dummy_ptr = edit_number(&array[0], 5);

    do { __typeof__(dummy_ptr) _nala_assert_left = dummy_ptr; __typeof__(&array[1]) _nala_assert_right = &array[1]; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 254 "test_dummy_functions_tests.c" 3 4
   0
# 254 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 254 "test_dummy_functions_tests.c" 3 4
   0
# 254 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 254 "test_dummy_functions_tests.c" 3 4
   _Bool
# 254 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 254 "test_dummy_functions_tests.c" 3 4
   _Bool
# 254 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 254, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void once_in_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock_once(5, &array[1]);
    edit_number_mock_set_dummy_struct_in(&array[1], sizeof(array[0]));
    edit_number(&array[0], 5);
}

static void once_in_error(void); static void once_in_error_before_fork() {} static struct nala_test_t nala_test_once_in_error = { .name_p = "once_in_error", .file_p = "test_dummy_functions_tests.c", .line = 268, .func = once_in_error, .before_fork_func = once_in_error_before_fork, .next_p = 
# 268 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 268 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_once_in_error(void) { nala_register_test(&nala_test_once_in_error); } static void once_in_error(void)
{
    function_error_in_subprocess(once_in_error_entry, "Memory mismatch.");
}

static void in_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock(5, &array[1]);
    edit_number_mock_set_dummy_struct_in(&array[1], sizeof(array[0]));
    edit_number(&array[0], 5);
}

static void in_error(void); static void in_error_before_fork() {} static struct nala_test_t nala_test_in_error = { .name_p = "in_error", .file_p = "test_dummy_functions_tests.c", .line = 284, .func = in_error, .before_fork_func = in_error_before_fork, .next_p = 
# 284 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 284 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_in_error(void) { nala_register_test(&nala_test_in_error); } static void in_error(void)
{
    function_error_in_subprocess(in_error_entry, "Memory mismatch.");
}

static void in_pointer_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock(5, &array[1]);
    edit_number_mock_set_dummy_struct_in_pointer(&array[1]);
    edit_number(&array[0], 5);
}

static void in_pointer_error(void); static void in_pointer_error_before_fork() {} static struct nala_test_t nala_test_in_pointer_error = { .name_p = "in_pointer_error", .file_p = "test_dummy_functions_tests.c", .line = 300, .func = in_pointer_error, .before_fork_func = in_pointer_error_before_fork, .next_p = 
# 300 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 300 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_in_pointer_error(void) { nala_register_test(&nala_test_in_pointer_error); } static void in_pointer_error(void)
{
    function_error_in_subprocess(in_pointer_error_entry,
                                 "Mocked edit_number(dummy_struct): ");
}

static void once_in_pointer_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock_once(5, &array[1]);
    edit_number_mock_set_dummy_struct_in_pointer(&array[1]);
    edit_number(&array[0], 5);
}

static void once_in_pointer_error(void); static void once_in_pointer_error_before_fork() {} static struct nala_test_t nala_test_once_in_pointer_error = { .name_p = "once_in_pointer_error", .file_p = "test_dummy_functions_tests.c", .line = 317, .func = once_in_pointer_error, .before_fork_func = once_in_pointer_error_before_fork, .next_p = 
# 317 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 317 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_once_in_pointer_error(void) { nala_register_test(&nala_test_once_in_pointer_error); } static void once_in_pointer_error(void)
{
    function_error_in_subprocess(once_in_pointer_error_entry,
                                 "Mocked edit_number(dummy_struct): ");
}

static void set_errno(void); static void set_errno_before_fork() {} static struct nala_test_t nala_test_set_errno = { .name_p = "set_errno", .file_p = "test_dummy_functions_tests.c", .line = 323, .func = set_errno, .before_fork_func = set_errno_before_fork, .next_p = 
# 323 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 323 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_set_errno(void) { nala_register_test(&nala_test_set_errno); } static void set_errno(void)
{
    fopen_mock_once("test_dummy_functions_tests.c", "r", 
# 325 "test_dummy_functions_tests.c" 3 4
                                  ((void *)0)
# 325 "test_dummy_functions_tests.c"
                                      );
    fopen_mock_set_errno(
# 326 "test_dummy_functions_tests.c" 3 4
                        2
# 326 "test_dummy_functions_tests.c"
                              );

    FILE *f = fopen("test_dummy_functions_tests.c", "r");

    do { __typeof__(f) _nala_assert_left = f; __typeof__(
# 330 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 330 "test_dummy_functions_tests.c"
   ) _nala_assert_right = 
# 330 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 330 "test_dummy_functions_tests.c"
   ; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 330 "test_dummy_functions_tests.c" 3 4
   0
# 330 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 330 "test_dummy_functions_tests.c" 3 4
   0
# 330 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 330 "test_dummy_functions_tests.c" 3 4
   _Bool
# 330 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 330 "test_dummy_functions_tests.c" 3 4
   _Bool
# 330 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 330, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(
# 331 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ())
# 331 "test_dummy_functions_tests.c"
   ) _nala_assert_left = 
# 331 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ())
# 331 "test_dummy_functions_tests.c"
   ; __typeof__(
# 331 "test_dummy_functions_tests.c" 3 4
   2
# 331 "test_dummy_functions_tests.c"
   ) _nala_assert_right = 
# 331 "test_dummy_functions_tests.c" 3 4
   2
# 331 "test_dummy_functions_tests.c"
   ; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 331 "test_dummy_functions_tests.c" 3 4
   0
# 331 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 331 "test_dummy_functions_tests.c" 3 4
   0
# 331 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 331 "test_dummy_functions_tests.c" 3 4
   _Bool
# 331 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 331 "test_dummy_functions_tests.c" 3 4
   _Bool
# 331 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 331, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    fopen_mock_disable();

    
# 335 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ()) 
# 335 "test_dummy_functions_tests.c"
         = 0;

    f = fopen("test_dummy_functions_tests.c", "r");
    do { __typeof__(f) _nala_assert_left = f; __typeof__(
# 338 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 338 "test_dummy_functions_tests.c"
   ) _nala_assert_right = 
# 338 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 338 "test_dummy_functions_tests.c"
   ; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), const char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), default: 
# 338 "test_dummy_functions_tests.c" 3 4
   1
# 338 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), const char *: (!nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right))), default: 
# 338 "test_dummy_functions_tests.c" 3 4
   1
# 338 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) != (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s == %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 338 "test_dummy_functions_tests.c" 3 4
   _Bool
# 338 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 338 "test_dummy_functions_tests.c" 3 4
   _Bool
# 338 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 338, nala_format(_nala_assert_format, _nala_assert_left, _nala_assert_right)); } } while (0);;
    fclose(f);

    do { __typeof__(
# 341 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ())
# 341 "test_dummy_functions_tests.c"
   ) _nala_assert_left = 
# 341 "test_dummy_functions_tests.c" 3 4
   (*__errno_location ())
# 341 "test_dummy_functions_tests.c"
   ; __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 341 "test_dummy_functions_tests.c" 3 4
   0
# 341 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 341 "test_dummy_functions_tests.c" 3 4
   0
# 341 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 341 "test_dummy_functions_tests.c" 3 4
   _Bool
# 341 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 341 "test_dummy_functions_tests.c" 3 4
   _Bool
# 341 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 341, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static 
# 344 "test_dummy_functions_tests.c" 3 4
      _Bool 
# 344 "test_dummy_functions_tests.c"
           variadic_function_callback_called;

void variadic_function_callback(int kind, va_list vl)
{
    do { __typeof__(kind) _nala_assert_left = kind; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 348 "test_dummy_functions_tests.c" 3 4
   0
# 348 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 348 "test_dummy_functions_tests.c" 3 4
   0
# 348 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 348 "test_dummy_functions_tests.c" 3 4
   _Bool
# 348 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 348 "test_dummy_functions_tests.c" 3 4
   _Bool
# 348 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 348, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(
# 349 "test_dummy_functions_tests.c" 3 4
   __builtin_va_arg(
# 349 "test_dummy_functions_tests.c"
   vl
# 349 "test_dummy_functions_tests.c" 3 4
   ,
# 349 "test_dummy_functions_tests.c"
   int
# 349 "test_dummy_functions_tests.c" 3 4
   )
# 349 "test_dummy_functions_tests.c"
   ) _nala_assert_left = 
# 349 "test_dummy_functions_tests.c" 3 4
   __builtin_va_arg(
# 349 "test_dummy_functions_tests.c"
   vl
# 349 "test_dummy_functions_tests.c" 3 4
   ,
# 349 "test_dummy_functions_tests.c"
   int
# 349 "test_dummy_functions_tests.c" 3 4
   )
# 349 "test_dummy_functions_tests.c"
   ; __typeof__(10) _nala_assert_right = 10; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 349 "test_dummy_functions_tests.c" 3 4
   0
# 349 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 349 "test_dummy_functions_tests.c" 3 4
   0
# 349 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 349 "test_dummy_functions_tests.c" 3 4
   _Bool
# 349 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 349 "test_dummy_functions_tests.c" 3 4
   _Bool
# 349 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 349, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    variadic_function_callback_called = 
# 350 "test_dummy_functions_tests.c" 3 4
                                       1
# 350 "test_dummy_functions_tests.c"
                                           ;
}

static void variadic_function(void); static void variadic_function_before_fork() {} static struct nala_test_t nala_test_variadic_function = { .name_p = "variadic_function", .file_p = "test_dummy_functions_tests.c", .line = 353, .func = variadic_function, .before_fork_func = variadic_function_before_fork, .next_p = 
# 353 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 353 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_variadic_function(void) { nala_register_test(&nala_test_variadic_function); } static void variadic_function(void)
{
    int bar_1;
    int bar_2;

    io_control_mock_once(1, 0, "%d", 5);
    do { __typeof__(io_control(1, 5)) _nala_assert_left = io_control(1, 5); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 359 "test_dummy_functions_tests.c" 3 4
   0
# 359 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 359 "test_dummy_functions_tests.c" 3 4
   0
# 359 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 359 "test_dummy_functions_tests.c" 3 4
   _Bool
# 359 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 359 "test_dummy_functions_tests.c" 3 4
   _Bool
# 359 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 359, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(2, 1, "%u", 5);
    do { __typeof__(io_control(2, 5)) _nala_assert_left = io_control(2, 5); __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 362 "test_dummy_functions_tests.c" 3 4
   0
# 362 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 362 "test_dummy_functions_tests.c" 3 4
   0
# 362 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 362 "test_dummy_functions_tests.c" 3 4
   _Bool
# 362 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 362 "test_dummy_functions_tests.c" 3 4
   _Bool
# 362 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 362, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(2, 1, "%u", 5);
    io_control_mock_ignore_va_arg_in_at(0);
    do { __typeof__(io_control(2, 6)) _nala_assert_left = io_control(2, 6); __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 366 "test_dummy_functions_tests.c" 3 4
   0
# 366 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 366 "test_dummy_functions_tests.c" 3 4
   0
# 366 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 366 "test_dummy_functions_tests.c" 3 4
   _Bool
# 366 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 366 "test_dummy_functions_tests.c" 3 4
   _Bool
# 366 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 366, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(1, 2, "%ld", 6l);
    do { __typeof__(io_control(1, 6l)) _nala_assert_left = io_control(1, 6l); __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 369 "test_dummy_functions_tests.c" 3 4
   0
# 369 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 369 "test_dummy_functions_tests.c" 3 4
   0
# 369 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 369 "test_dummy_functions_tests.c" 3 4
   _Bool
# 369 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 369 "test_dummy_functions_tests.c" 3 4
   _Bool
# 369 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 369, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(2, 3, "%lu", 7ul);
    do { __typeof__(io_control(2, 7lu)) _nala_assert_left = io_control(2, 7lu); __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 372 "test_dummy_functions_tests.c" 3 4
   0
# 372 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 372 "test_dummy_functions_tests.c" 3 4
   0
# 372 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 372 "test_dummy_functions_tests.c" 3 4
   _Bool
# 372 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 372 "test_dummy_functions_tests.c" 3 4
   _Bool
# 372 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 372, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(3, 0, "%p");
    do { __typeof__(io_control(3, 
# 375 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 375 "test_dummy_functions_tests.c"
   )) _nala_assert_left = io_control(3, 
# 375 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 375 "test_dummy_functions_tests.c"
   ); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 375 "test_dummy_functions_tests.c" 3 4
   0
# 375 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 375 "test_dummy_functions_tests.c" 3 4
   0
# 375 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 375 "test_dummy_functions_tests.c" 3 4
   _Bool
# 375 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 375 "test_dummy_functions_tests.c" 3 4
   _Bool
# 375 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 375, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_pointer_at(0, 
# 378 "test_dummy_functions_tests.c" 3 4
                                               ((void *)0)
# 378 "test_dummy_functions_tests.c"
                                                   );
    do { __typeof__(io_control(3, 
# 379 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 379 "test_dummy_functions_tests.c"
   )) _nala_assert_left = io_control(3, 
# 379 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 379 "test_dummy_functions_tests.c"
   ); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 379 "test_dummy_functions_tests.c" 3 4
   0
# 379 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 379 "test_dummy_functions_tests.c" 3 4
   0
# 379 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 379 "test_dummy_functions_tests.c" 3 4
   _Bool
# 379 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 379 "test_dummy_functions_tests.c" 3 4
   _Bool
# 379 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 379, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_pointer_at(0, &bar_1);
    do { __typeof__(io_control(3, &bar_1)) _nala_assert_left = io_control(3, &bar_1); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 383 "test_dummy_functions_tests.c" 3 4
   0
# 383 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 383 "test_dummy_functions_tests.c" 3 4
   0
# 383 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 383 "test_dummy_functions_tests.c" 3 4
   _Bool
# 383 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 383 "test_dummy_functions_tests.c" 3 4
   _Bool
# 383 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 383, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    bar_1 = 4;
    bar_2 = 4;
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_at(0, &bar_1, sizeof(bar_1));
    do { __typeof__(io_control(3, &bar_2)) _nala_assert_left = io_control(3, &bar_2); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 389 "test_dummy_functions_tests.c" 3 4
   0
# 389 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 389 "test_dummy_functions_tests.c" 3 4
   0
# 389 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 389 "test_dummy_functions_tests.c" 3 4
   _Bool
# 389 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 389 "test_dummy_functions_tests.c" 3 4
   _Bool
# 389 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 389, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    bar_1 = 3;
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_out_at(0, &bar_1, sizeof(bar_1));
    bar_1 = 5;
    do { __typeof__(io_control(3, &bar_1)) _nala_assert_left = io_control(3, &bar_1); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 395 "test_dummy_functions_tests.c" 3 4
   0
# 395 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 395 "test_dummy_functions_tests.c" 3 4
   0
# 395 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 395 "test_dummy_functions_tests.c" 3 4
   _Bool
# 395 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 395 "test_dummy_functions_tests.c" 3 4
   _Bool
# 395 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 395, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(bar_1) _nala_assert_left = bar_1; __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 396 "test_dummy_functions_tests.c" 3 4
   0
# 396 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 396 "test_dummy_functions_tests.c" 3 4
   0
# 396 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 396 "test_dummy_functions_tests.c" 3 4
   _Bool
# 396 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 396 "test_dummy_functions_tests.c" 3 4
   _Bool
# 396 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 396, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(4, 1, "%d%u%p", 1, 2);
    do { __typeof__(io_control(4, 1, 2, 
# 399 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 399 "test_dummy_functions_tests.c"
   )) _nala_assert_left = io_control(4, 1, 2, 
# 399 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 399 "test_dummy_functions_tests.c"
   ); __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 399 "test_dummy_functions_tests.c" 3 4
   0
# 399 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 399 "test_dummy_functions_tests.c" 3 4
   0
# 399 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 399 "test_dummy_functions_tests.c" 3 4
   _Bool
# 399 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 399 "test_dummy_functions_tests.c" 3 4
   _Bool
# 399 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 399, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(5, 0, "");
    do { __typeof__(io_control(5)) _nala_assert_left = io_control(5); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 402 "test_dummy_functions_tests.c" 3 4
   0
# 402 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 402 "test_dummy_functions_tests.c" 3 4
   0
# 402 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 402 "test_dummy_functions_tests.c" 3 4
   _Bool
# 402 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 402 "test_dummy_functions_tests.c" 3 4
   _Bool
# 402 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 402, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    io_control_mock_once(1, 2, "%d", 10);
    io_control_mock_set_callback(variadic_function_callback);
    variadic_function_callback_called = 
# 406 "test_dummy_functions_tests.c" 3 4
                                       0
# 406 "test_dummy_functions_tests.c"
                                            ;
    do { __typeof__(io_control(1, 10)) _nala_assert_left = io_control(1, 10); __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 407 "test_dummy_functions_tests.c" 3 4
   0
# 407 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 407 "test_dummy_functions_tests.c" 3 4
   0
# 407 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 407 "test_dummy_functions_tests.c" 3 4
   _Bool
# 407 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 407 "test_dummy_functions_tests.c" 3 4
   _Bool
# 407 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 407, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(variadic_function_callback_called) _nala_assert_left = variadic_function_callback_called; __typeof__(
# 408 "test_dummy_functions_tests.c" 3 4
   1
# 408 "test_dummy_functions_tests.c"
   ) _nala_assert_right = 
# 408 "test_dummy_functions_tests.c" 3 4
   1
# 408 "test_dummy_functions_tests.c"
   ; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 408 "test_dummy_functions_tests.c" 3 4
   0
# 408 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 408 "test_dummy_functions_tests.c" 3 4
   0
# 408 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 408 "test_dummy_functions_tests.c" 3 4
   _Bool
# 408 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 408 "test_dummy_functions_tests.c" 3 4
   _Bool
# 408 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 408, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static int va_list_ellipsis(int kind, ...)
{
    int res;
    va_list ap;

    
# 416 "test_dummy_functions_tests.c" 3 4
   __builtin_va_start(
# 416 "test_dummy_functions_tests.c"
   ap
# 416 "test_dummy_functions_tests.c" 3 4
   ,
# 416 "test_dummy_functions_tests.c"
   kind
# 416 "test_dummy_functions_tests.c" 3 4
   )
# 416 "test_dummy_functions_tests.c"
                     ;
    res = io_vcontrol(kind, ap);
    
# 418 "test_dummy_functions_tests.c" 3 4
   __builtin_va_end(
# 418 "test_dummy_functions_tests.c"
   ap
# 418 "test_dummy_functions_tests.c" 3 4
   )
# 418 "test_dummy_functions_tests.c"
             ;

    return (res);
}

static void va_list_function(void); static void va_list_function_before_fork() {} static struct nala_test_t nala_test_va_list_function = { .name_p = "va_list_function", .file_p = "test_dummy_functions_tests.c", .line = 423, .func = va_list_function, .before_fork_func = va_list_function_before_fork, .next_p = 
# 423 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 423 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_va_list_function(void) { nala_register_test(&nala_test_va_list_function); } static void va_list_function(void)
{
    io_vcontrol_mock_once(1, 3);
    do { __typeof__(va_list_ellipsis(1, 5)) _nala_assert_left = va_list_ellipsis(1, 5); __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 426 "test_dummy_functions_tests.c" 3 4
   0
# 426 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 426 "test_dummy_functions_tests.c" 3 4
   0
# 426 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 426 "test_dummy_functions_tests.c" 3 4
   _Bool
# 426 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 426 "test_dummy_functions_tests.c" 3 4
   _Bool
# 426 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 426, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

int io_control_mock_va_arg_real(int kind, va_list __nala_va_list)
{
    int res;

    switch (kind) {

    case 1:
        res = 
# 436 "test_dummy_functions_tests.c" 3 4
             __builtin_va_arg(
# 436 "test_dummy_functions_tests.c"
             __nala_va_list
# 436 "test_dummy_functions_tests.c" 3 4
             ,
# 436 "test_dummy_functions_tests.c"
             int
# 436 "test_dummy_functions_tests.c" 3 4
             )
# 436 "test_dummy_functions_tests.c"
                                        ;
        res += 100;
        break;

    case 2:
        res = (int)
# 441 "test_dummy_functions_tests.c" 3 4
                  __builtin_va_arg(
# 441 "test_dummy_functions_tests.c"
                  __nala_va_list
# 441 "test_dummy_functions_tests.c" 3 4
                  ,
# 441 "test_dummy_functions_tests.c"
                  unsigned int
# 441 "test_dummy_functions_tests.c" 3 4
                  )
# 441 "test_dummy_functions_tests.c"
                                                      ;
        res += 1000;
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

static void variadic_function_real_1(void); static void variadic_function_real_1_before_fork() {} static struct nala_test_t nala_test_variadic_function_real_1 = { .name_p = "variadic_function_real_1", .file_p = "test_dummy_functions_tests.c", .line = 453, .func = variadic_function_real_1, .before_fork_func = variadic_function_real_1_before_fork, .next_p = 
# 453 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 453 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_variadic_function_real_1(void) { nala_register_test(&nala_test_variadic_function_real_1); } static void variadic_function_real_1(void)
{
    do { __typeof__(io_control(1, 5)) _nala_assert_left = io_control(1, 5); __typeof__(105) _nala_assert_right = 105; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 455 "test_dummy_functions_tests.c" 3 4
   0
# 455 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 455 "test_dummy_functions_tests.c" 3 4
   0
# 455 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 455 "test_dummy_functions_tests.c" 3 4
   _Bool
# 455 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 455 "test_dummy_functions_tests.c" 3 4
   _Bool
# 455 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 455, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void variadic_function_real_2(void); static void variadic_function_real_2_before_fork() {} static struct nala_test_t nala_test_variadic_function_real_2 = { .name_p = "variadic_function_real_2", .file_p = "test_dummy_functions_tests.c", .line = 458, .func = variadic_function_real_2, .before_fork_func = variadic_function_real_2_before_fork, .next_p = 
# 458 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 458 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_variadic_function_real_2(void) { nala_register_test(&nala_test_variadic_function_real_2); } static void variadic_function_real_2(void)
{
    do { __typeof__(io_control(2, 5)) _nala_assert_left = io_control(2, 5); __typeof__(1005) _nala_assert_right = 1005; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 460 "test_dummy_functions_tests.c" 3 4
   0
# 460 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 460 "test_dummy_functions_tests.c" 3 4
   0
# 460 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 460 "test_dummy_functions_tests.c" 3 4
   _Bool
# 460 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 460 "test_dummy_functions_tests.c" 3 4
   _Bool
# 460 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 460, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void variadic_function_error_integer_entry(void *arg_p)
{
    (void)arg_p;

    io_control_mock_once(1, 0, "%d", 5);
    io_control(1, 6);
}

static void variadic_function_error_integer(void); static void variadic_function_error_integer_before_fork() {} static struct nala_test_t nala_test_variadic_function_error_integer = { .name_p = "variadic_function_error_integer", .file_p = "test_dummy_functions_tests.c", .line = 471, .func = variadic_function_error_integer, .before_fork_func = variadic_function_error_integer_before_fork, .next_p = 
# 471 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 471 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_variadic_function_error_integer(void) { nala_register_test(&nala_test_variadic_function_error_integer); } static void variadic_function_error_integer(void)
{
    function_error_in_subprocess(variadic_function_error_integer_entry,
                                 "5 != 6");
}

static void variadic_function_error_va_arg_in_entry(void *arg_p)
{
    (void)arg_p;

    int bar_1;
    int bar_2;

    bar_1 = 3;
    bar_2 = 4;
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_at(0, &bar_1, sizeof(bar_1));
    io_control(3, &bar_2);
}

static void variadic_function_error_va_arg_in(void); static void variadic_function_error_va_arg_in_before_fork() {} static struct nala_test_t nala_test_variadic_function_error_va_arg_in = { .name_p = "variadic_function_error_va_arg_in", .file_p = "test_dummy_functions_tests.c", .line = 491, .func = variadic_function_error_va_arg_in, .before_fork_func = variadic_function_error_va_arg_in_before_fork, .next_p = 
# 491 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 491 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_variadic_function_error_va_arg_in(void) { nala_register_test(&nala_test_variadic_function_error_va_arg_in); } static void variadic_function_error_va_arg_in(void)
{
    function_error_in_subprocess(variadic_function_error_va_arg_in_entry, 
# 493 "test_dummy_functions_tests.c" 3 4
                                                                         ((void *)0)
# 493 "test_dummy_functions_tests.c"
                                                                             );
}

static void compose_twice_function(void); static void compose_twice_function_before_fork() {} static struct nala_test_t nala_test_compose_twice_function = { .name_p = "compose_twice_function", .file_p = "test_dummy_functions_tests.c", .line = 496, .func = compose_twice_function, .before_fork_func = compose_twice_function_before_fork, .next_p = 
# 496 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 496 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_compose_twice_function(void) { nala_register_test(&nala_test_compose_twice_function); } static void compose_twice_function(void)
{
    DummyStruct dummy_struct = { .number = 1 };
    DummyStruct *dummy_ptr = compose_twice(&dummy_struct, add_one);

    do { __typeof__(dummy_struct.number) _nala_assert_left = dummy_struct.number; __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 501 "test_dummy_functions_tests.c" 3 4
   0
# 501 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 501 "test_dummy_functions_tests.c" 3 4
   0
# 501 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 501 "test_dummy_functions_tests.c" 3 4
   _Bool
# 501 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 501 "test_dummy_functions_tests.c" 3 4
   _Bool
# 501 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 501, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    compose_twice_mock_once(&dummy_struct);

    do { __typeof__(compose_twice(dummy_ptr, add_one)) _nala_assert_left = compose_twice(dummy_ptr, add_one); __typeof__(dummy_ptr) _nala_assert_right = dummy_ptr; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 505 "test_dummy_functions_tests.c" 3 4
   0
# 505 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 505 "test_dummy_functions_tests.c" 3 4
   0
# 505 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 505 "test_dummy_functions_tests.c" 3 4
   _Bool
# 505 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 505 "test_dummy_functions_tests.c" 3 4
   _Bool
# 505 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 505, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(dummy_struct.number) _nala_assert_left = dummy_struct.number; __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 506 "test_dummy_functions_tests.c" 3 4
   0
# 506 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 506 "test_dummy_functions_tests.c" 3 4
   0
# 506 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 506 "test_dummy_functions_tests.c" 3 4
   _Bool
# 506 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 506 "test_dummy_functions_tests.c" 3 4
   _Bool
# 506 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 506, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void struct_param_function(void); static void struct_param_function_before_fork() {} static struct nala_test_t nala_test_struct_param_function = { .name_p = "struct_param_function", .file_p = "test_dummy_functions_tests.c", .line = 509, .func = struct_param_function, .before_fork_func = struct_param_function_before_fork, .next_p = 
# 509 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 509 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_struct_param_function(void) { nala_register_test(&nala_test_struct_param_function); } static void struct_param_function(void)
{
    struct_param_mock();
    struct_param(
# 512 "test_dummy_functions_tests.c" 3 4
                ((void *)0)
# 512 "test_dummy_functions_tests.c"
                    );
}

static void enum_param_function(void); static void enum_param_function_before_fork() {} static struct nala_test_t nala_test_enum_param_function = { .name_p = "enum_param_function", .file_p = "test_dummy_functions_tests.c", .line = 515, .func = enum_param_function, .before_fork_func = enum_param_function_before_fork, .next_p = 
# 515 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 515 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_enum_param_function(void) { nala_register_test(&nala_test_enum_param_function); } static void enum_param_function(void)
{
    enum_param_mock(enum_param_type_a);
    enum_param(enum_param_type_a);
}

static void call_function(void); static void call_function_before_fork() {} static struct nala_test_t nala_test_call_function = { .name_p = "call_function", .file_p = "test_dummy_functions_tests.c", .line = 521, .func = call_function, .before_fork_func = call_function_before_fork, .next_p = 
# 521 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 521 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_call_function(void) { nala_register_test(&nala_test_call_function); } static void call_function(void)
{
    call_mock_once(4);
    do { __typeof__(call(
# 524 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 524 "test_dummy_functions_tests.c"
   )) _nala_assert_left = call(
# 524 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 524 "test_dummy_functions_tests.c"
   ); __typeof__(4) _nala_assert_right = 4; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 524 "test_dummy_functions_tests.c" 3 4
   0
# 524 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 524 "test_dummy_functions_tests.c" 3 4
   0
# 524 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 524 "test_dummy_functions_tests.c" 3 4
   _Bool
# 524 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 524 "test_dummy_functions_tests.c" 3 4
   _Bool
# 524 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 524, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void buf_p_assert(int *actual_p, const void *buf_p, size_t size)
{
    const int *expected_p;

    expected_p = buf_p;

    do { __typeof__(actual_p[0]) _nala_assert_left = actual_p[0]; __typeof__(expected_p[0]) _nala_assert_right = expected_p[0]; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 533 "test_dummy_functions_tests.c" 3 4
   0
# 533 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 533 "test_dummy_functions_tests.c" 3 4
   0
# 533 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 533 "test_dummy_functions_tests.c" 3 4
   _Bool
# 533 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 533 "test_dummy_functions_tests.c" 3 4
   _Bool
# 533 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 533, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void in_out_function(void); static void in_out_function_before_fork() {} static struct nala_test_t nala_test_in_out_function = { .name_p = "in_out_function", .file_p = "test_dummy_functions_tests.c", .line = 536, .func = in_out_function, .before_fork_func = in_out_function_before_fork, .next_p = 
# 536 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 536 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_in_out_function(void) { nala_register_test(&nala_test_in_out_function); } static void in_out_function(void)
{

    int values[2];

    in_out_mock_once();
    values[0] = 2;
    values[1] = 0;
    in_out_mock_set_buf_p_in(&values[0], sizeof(values));
    in_out_mock_set_buf_p_in_assert(buf_p_assert);
    values[0] = 2;
    values[1] = 1;
    in_out_mock_set_buf_p_out(&values[0], sizeof(values));
    values[0] = 2;
    values[1] = -1;
    in_out(&values[0]);
    do { __typeof__(values[1]) _nala_assert_left = values[1]; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 552 "test_dummy_functions_tests.c" 3 4
   0
# 552 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 552 "test_dummy_functions_tests.c" 3 4
   0
# 552 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 552 "test_dummy_functions_tests.c" 3 4
   _Bool
# 552 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 552 "test_dummy_functions_tests.c" 3 4
   _Bool
# 552 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 552, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);
}

static void struct_param_and_return_type_function(void); static void struct_param_and_return_type_function_before_fork() {} static struct nala_test_t nala_test_struct_param_and_return_type_function = { .name_p = "struct_param_and_return_type_function", .file_p = "test_dummy_functions_tests.c", .line = 555, .func = struct_param_and_return_type_function, .before_fork_func = struct_param_and_return_type_function_before_fork, .next_p = 
# 555 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 555 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_struct_param_and_return_type_function(void) { nala_register_test(&nala_test_struct_param_and_return_type_function); } static void struct_param_and_return_type_function(void)
{
    struct struct_param_type value;

    value.number = 1;
    value = struct_param_and_return_type(value);
    do { __typeof__(value.number) _nala_assert_left = value.number; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 561 "test_dummy_functions_tests.c" 3 4
   0
# 561 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 561 "test_dummy_functions_tests.c" 3 4
   0
# 561 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 561 "test_dummy_functions_tests.c" 3 4
   _Bool
# 561 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 561 "test_dummy_functions_tests.c" 3 4
   _Bool
# 561 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 561, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    value.number = 1;
    struct_param_and_return_type_mock_once(value);
    value = struct_param_and_return_type(value);
    do { __typeof__(value.number) _nala_assert_left = value.number; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 566 "test_dummy_functions_tests.c" 3 4
   0
# 566 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 566 "test_dummy_functions_tests.c" 3 4
   0
# 566 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 566 "test_dummy_functions_tests.c" 3 4
   _Bool
# 566 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 566 "test_dummy_functions_tests.c" 3 4
   _Bool
# 566 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 566, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void union_param_and_return_type_function(void); static void union_param_and_return_type_function_before_fork() {} static struct nala_test_t nala_test_union_param_and_return_type_function = { .name_p = "union_param_and_return_type_function", .file_p = "test_dummy_functions_tests.c", .line = 569, .func = union_param_and_return_type_function, .before_fork_func = union_param_and_return_type_function_before_fork, .next_p = 
# 569 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 569 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_union_param_and_return_type_function(void) { nala_register_test(&nala_test_union_param_and_return_type_function); } static void union_param_and_return_type_function(void)
{
    union union_type value;

    value.a.number = 1;
    value = union_param_and_return_type(value);
    do { __typeof__(value.a.number) _nala_assert_left = value.a.number; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 575 "test_dummy_functions_tests.c" 3 4
   0
# 575 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 575 "test_dummy_functions_tests.c" 3 4
   0
# 575 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 575 "test_dummy_functions_tests.c" 3 4
   _Bool
# 575 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 575 "test_dummy_functions_tests.c" 3 4
   _Bool
# 575 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 575, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    value.a.number = 1;
    union_param_and_return_type_mock_once(value);
    value = union_param_and_return_type(value);
    do { __typeof__(value.a.number) _nala_assert_left = value.a.number; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 580 "test_dummy_functions_tests.c" 3 4
   0
# 580 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 580 "test_dummy_functions_tests.c" 3 4
   0
# 580 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 580 "test_dummy_functions_tests.c" 3 4
   _Bool
# 580 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 580 "test_dummy_functions_tests.c" 3 4
   _Bool
# 580 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 580, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void typedef_struct_param_and_return_type_function(void); static void typedef_struct_param_and_return_type_function_before_fork() {} static struct nala_test_t nala_test_typedef_struct_param_and_return_type_function = { .name_p = "typedef_struct_param_and_return_type_function", .file_p = "test_dummy_functions_tests.c", .line = 583, .func = typedef_struct_param_and_return_type_function, .before_fork_func = typedef_struct_param_and_return_type_function_before_fork, .next_p = 
# 583 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 583 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_typedef_struct_param_and_return_type_function(void) { nala_register_test(&nala_test_typedef_struct_param_and_return_type_function); } static void typedef_struct_param_and_return_type_function(void)
{
    struct_param_type value;

    value.number = 1;
    value = typedef_struct_param_and_return_type(value);
    do { __typeof__(value.number) _nala_assert_left = value.number; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 589 "test_dummy_functions_tests.c" 3 4
   0
# 589 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 589 "test_dummy_functions_tests.c" 3 4
   0
# 589 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 589 "test_dummy_functions_tests.c" 3 4
   _Bool
# 589 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 589 "test_dummy_functions_tests.c" 3 4
   _Bool
# 589 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 589, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    value.number = 1;
    typedef_struct_param_and_return_type_mock_once(value);
    value = typedef_struct_param_and_return_type(value);
    do { __typeof__(value.number) _nala_assert_left = value.number; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 594 "test_dummy_functions_tests.c" 3 4
   0
# 594 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 594 "test_dummy_functions_tests.c" 3 4
   0
# 594 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 594 "test_dummy_functions_tests.c" 3 4
   _Bool
# 594 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 594 "test_dummy_functions_tests.c" 3 4
   _Bool
# 594 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 594, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void typedef_union_param_and_return_type_function(void); static void typedef_union_param_and_return_type_function_before_fork() {} static struct nala_test_t nala_test_typedef_union_param_and_return_type_function = { .name_p = "typedef_union_param_and_return_type_function", .file_p = "test_dummy_functions_tests.c", .line = 597, .func = typedef_union_param_and_return_type_function, .before_fork_func = typedef_union_param_and_return_type_function_before_fork, .next_p = 
# 597 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 597 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_typedef_union_param_and_return_type_function(void) { nala_register_test(&nala_test_typedef_union_param_and_return_type_function); } static void typedef_union_param_and_return_type_function(void)
{
    union_type value;

    value.a.number = 1;
    value = typedef_union_param_and_return_type(value);
    do { __typeof__(value.a.number) _nala_assert_left = value.a.number; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 603 "test_dummy_functions_tests.c" 3 4
   0
# 603 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 603 "test_dummy_functions_tests.c" 3 4
   0
# 603 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 603 "test_dummy_functions_tests.c" 3 4
   _Bool
# 603 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 603 "test_dummy_functions_tests.c" 3 4
   _Bool
# 603 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 603, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    value.a.number = 1;
    typedef_union_param_and_return_type_mock_once(value);
    value = typedef_union_param_and_return_type(value);
    do { __typeof__(value.a.number) _nala_assert_left = value.a.number; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 608 "test_dummy_functions_tests.c" 3 4
   0
# 608 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 608 "test_dummy_functions_tests.c" 3 4
   0
# 608 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 608 "test_dummy_functions_tests.c" 3 4
   _Bool
# 608 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 608 "test_dummy_functions_tests.c" 3 4
   _Bool
# 608 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 608, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void double_pointer_out_copy(int **value_pp, const void *buf_p, size_t size)
{
    **value_pp = *(const int *)buf_p;
}

static void double_pointer_function(void); static void double_pointer_function_before_fork() {} static struct nala_test_t nala_test_double_pointer_function = { .name_p = "double_pointer_function", .file_p = "test_dummy_functions_tests.c", .line = 616, .func = double_pointer_function, .before_fork_func = double_pointer_function_before_fork, .next_p = 
# 616 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 616 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_double_pointer_function(void) { nala_register_test(&nala_test_double_pointer_function); } static void double_pointer_function(void)
{
    int value;
    int *value_p;

    value = -1;
    value_p = &value;
    do { __typeof__(double_pointer(&value_p)) _nala_assert_left = double_pointer(&value_p); __typeof__(5) _nala_assert_right = 5; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 623 "test_dummy_functions_tests.c" 3 4
   0
# 623 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 623 "test_dummy_functions_tests.c" 3 4
   0
# 623 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 623 "test_dummy_functions_tests.c" 3 4
   _Bool
# 623 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 623 "test_dummy_functions_tests.c" 3 4
   _Bool
# 623 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 623, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(value) _nala_assert_left = value; __typeof__(1) _nala_assert_right = 1; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 624 "test_dummy_functions_tests.c" 3 4
   0
# 624 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 624 "test_dummy_functions_tests.c" 3 4
   0
# 624 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 624 "test_dummy_functions_tests.c" 3 4
   _Bool
# 624 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 624 "test_dummy_functions_tests.c" 3 4
   _Bool
# 624 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 624, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    value = -1;
    value_p = &value;
    double_pointer_mock_once(3);
    value = 2;
    double_pointer_mock_set_value_pp_out(&value, sizeof(value));
    double_pointer_mock_set_value_pp_out_copy(double_pointer_out_copy);
    value = -1;
    do { __typeof__(double_pointer(&value_p)) _nala_assert_left = double_pointer(&value_p); __typeof__(3) _nala_assert_right = 3; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 633 "test_dummy_functions_tests.c" 3 4
   0
# 633 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 633 "test_dummy_functions_tests.c" 3 4
   0
# 633 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 633 "test_dummy_functions_tests.c" 3 4
   _Bool
# 633 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 633 "test_dummy_functions_tests.c" 3 4
   _Bool
# 633 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 633, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(value) _nala_assert_left = value; __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 634 "test_dummy_functions_tests.c" 3 4
   0
# 634 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 634 "test_dummy_functions_tests.c" 3 4
   0
# 634 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 634 "test_dummy_functions_tests.c" 3 4
   _Bool
# 634 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 634 "test_dummy_functions_tests.c" 3 4
   _Bool
# 634 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 634, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void rename_parameters(void); static void rename_parameters_before_fork() {} static struct nala_test_t nala_test_rename_parameters = { .name_p = "rename_parameters", .file_p = "test_dummy_functions_tests.c", .line = 637, .func = rename_parameters, .before_fork_func = rename_parameters_before_fork, .next_p = 
# 637 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 637 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_rename_parameters(void) { nala_register_test(&nala_test_rename_parameters); } static void rename_parameters(void)
{
    close_mock_ignore_in(0);
    endmntent_mock_ignore_in(0);
    fclose_mock(0);
    fopen_mock_ignore_in(
# 642 "test_dummy_functions_tests.c" 3 4
                        ((void *)0)
# 642 "test_dummy_functions_tests.c"
                            );

    fseek_mock_ignore_in(0);
    ftell_mock_ignore_in(0);
    fwrite_mock_ignore_in(0);
    getmntent_mock_ignore_in(
# 647 "test_dummy_functions_tests.c" 3 4
                            ((void *)0)
# 647 "test_dummy_functions_tests.c"
                                );
    mount_mock_ignore_in(0);

    pipe_mock_ignore_in(0);
    poll_mock_ignore_in(0);
    read_mock_ignore_in(0);
    sendto_mock_ignore_in(0);
    setsockopt_mock_ignore_in(0);
    sleep_mock_ignore_in(0);
    statvfs_mock_ignore_in(0);
    time_mock_ignore_in(0);
    timerfd_settime_mock_ignore_in(0);
    usleep_mock_ignore_in(0);
    write_mock_ignore_in(0);
}

static void print_hello_function(void); static void print_hello_function_before_fork() {} static struct nala_test_t nala_test_print_hello_function = { .name_p = "print_hello_function", .file_p = "test_dummy_functions_tests.c", .line = 663, .func = print_hello_function, .before_fork_func = print_hello_function_before_fork, .next_p = 
# 663 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 663 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_print_hello_function(void) { nala_register_test(&nala_test_print_hello_function); } static void print_hello_function(void)
{
    print_hello_mock_once();
    print_hello();
}

static void capture_output_suspend_resume(void); static void capture_output_suspend_resume_before_fork() {} static struct nala_test_t nala_test_capture_output_suspend_resume = { .name_p = "capture_output_suspend_resume", .file_p = "test_dummy_functions_tests.c", .line = 669, .func = capture_output_suspend_resume, .before_fork_func = capture_output_suspend_resume_before_fork, .next_p = 
# 669 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 669 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_capture_output_suspend_resume(void) { nala_register_test(&nala_test_capture_output_suspend_resume); } static void capture_output_suspend_resume(void)
{
    fflush_mock_once(0);
    dup_mock_once(1, 2);
    dup2_mock_once(3, 4, 5);
    tmpfile_mock_once(
# 674 "test_dummy_functions_tests.c" 3 4
                     ((void *)0)
# 674 "test_dummy_functions_tests.c"
                         );
    close_mock_once(1, 9);
    fileno_mock_once(5);
    ftell_mock_once(6);
    fseek_mock_once(3, 1, 5);
    fread_mock_once(7, 6, 5);
    fclose_mock_once(8);

    int outputi; static char *output = 
# 682 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 682 "test_dummy_functions_tests.c"
   ; static char *errput = 
# 682 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 682 "test_dummy_functions_tests.c"
   ; for (outputi = 0, nala_capture_output_start(&output, &errput); outputi < 1; outputi++, nala_capture_output_stop()) {
        printf("Stdout!\n");
        fprintf(
# 684 "test_dummy_functions_tests.c" 3 4
               stderr
# 684 "test_dummy_functions_tests.c"
                     , "Stderr!\n");
    }

    do { __typeof__(output) _nala_assert_left = output; __typeof__("Stdout!\n") _nala_assert_right = "Stdout!\n"; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 687 "test_dummy_functions_tests.c" 3 4
   0
# 687 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 687 "test_dummy_functions_tests.c" 3 4
   0
# 687 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 687 "test_dummy_functions_tests.c" 3 4
   _Bool
# 687 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 687 "test_dummy_functions_tests.c" 3 4
   _Bool
# 687 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 687, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(errput) _nala_assert_left = errput; __typeof__("Stderr!\n") _nala_assert_right = "Stderr!\n"; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 688 "test_dummy_functions_tests.c" 3 4
   0
# 688 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 688 "test_dummy_functions_tests.c" 3 4
   0
# 688 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 688 "test_dummy_functions_tests.c" 3 4
   _Bool
# 688 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 688 "test_dummy_functions_tests.c" 3 4
   _Bool
# 688 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 688, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;

    do { __typeof__(fflush(
# 690 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 690 "test_dummy_functions_tests.c"
   )) _nala_assert_left = fflush(
# 690 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 690 "test_dummy_functions_tests.c"
   ); __typeof__(0) _nala_assert_right = 0; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 690 "test_dummy_functions_tests.c" 3 4
   0
# 690 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 690 "test_dummy_functions_tests.c" 3 4
   0
# 690 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 690 "test_dummy_functions_tests.c" 3 4
   _Bool
# 690 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 690 "test_dummy_functions_tests.c" 3 4
   _Bool
# 690 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 690, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(dup(1)) _nala_assert_left = dup(1); __typeof__(2) _nala_assert_right = 2; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 691 "test_dummy_functions_tests.c" 3 4
   0
# 691 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 691 "test_dummy_functions_tests.c" 3 4
   0
# 691 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 691 "test_dummy_functions_tests.c" 3 4
   _Bool
# 691 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 691 "test_dummy_functions_tests.c" 3 4
   _Bool
# 691 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 691, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(dup2(3, 4)) _nala_assert_left = dup2(3, 4); __typeof__(5) _nala_assert_right = 5; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 692 "test_dummy_functions_tests.c" 3 4
   0
# 692 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 692 "test_dummy_functions_tests.c" 3 4
   0
# 692 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 692 "test_dummy_functions_tests.c" 3 4
   _Bool
# 692 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 692 "test_dummy_functions_tests.c" 3 4
   _Bool
# 692 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 692, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(tmpfile()) _nala_assert_left = tmpfile(); __typeof__(
# 693 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 693 "test_dummy_functions_tests.c"
   ) _nala_assert_right = 
# 693 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 693 "test_dummy_functions_tests.c"
   ; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 693 "test_dummy_functions_tests.c" 3 4
   0
# 693 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 693 "test_dummy_functions_tests.c" 3 4
   0
# 693 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 693 "test_dummy_functions_tests.c" 3 4
   _Bool
# 693 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 693 "test_dummy_functions_tests.c" 3 4
   _Bool
# 693 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 693, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(close(1)) _nala_assert_left = close(1); __typeof__(9) _nala_assert_right = 9; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 694 "test_dummy_functions_tests.c" 3 4
   0
# 694 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 694 "test_dummy_functions_tests.c" 3 4
   0
# 694 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 694 "test_dummy_functions_tests.c" 3 4
   _Bool
# 694 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 694 "test_dummy_functions_tests.c" 3 4
   _Bool
# 694 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 694, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fileno(
# 695 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 695 "test_dummy_functions_tests.c"
   )) _nala_assert_left = fileno(
# 695 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 695 "test_dummy_functions_tests.c"
   ); __typeof__(5) _nala_assert_right = 5; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 695 "test_dummy_functions_tests.c" 3 4
   0
# 695 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 695 "test_dummy_functions_tests.c" 3 4
   0
# 695 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 695 "test_dummy_functions_tests.c" 3 4
   _Bool
# 695 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 695 "test_dummy_functions_tests.c" 3 4
   _Bool
# 695 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 695, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(ftell(
# 696 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 696 "test_dummy_functions_tests.c"
   )) _nala_assert_left = ftell(
# 696 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 696 "test_dummy_functions_tests.c"
   ); __typeof__(6) _nala_assert_right = 6; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 696 "test_dummy_functions_tests.c" 3 4
   0
# 696 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 696 "test_dummy_functions_tests.c" 3 4
   0
# 696 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 696 "test_dummy_functions_tests.c" 3 4
   _Bool
# 696 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 696 "test_dummy_functions_tests.c" 3 4
   _Bool
# 696 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 696, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fseek(
# 697 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 697 "test_dummy_functions_tests.c"
   , 3, 1)) _nala_assert_left = fseek(
# 697 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 697 "test_dummy_functions_tests.c"
   , 3, 1); __typeof__(5) _nala_assert_right = 5; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 697 "test_dummy_functions_tests.c" 3 4
   0
# 697 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 697 "test_dummy_functions_tests.c" 3 4
   0
# 697 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 697 "test_dummy_functions_tests.c" 3 4
   _Bool
# 697 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 697 "test_dummy_functions_tests.c" 3 4
   _Bool
# 697 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 697, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fread(
# 698 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 698 "test_dummy_functions_tests.c"
   , 7, 6, 
# 698 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 698 "test_dummy_functions_tests.c"
   )) _nala_assert_left = fread(
# 698 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 698 "test_dummy_functions_tests.c"
   , 7, 6, 
# 698 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 698 "test_dummy_functions_tests.c"
   ); __typeof__(5u) _nala_assert_right = 5u; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 698 "test_dummy_functions_tests.c" 3 4
   0
# 698 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 698 "test_dummy_functions_tests.c" 3 4
   0
# 698 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 698 "test_dummy_functions_tests.c" 3 4
   _Bool
# 698 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 698 "test_dummy_functions_tests.c" 3 4
   _Bool
# 698 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 698, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    do { __typeof__(fclose(
# 699 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 699 "test_dummy_functions_tests.c"
   )) _nala_assert_left = fclose(
# 699 "test_dummy_functions_tests.c" 3 4
   ((void *)0)
# 699 "test_dummy_functions_tests.c"
   ); __typeof__(8) _nala_assert_right = 8; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 699 "test_dummy_functions_tests.c" 3 4
   0
# 699 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 699 "test_dummy_functions_tests.c" 3 4
   0
# 699 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 699 "test_dummy_functions_tests.c" 3 4
   _Bool
# 699 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 699 "test_dummy_functions_tests.c" 3 4
   _Bool
# 699 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 699, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
}

static void malloc_free(void); static void malloc_free_before_fork() {} static struct nala_test_t nala_test_malloc_free = { .name_p = "malloc_free", .file_p = "test_dummy_functions_tests.c", .line = 702, .func = malloc_free, .before_fork_func = malloc_free_before_fork, .next_p = 
# 702 "test_dummy_functions_tests.c" 3 4
((void *)0) 
# 702 "test_dummy_functions_tests.c"
}; __attribute__ ((constructor)) static void nala_constructor_malloc_free(void) { nala_register_test(&nala_test_malloc_free); } static void malloc_free(void)
{
    int a;
    int *a_p;

    malloc_mock_once(1, &a);
    free_mock_once();
    free_mock_set_ptr_in_pointer(&a);
    a_p = malloc(1);
    do { __typeof__(a_p) _nala_assert_left = a_p; __typeof__(&a) _nala_assert_right = &a; if (!_Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 711 "test_dummy_functions_tests.c" 3 4
   0
# 711 "test_dummy_functions_tests.c"
   ), const char *: _Generic( (_nala_assert_right), char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_check_string_equal( (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: 
# 711 "test_dummy_functions_tests.c" 3 4
   0
# 711 "test_dummy_functions_tests.c"
   ), default: (_nala_assert_left) == (_nala_assert_right))) { nala_reset_all_mocks(); char _nala_assert_format[512]; snprintf(&_nala_assert_format[0], sizeof(_nala_assert_format), "%s != %s\n", _Generic((_nala_assert_left), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 711 "test_dummy_functions_tests.c" 3 4
   _Bool
# 711 "test_dummy_functions_tests.c"
   : "%d", default: "%p"), _Generic((_nala_assert_right), char: "%c", const char: "%c", signed char: "%hhd", const signed char: "%hhd", unsigned char: "%hhu", const unsigned char: "%hhu", signed short: "%hd", const signed short: "%hd", unsigned short: "%hu", const unsigned short: "%hu", signed int: "%d", const signed int: "%d", unsigned int: "%u", const unsigned int: "%u", long int: "%ld", const long int: "%ld", unsigned long int: "%lu", const unsigned long int: "%lu", long long int: "%lld", const long long int: "%lld", unsigned long long int: "%llu", const unsigned long long int: "%llu", float: "%f", const float: "%f", double: "%f", const double: "%f", long double: "%Lf", const long double: "%Lf", char *: "\"%s\"", const char *: "\"%s\"", 
# 711 "test_dummy_functions_tests.c" 3 4
   _Bool
# 711 "test_dummy_functions_tests.c"
   : "%d", default: "%p")); nala_test_failure("test_dummy_functions_tests.c", 711, _Generic( (_nala_assert_left), char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), const char *: _Generic( (_nala_assert_right), char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), const char *: nala_format_string( _nala_assert_format, (char *)(uintptr_t)(_nala_assert_left), (char *)(uintptr_t)(_nala_assert_right)), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right))), default: nala_format(_nala_assert_format, (_nala_assert_left), (_nala_assert_right)))); } } while (0);;
    free(a_p);
}
