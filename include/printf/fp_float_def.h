#ifndef FLOAT_DEF_H
#define FLOAT_DEF_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Contains several defines needed to parse floating point numbers
 */

/**
 * Enum that defines possible "speciality tags"
 */
typedef enum {
	FPS_NORMAL = 0,		/**< Number is normalized, non-special */
	FPS_DENORMALIZED,	/**< Number is denormalized, non-special */
	FPS_ZERO,		/**< Number is zero */
	FPS_INF,		/**< Number is infinity special */
	FPS_NAN			/**< Number is Not-a-number (NaN) special */
} fp_specialval_t;

/**
 * Macro to check for "really special" cases of "speciality tag"
 */
#define FP_IS_SPECIAL(x) ((x) > FPS_ZERO)

/*
 * NOTE: LITTLE-ENDIAN DEFINITIONS!
 *
 * Special float values:
 * exponent = 0, fraction = 0: zero
 * exponent = 0, fraction != 0: denormalized value (without implicit 1)
 * exponent = MAX, fraction = 0: infinity
 * exponent = MAX, fraction != 0: not-a-number
 */

#define MANTLEN_F 23 /**< Trimmed mantissa length of float */
#define MANTLEN_D 52 /**< Trimmed mantissa length of double */

#define EXPLEN_F 8  /**< Exponent length of float */
#define EXPLEN_D 11 /**< Exponent length of double */

#define EXPMID_F ((1U<<(EXPLEN_F-1)) - 1) /**< Exponent bias of float */
#define EXPMID_D ((1U<<(EXPLEN_D-1)) - 1) /**< Exponent bias of double */

#define EXPMAX_F ((1U<<EXPLEN_F) - 1) /**< Maximal exponent value of float */
#define EXPMAX_D ((1U<<EXPLEN_D) - 1) /**< Maximal exponent value of double */

/**
 * @brief Union to divide float type into components
 */
union fp_float_t {
	float val;
	struct {
		uint32_t fraction:MANTLEN_F;
		unsigned exponent:EXPLEN_F;
		unsigned sign:1;
	} p;
};

/**
 * @brief Union to divide double type into components
 */
union fp_double_t {
	double val;
	struct {
		uint64_t fraction:MANTLEN_D;
		unsigned exponent:EXPLEN_D;
		unsigned sign:1;
	} p;
};

#endif // FLOAT_DEF_H
