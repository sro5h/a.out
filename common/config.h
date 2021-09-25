#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

// TODO: Make portable
#if   defined(COMMON_EXPORTS)
#       define COMMON_API __attribute__((visibility("default")))
#elif defined(COMMON_IMPORTS)
#       define COMMON_API __attribute__((visibility("default")))
#else
#       define COMMON_API
#endif

#endif
