/*
 * TransitClock
 * Copyright (C) 2023-2024 Taylor Berg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRANSIT_CLOCK_PORTABLE_H
#define TRANSIT_CLOCK_PORTABLE_H

#include <stdint.h>

/**
 * An 8-bit signed integer.
 */
typedef int8_t int8;

/**
 * A 16-bit signed integer.
 */
typedef int16_t int16;

/**
 * A 32-bit signed integer.
 */
typedef int32_t int32;

/**
 * A 64-bit signed integer.
 */
typedef int64_t int64;

/**
 * An 8-bit unsigned integer.
 */
typedef uint8_t uint8;

/**
 * A 16-bit unsigned integer.
 */
typedef uint16_t uint16;

/**
 * A 32-bit unsigned integer.
 */
typedef uint32_t uint32;

/**
 * A 64-bit unsigned integer.
 */
typedef uint64_t uint64;

#endif //TRANSIT_CLOCK_PORTABLE_H
