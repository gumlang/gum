#ifndef GUM_H_
#define GUM_H_
#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#	define GUM_EXPORT __declspec(dllexport)
#	define GUM_IMPORT __declspec(dllimport)
#else
#	define GUM_EXPORT
#	define GUM_IMPORT
#endif

#ifdef GUM_BUILD
#	define GUM_API GUM_EXPORT
#elif defined(__cplusplus)
#	define GUM_API extern "C" GUM_IMPORT
#else
#	define GUM_API GUM_IMPORT
#endif

typedef int32_t gum_int_t;
typedef float gum_float_t;

typedef struct gum_t gum_t;

typedef size_t (*gum_stream_t)(char* buffer, size_t size);

GUM_API void gum_run_stream(gum_stream_t stream);
GUM_API void gum_run_buffer(char* buffer, size_t size);
GUM_API void gum_run_file(FILE* file);

#endif