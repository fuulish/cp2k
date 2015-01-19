/******************************************************************************
** Copyright (c) 2014-2015, Intel Corporation                                **
** All rights reserved.                                                      **
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions        **
** are met:                                                                  **
** 1. Redistributions of source code must retain the above copyright         **
**    notice, this list of conditions and the following disclaimer.          **
** 2. Redistributions in binary form must reproduce the above copyright      **
**    notice, this list of conditions and the following disclaimer in the    **
**    documentation and/or other materials provided with the distribution.   **
** 3. Neither the name of the copyright holder nor the names of its          **
**    contributors may be used to endorse or promote products derived        **
**    from this software without specific prior written permission.          **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  **
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
******************************************************************************/
/* Hans Pabst (Intel Corp.)
******************************************************************************/
#ifndef LIBXSTREAM_MACROS_H
#define LIBXSTREAM_MACROS_H

#include "libxstream_config.h"

#if (1600 < _MSC_VER) || !defined(_WIN32)
# if !defined(LIBXSTREAM_MIC_STDTHREAD)
#   define LIBXSTREAM_MIC_STDTHREAD
# endif
# if !defined(_GLIBCXX_HAS_GTHREADS)
#   define _GLIBCXX_HAS_GTHREADS
# endif
# if !defined(_GLIBCXX_USE_C99_STDINT_TR1)
#   define _GLIBCXX_USE_C99_STDINT_TR1
# endif
# if !defined(_GLIBCXX_USE_SCHED_YIELD)
#   define _GLIBCXX_USE_SCHED_YIELD
# endif
#endif

#if defined(_WIN32) && !defined(__GNUC__)
# define LIBXSTREAM_ATTRIBUTE(A) __declspec(A)
# define LIBXSTREAM_ALIGNED(DECL, N) LIBXSTREAM_ATTRIBUTE(align(N)) DECL
#elif defined(__GNUC__)
# define LIBXSTREAM_ATTRIBUTE(A) __attribute__((A))
# define LIBXSTREAM_ALIGNED(DECL, N) DECL LIBXSTREAM_ATTRIBUTE(aligned(N))
#endif

#if defined(_OPENMP) // so far we do not use TLS without OpenMP
# if defined(_WIN32) && !defined(__GNUC__)
#   define LIBXSTREAM_TLS LIBXSTREAM_ATTRIBUTE(thread)
# elif defined(__GNUC__)
#   define LIBXSTREAM_TLS __thread
# endif
#endif
#if !defined(LIBXSTREAM_TLS)
# define LIBXSTREAM_TLS
#endif

#define LIBXSTREAM_TOSTRING_AUX(SYMBOL) #SYMBOL
#define LIBXSTREAM_TOSTRING(SYMBOL) LIBXSTREAM_TOSTRING_AUX(SYMBOL)

#if defined(__INTEL_COMPILER)
# define LIBXSTREAM_ASSUME_ALIGNED(A, N) __assume_aligned(A, N)
# define LIBXSTREAM_PRAGMA(DIRECTIVE) __pragma(DIRECTIVE)
#elif (199901L <= __STDC_VERSION__)
# define LIBXSTREAM_ASSUME_ALIGNED(A, N)
# define LIBXSTREAM_PRAGMA(DIRECTIVE) _Pragma(LIBXSTREAM_STRINGIFY(DIRECTIVE))
#elif defined(_MSC_VER)
# define LIBXSTREAM_ASSUME_ALIGNED(A, N)
# define LIBXSTREAM_PRAGMA(DIRECTIVE) __pragma(DIRECTIVE)
#else
# define LIBXSTREAM_ASSUME_ALIGNED(A, N)
# define LIBXSTREAM_PRAGMA(DIRECTIVE)
#endif

#if defined(__INTEL_OFFLOAD) && (!defined(_WIN32) || (1400 <= __INTEL_COMPILER))
# define LIBXSTREAM_OFFLOAD
# define LIBXSTREAM_TARGET(A) LIBXSTREAM_ATTRIBUTE(target(A))
#else
# define LIBXSTREAM_TARGET(A)
#endif

#if defined(__cplusplus)
# define LIBXSTREAM_EXTERN_C extern "C"
#else
# define LIBXSTREAM_EXTERN_C
#endif // __cplusplus

#define LIBXSTREAM_EXPORT LIBXSTREAM_TARGET(mic)

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__CYGWIN32__)
# define LIBXSTREAM_RESTRICT __restrict__
#elif defined(_MSC_VER)
# define LIBXSTREAM_RESTRICT __restrict
#else
# define LIBXSTREAM_RESTRICT
#endif

#if (defined(LIBXSTREAM_ERROR_DEBUG) || defined(_DEBUG)) && !defined(NDEBUG) && !defined(LIBXSTREAM_DEBUG)
# define LIBXSTREAM_DEBUG
#endif

#if defined(LIBXSTREAM_ERROR_CHECK) && !defined(LIBXSTREAM_CHECK)
# define LIBXSTREAM_CHECK
#endif

#if defined(LIBXSTREAM_DEBUG)
# define LIBXSTREAM_ASSERT(A) assert(A)
#else
# define LIBXSTREAM_ASSERT(A)
#endif

#define LIBXSTREAM_ERROR_NONE       0
#define LIBXSTREAM_ERROR_RUNTIME   -1
#define LIBXSTREAM_ERROR_CONDITION -2

#if defined(_MSC_VER)
# define LIBXSTREAM_SNPRINTF(S, N, F, ...) _snprintf_s(S, N, _TRUNCATE, F, __VA_ARGS__)
#else
# define LIBXSTREAM_SNPRINTF(S, N, F, ...) snprintf(S, N, F, __VA_ARGS__)
#endif

#define LIBXSTREAM_MIN(A, B) ((A) < (B) ? (A) : (B))
#define LIBXSTREAM_MAX(A, B) ((A) < (B) ? (B) : (A))

#if defined(LIBXSTREAM_CHECK)
# define LIBXSTREAM_CHECK_ERROR(RETURN_VALUE) if (LIBXSTREAM_ERROR_NONE != (RETURN_VALUE)) return RETURN_VALUE;
# define LIBXSTREAM_CHECK_CONDITION(CONDITION) if (!(CONDITION)) return LIBXSTREAM_ERROR_CONDITION;
# ifdef __cplusplus
#   define LIBXSTREAM_CHECK_CALL_THROW(CONDITION) do { int result = (CONDITION); if (LIBXSTREAM_ERROR_NONE != result) throw std::runtime_error(LIBXSTREAM_TOSTRING(CONDITION)); } while(0)
# else
#   define LIBXSTREAM_CHECK_CALL_THROW(CONDITION) do { int result = (CONDITION); if (LIBXSTREAM_ERROR_NONE != result) abort(result); } while(0)
# endif
# if defined(_OPENMP)
#   define LIBXSTREAM_CHECK_CALL(CONDITION) LIBXSTREAM_CHECK_CALL_THROW(CONDITION)
# else
#   define LIBXSTREAM_CHECK_CALL(CONDITION) do { int result = (CONDITION); if (LIBXSTREAM_ERROR_NONE != result) return result; } while(0)
# endif
#else
# define LIBXSTREAM_CHECK_ERROR(RETURN_VALUE) LIBXSTREAM_ASSERT(LIBXSTREAM_ERROR_NONE == (RETURN_VALUE));
# define LIBXSTREAM_CHECK_CONDITION(CONDITION) LIBXSTREAM_ASSERT(CONDITION);
# define LIBXSTREAM_CHECK_CALL_THROW(CONDITION) CONDITION
# define LIBXSTREAM_CHECK_CALL(CONDITION) CONDITION
#endif

#if defined(LIBXSTREAM_OFFLOAD) && defined(LIBXSTREAM_ASYNC) && (0 != (2*LIBXSTREAM_ASYNC+1)/2)
# if (1 == (2*LIBXSTREAM_ASYNC+1)/2) // asynchronous offload
#   define LIBXSTREAM_OFFLOAD_DECL \
      int device_ = stream_ ? stream_->device() : val<int,0>(); \
      libxstream_signal signal_ = stream_ ? stream_->signal() : 0; \
      const libxstream_signal signal_consumed_ = signal_; \
      const libxstream_signal pending_ = stream_ ? stream_->pending() : 0
#   define LIBXSTREAM_OFFLOAD_TARGET target(mic:device_)
#   define LIBXSTREAM_OFFLOAD_TARGET_SIGNAL LIBXSTREAM_OFFLOAD_TARGET signal(signal_++)
#   define LIBXSTREAM_OFFLOAD_TARGET_WAIT LIBXSTREAM_OFFLOAD_TARGET_SIGNAL wait(pending_)
# elif (2 == (2*LIBXSTREAM_ASYNC+1)/2) // compiler streams
#   define LIBXSTREAM_OFFLOAD_DECL \
      int device_ = stream_ ? stream_->device() : val<int,0>(); \
      libxstream_signal signal_ = stream_ ? stream_->signal() : 0; \
      const libxstream_signal signal_consumed_ = signal_; \
      const libxstream_signal pending_ = stream_ ? stream_->pending() : 0; \
      const _Offload_stream handle_ = stream_ ? stream_->handle() : 0
#   define LIBXSTREAM_OFFLOAD_TARGET target(mic:device_) stream(handle_)
#   define LIBXSTREAM_OFFLOAD_TARGET_SIGNAL LIBXSTREAM_OFFLOAD_TARGET signal(signal_++)
#   define LIBXSTREAM_OFFLOAD_TARGET_WAIT LIBXSTREAM_OFFLOAD_TARGET_SIGNAL
# endif
#else // synchronous offload
# define LIBXSTREAM_OFFLOAD_DECL \
    int device_ = stream_ ? stream_->device() : val<int,0>(); \
    const libxstream_signal signal_ = 0; \
    const libxstream_signal signal_consumed_ = 0; \
    const libxstream_signal pending_ = 0
# define LIBXSTREAM_OFFLOAD_TARGET target(mic:device_)
# define LIBXSTREAM_OFFLOAD_TARGET_SIGNAL LIBXSTREAM_OFFLOAD_TARGET
# define LIBXSTREAM_OFFLOAD_TARGET_WAIT LIBXSTREAM_OFFLOAD_TARGET_SIGNAL
#endif

#define LIBXSTREAM_OFFLOAD_READY (0 == (LIBXSTREAM_OFFLOAD_PENDING))
#define LIBXSTREAM_OFFLOAD_PENDING (pending_)
#define LIBXSTREAM_OFFLOAD_STREAM (stream_)
#define LIBXSTREAM_OFFLOAD_DEVICE (device_)
#define LIBXSTREAM_OFFLOAD_DEVICE_UPDATE(DEVICE) device_ = (DEVICE)

#define LIBXSTREAM_OFFLOAD_BEGIN(STREAM, ARG, ...) { \
  libxstream_stream *const stream_ = cast_to_stream(STREAM); \
  const libxstream_offload_region::arg_type args_[] = { ARG, __VA_ARGS__ }; \
  struct offload_region: public libxstream_offload_region { libxstream_stream* stream_; \
    offload_region(libxstream_stream* stream, const arg_type args[], size_t nargs) \
      : libxstream_offload_region(args, nargs), stream_(stream) {} \
    offload_region* clone() const { return new offload_region(*this); } \
    void operator()() const { LIBXSTREAM_OFFLOAD_DECL; do
#define LIBXSTREAM_OFFLOAD_END(WAIT) while(false); \
      if (stream_ && signal_ != signal_consumed_) stream_->pending(signal_consumed_); \
    } \
  } offload_region_(stream_, args_, sizeof(args_) / sizeof(*args_)); \
  libxstream_offload(offload_region_, true == (WAIT)); }

#endif // LIBXSTREAM_MACROS_H