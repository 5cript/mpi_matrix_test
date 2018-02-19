#pragma once

#if __cplusplus == 201703L
#   define MAYBE_UNUSED [[maybe_unused]]
#elif defined(__GNUC__)
#   define MAYBE_UNUSED __attribute__((unused))
#else
#   define MAYBE_UNUSED
#endif

#ifndef NOT_LOGGED_IN
    MAYBE_UNUSED static const char* default_chunk_dump = "/bigwork/nhmqebbe/results";
    MAYBE_UNUSED static const char* matrix_default_location = "/bigwork/nhmqebbe/data";
#else
    MAYBE_UNUSED static const char* default_chunk_dump = "data/results";
    MAYBE_UNUSED static const char* matrix_default_location = "data";
#endif
