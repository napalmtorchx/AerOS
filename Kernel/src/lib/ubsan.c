#include <lib/ubsan.h>
#include <kernel.h>

void __ubsan_handle_out_of_bounds(ubsan_outofbounds_t* info, uintptr_t pointer)
{
    debug_log("%s Out of Bounds Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_type_mismatch(ubsan_typemismatch_t* info, uintptr_t pointer)
{
    if (pointer == 0) { debug_log("%s Type Mismatch Exception\n", DEBUG_ERROR); debug_log("Null pointer access\n"); }
    else if (info->alignment != 0 && IS_ALIGNED(pointer, info->alignment)) { debug_log("%s Type Mismatch Exception\n", DEBUG_ERROR); debug_log("Unaligned memory access\n"); }   
    else { return; } 
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_add_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    debug_log("%s Add Overflow Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_sub_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    debug_log("%s Sub Overflow Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_mul_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    debug_log("%s Mul Overflow Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_negate_overflow(ubsan_overflow_t* info, uint32_t old_val)
{
    debug_log("%s Negate Overflow Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_divrem_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    debug_log("%s Divrem Overflow Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_missing_return(ubsan_unreachabledata_t* info)
{
    debug_log("%s Missing Return Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_load_invalid_value(ubsan_invalidvalue_t* info, uint32_t val)
{
    debug_log("%s Invalid Value Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_shift_out_of_bounds(ubsan_shift_outofbounds_t* info, uint32_t lhs, uint32_t rhs)
{
    debug_log("%s Shift Out of Bounds Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_vla_bound_not_positive(ubsan_vla_bound_data_t* info, uint32_t bound)
{
    debug_log("%s VLA Bound Not Positive Exception\n", DEBUG_ERROR);
    debug_error("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}
