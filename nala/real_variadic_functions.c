// NALA_REAL_VARIADIC_FUNCTION_BEGIN open

extern int __real_open(const char *pathname, int flags, ...);

__attribute__ ((weak))
int open_mock_va_arg_real(const char *pathname, int flags, va_list __nala_va_list)
{
    return (__real_open(pathname, flags, va_arg(__nala_va_list, mode_t)));
}

// NALA_REAL_VARIADIC_FUNCTION_END
