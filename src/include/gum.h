#ifndef GUM_H_
#define GUM_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
#	define GUM_EXTERN extern "C"
#else
#	define GUM_EXTERN extern
#endif

#ifdef _WIN32
#	define GUM__EXPORT __declspec(dllexport)
#	define GUM__IMPORT __declspec(dllimport)
#else
#	define GUM__EXPORT
#	define GUM__IMPORT
#endif

#define GUM_EXPORT GUM_EXTERN GUM__EXPORT
#define GUM_IMPORT GUM_EXTERN GUM__IMPORT

#ifdef GUM_BUILD
#	define GUM_API GUM_EXPORT
#else
#	define GUM_API GUM_IMPORT
#endif

typedef _Bool gum_bool_t;
typedef int32_t gum_int_t;
typedef float gum_float_t;

typedef struct gum_t gum_t;

typedef size_t (*gum_stream_t)(char* buffer, size_t size);

GUM_API void gum_run_stream(gum_stream_t stream);
GUM_API void gum_run_buffer(char* buffer, size_t size);
GUM_API void gum_run_file(FILE* file);

#endif