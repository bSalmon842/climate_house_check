/*
Project: Climate Impact
File: house_check_utility.h
Author: Brock Salmon
Notice: (C) Copyright 2021 by Brock Salmon. All Rights Reserved
*/

#ifndef HOUSE_CHECK_UTILITY_H

#include <math.h>

#define function static
#define persist static
#define global static

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef __int8 s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;

typedef u64 usize;
typedef s64 ssize;

typedef s32 b32;
typedef bool b8;

typedef float f32;
typedef double f64;

#if CLIMATE_SLOW
#define ASSERT(check) if(!(check)) {*(s32 *)0 = 0;}
#define INVALID_CODE_PATH ASSERT(false)
#else
#define ASSERT(check) 
#define INVALID_CODE_PATH 
#endif

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define MAX(a, b) ((a > b) ? a : b)

inline s32 StringLength(char *str)
{
    s32 result = 0;
    while (*str++)
	{
		++result;
	}
    return result;
}

inline f32 Abs(f32 value)
{
    f32 result = (value < 0) ? -value : value;
    return result;
}

#define HOUSE_CHECK_UTILITY_H
#endif //HOUSE_CHECK_UTILITY_H
