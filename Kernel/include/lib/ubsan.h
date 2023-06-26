#pragma once
#include <lib/types.h>

static const char *UBSAN_TYPECHECK_KINDS[] = 
{
    "load of",
    "store to",
    "reference binding to",
    "member access within",
    "member call on",
    "constructor call on",
    "downcast of",
    "downcast of",
    "upcast of",
    "cast to virtual base of",
};

typedef struct 
{
    const char* file;
    uint32_t    line;
    uint32_t    column;
} ubsan_srcloc_t;

typedef struct 
{
    uint16_t kind;
    uint16_t info;
    char     name[1];
} ubsan_typedesc_t;

typedef struct ubsan_typemismatch_t 
{
    ubsan_srcloc_t    location;
    ubsan_typedesc_t* type;
    uintptr_t         alignment;
    uint8_t           type_check;
} ubsan_typemismatch_t;

typedef struct 
{
    ubsan_srcloc_t    location;
    ubsan_typedesc_t* array_type;
    ubsan_typedesc_t* index_type;
} ubsan_outofbounds_t;

typedef struct
{
    ubsan_srcloc_t    location;
    ubsan_typedesc_t* type;
} ubsan_overflow_t;

typedef struct
{
    ubsan_srcloc_t location;
} ubsan_unreachabledata_t;

typedef struct
{
    ubsan_srcloc_t  location;
    ubsan_typedesc_t* type;
} ubsan_invalidvalue_t;

typedef struct
{
    ubsan_srcloc_t    location;
    ubsan_typedesc_t* left_type;
    ubsan_typedesc_t* right_type;
} ubsan_shift_outofbounds_t;

typedef struct
{
    ubsan_srcloc_t    location;
    ubsan_typedesc_t* type;
} ubsan_vla_bound_data_t;

void __ubsan_handle_out_of_bounds(ubsan_outofbounds_t* info, uintptr_t pointer);
void __ubsan_handle_type_mismatch(ubsan_typemismatch_t* info, uintptr_t pointer);

void __ubsan_handle_add_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs);
void __ubsan_handle_sub_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs);
void __ubsan_handle_mul_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs);
void __ubsan_handle_negate_overflow(ubsan_overflow_t* info, uint32_t old_val);
void __ubsan_handle_divrem_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs);

void __ubsan_handle_missing_return(ubsan_unreachabledata_t* info);
void __ubsan_handle_load_invalid_value(ubsan_invalidvalue_t* info, uint32_t val);
void __ubsan_handle_shift_out_of_bounds(ubsan_shift_outofbounds_t* info, uint32_t lhs, uint32_t rhs);

void __ubsan_handle_vla_bound_not_positive(ubsan_vla_bound_data_t* info, uint32_t bound);
