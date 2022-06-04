/*
 * math.h
 *
 *  Created on: 28 aug. 2020
 *      Author: Ludo
 */

#ifndef __MATH_H__
#define __MATH_H__

#include "mode.h"

/*** MATH macros ***/

#define MATH_BINARY_MAX_LENGTH			32
#define MATH_DECIMAL_MAX_LENGTH			10
#define MATH_HEXADECIMAL_MAX_LENGTH		4
#define MATH_BYTE_MAX					0xFF

#if (defined CM || defined ATM)
static const signed short MATH_COS_TABLE[360] = {
	1000, 1000, 999, 999, 998, 996, 995, 993, 990, 988,
	985, 982, 978, 974, 970, 966, 961, 956, 951, 946,
	940, 934, 927, 921, 914, 906, 899, 891, 883, 875,
	866, 857, 848, 839, 829, 819, 809, 799, 788, 777,
	766, 755, 743, 731, 719, 707, 695, 682, 669, 656,
	643, 629, 616, 602, 588, 574, 559, 545, 530, 515,
	500, 485, 469, 454, 438, 423, 407, 391, 375, 358,
	342, 326, 309, 292, 276, 259, 242, 225, 208, 191,
	174, 156, 139, 122, 105, 87, 70, 52, 35, 17,
	0, -17, -35, -52, -70, -87, -105, -122, -139, -156,
	-174, -191, -208, -225, -242, -259, -276, -292, -309, -326,
	-342, -358, -375, -391, -407, -423, -438, -454, -469, -485,
	-500, -515, -530, -545, -559, -574, -588, -602, -616, -629,
	-643, -656, -669, -682, -695, -707, -719, -731, -743, -755,
	-766, -777, -788, -799, -809, -819, -829, -839, -848, -857,
	-866, -875, -883, -891, -899, -906, -914, -921, -927, -934,
	-940, -946, -951, -956, -961, -966, -970, -974, -978, -982,
	-985, -988, -990, -993, -995, -996, -998, -999, -999, -1000,
	-1000, -1000, -999, -999, -998, -996, -995, -993, -990, -988,
	-985, -982, -978, -974, -970, -966, -961, -956, -951, -946,
	-940, -934, -927, -921, -914, -906, -899, -891, -883, -875,
	-866, -857, -848, -839, -829, -819, -809, -799, -788, -777,
	-766, -755, -743, -731, -719, -707, -695, -682, -669, -656,
	-643, -629, -616, -602, -588, -574, -559, -545, -530, -515,
	-500, -485, -469, -454, -438, -423, -407, -391, -375, -358,
	-342, -326, -309, -292, -276, -259, -242, -225, -208, -191,
	-174, -156, -139, -122, -105, -87, -70, -52, -35, -17,
	0, 17, 35, 52, 70, 87, 105, 122, 139, 156,
	174, 191, 208, 225, 242, 259, 276, 292, 309, 326,
	342, 358, 375, 391, 407, 423, 438, 454, 469, 485,
	500, 515, 530, 545, 559, 574, 588, 602, 616, 629,
	643, 656, 669, 682, 695, 707, 719, 731, 743, 755,
	766, 777, 788, 799, 809, 819, 829, 839, 848, 857,
	866, 875, 883, 891, 899, 906, 914, 921, 927, 934,
	940, 946, 951, 956, 961, 966, 970, 974, 978, 982,
	985, 988, 990, 993, 995, 996, 998, 999, 999, 1000
};

static const signed short MATH_SIN_TABLE[360] = {
	0, 17, 35, 52, 70, 87, 105, 122, 139, 156,
	174, 191, 208, 225, 242, 259, 276, 292, 309, 326,
	342, 358, 375, 391, 407, 423, 438, 454, 469, 485,
	500, 515, 530, 545, 559, 574, 588, 602, 616, 629,
	643, 656, 669, 682, 695, 707, 719, 731, 743, 755,
	766, 777, 788, 799, 809, 819, 829, 839, 848, 857,
	866, 875, 883, 891, 899, 906, 914, 921, 927, 934,
	940, 946, 951, 956, 961, 966, 970, 974, 978, 982,
	985, 988, 990, 993, 995, 996, 998, 999, 999, 1000,
	1000, 1000, 999, 999, 998, 996, 995, 993, 990, 988,
	985, 982, 978, 974, 970, 966, 961, 956, 951, 946,
	940, 934, 927, 921, 914, 906, 899, 891, 883, 875,
	866, 857, 848, 839, 829, 819, 809, 799, 788, 777,
	766, 755, 743, 731, 719, 707, 695, 682, 669, 656,
	643, 629, 616, 602, 588, 574, 559, 545, 530, 515,
	500, 485, 469, 454, 438, 423, 407, 391, 375, 358,
	342, 326, 309, 292, 276, 259, 242, 225, 208, 191,
	174, 156, 139, 122, 105, 87, 70, 52, 35, 17,
	0, -17, -35, -52, -70, -87, -105, -122, -139, -156,
	-174, -191, -208, -225, -242, -259, -276, -292, -309, -326,
	-342, -358, -375, -391, -407, -423, -438, -454, -469, -485,
	-500, -515, -530, -545, -559, -574, -588, -602, -616, -629,
	-643, -656, -669, -682, -695, -707, -719, -731, -743, -755,
	-766, -777, -788, -799, -809, -819, -829, -839, -848, -857,
	-866, -875, -883, -891, -899, -906, -914, -921, -927, -934,
	-940, -946, -951, -956, -961, -966, -970, -974, -978, -982,
	-985, -988, -990, -993, -995, -996, -998, -999, -999, -1000,
	-1000, -1000, -999, -999, -998, -996, -995, -993, -990, -988,
	-985, -982, -978, -974, -970, -966, -961, -956, -951, -946,
	-940, -934, -927, -921, -914, -906, -899, -891, -883, -875,
	-866, -857, -848, -839, -829, -819, -809, -799, -788, -777,
	-766, -755, -743, -731, -719, -707, -695, -682, -669, -656,
	-643, -629, -616, -602, -588, -574, -559, -545, -530, -515,
	-500, -485, -469, -454, -438, -423, -407, -391, -375, -358,
	-342, -326, -309, -292, -276, -259, -242, -225, -208, -191,
	-174, -156, -139, -122, -105, -87, -70, -52, -35, -17
};
#endif

/*** MATH structures ***/

typedef enum {
	MATH_SUCCESS = 0,
	MATH_ERROR_OVERFLOW,
	MATH_ERROR_UNDEFINED,
	MATH_ERROR_SIGN_BIT,
	MATH_ERROR_BASE_LAST = 0x0100
} MATH_status_t;

/*** MATH functions ***/

unsigned char MATH_min_u8(unsigned char* data, unsigned char data_length);
unsigned short MATH_min_u16(unsigned short* data, unsigned char data_length);
unsigned int MATH_min_u32(unsigned int* data, unsigned char data_length);

unsigned char MATH_max_u8(unsigned char* data, unsigned char data_length);
unsigned short MATH_max_u16(unsigned short* data, unsigned char data_length);
unsigned int MATH_max_u32(unsigned int* data, unsigned char data_length);

unsigned char MATH_average_u8(unsigned char* data, unsigned char data_length);
unsigned short MATH_average_u16(unsigned short* data, unsigned char data_length);
unsigned int MATH_average_u32(unsigned int* data, unsigned char data_length);

unsigned char MATH_median_filter_u8(unsigned char* data, unsigned char median_length, unsigned char average_length);
unsigned short MATH_median_filter_u16(unsigned short* data, unsigned char median_length, unsigned char average_length);
unsigned int MATH_median_filter_u32(unsigned int* data, unsigned char median_length, unsigned char average_length);

MATH_status_t MATH_pow_10(unsigned char power, unsigned int* result);
unsigned int MATH_abs(signed int x);
MATH_status_t MATH_atan2(signed int x, signed int y, unsigned int* alpha);

MATH_status_t MATH_two_complement(unsigned int value, unsigned char sign_bit_position, signed int* result);
MATH_status_t MATH_one_complement(signed int value, unsigned char sign_bit_position, unsigned int* result);

#define MATH_status_check(error_base) { if (math_status != MATH_SUCCESS) { status = error_base + math_status; goto errors; }}
#define MATH_error_check() { ERROR_status_check(math_status, MATH_SUCCESS, ERROR_BASE_MATH); }
#define MATH_error_check_print() { ERROR_status_check_print(math_status, MATH_SUCCESS, ERROR_BASE_MATH); }

#endif /* __MATH_H__ */
