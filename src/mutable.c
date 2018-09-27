/*
 *  Copyright (C) 2018   The R Core Team
 *  Distributed under GPL 2 or 3
 */

#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Altrep.h>

/**
 ** Mutable Vectors
 **/

/*
 * Mutable Vector Classes and Objects
 */

static R_altrep_class_t mutable_integer_class;
static R_altrep_class_t mutable_real_class;

/* Mutable objects are ALTREP objects with data fields

       data1: the data vector
       data2: not used
*/

static SEXP make_mutable(SEXP data)
{
    R_altrep_class_t class;
    int type = TYPEOF(data);

    switch(type) {
    case INTSXP:
	class = mutable_integer_class;
	break;
    case REALSXP:
	class = mutable_real_class;
	break;
    default:
	error("mutable vectors for %s not supported yet", type2char(type));
    }

    if (MAYBE_REFERENCED(data))
	data = duplicate(data);
    PROTECT(data);
    SEXP ans = R_new_altrep(class, data, R_NilValue);
    UNPROTECT(1); /* data */
    return ans;
}

#define MUTABLE_DATA(x) R_altrep_data1(x)


/*
 * ALTREP Methods
 */

Rboolean mutable_Inspect(SEXP x, int pre, int deep, int pvec,
			 void (*inspect_subtree)(SEXP, int, int, int))
{
    Rprintf(" mutable %s\n", type2char(TYPEOF(x)));
    inspect_subtree(MUTABLE_DATA(x), pre, deep, pvec);
    return TRUE;
}

static R_xlen_t mutable_Length(SEXP x)
{
    return XLENGTH(MUTABLE_DATA(x));
}


/*
 * ALTVEC Methods
 */

static void *mutable_Dataptr(SEXP x, Rboolean writeable)
{
    if (writeable)
	return DATAPTR(MUTABLE_DATA(x));
    else
	return (void *) DATAPTR_RO(MUTABLE_DATA(x));
}

static const void *mutable_Dataptr_or_null(SEXP x)
{
    return DATAPTR_OR_NULL(MUTABLE_DATA(x));
}


/*
 * ALTINTEGER Methods
 */

static int mutable_integer_Elt(SEXP x, R_xlen_t i)
{
    return INTEGER(MUTABLE_DATA(x))[i];
}

static
R_xlen_t mutable_integer_Get_region(SEXP x, R_xlen_t i, R_xlen_t n, int *buf)
{
    return INTEGER_GET_REGION(MUTABLE_DATA(x), i, n, buf);
}


/*
 * ALTREAL Methods
 */

static double mutable_real_Elt(SEXP x, R_xlen_t i)
{
    return REAL(MUTABLE_DATA(x))[i];
}

static
R_xlen_t mutable_real_Get_region(SEXP x, R_xlen_t i, R_xlen_t n, double *buf)
{
    return REAL_GET_REGION(MUTABLE_DATA(x), i, n, buf);
}


/*
 * Class Objects and Method Tables
 */

#define MUTPKG "mutable"

static void InitMutableIntegerClass(DllInfo *dll)
{
    R_altrep_class_t cls =
	R_make_altinteger_class("mutable_integer", MUTPKG, dll);
    mutable_integer_class = cls;
 
    /* override ALTREP methods */
    R_set_altrep_Inspect_method(cls, mutable_Inspect);
    R_set_altrep_Length_method(cls, mutable_Length);

    /* override ALTVEC methods */
    R_set_altvec_Dataptr_method(cls, mutable_Dataptr);
    R_set_altvec_Dataptr_or_null_method(cls, mutable_Dataptr_or_null);

    /* override ALTINTEGER methods */
    R_set_altinteger_Elt_method(cls, mutable_integer_Elt);
    R_set_altinteger_Get_region_method(cls, mutable_integer_Get_region);
}

static void InitMutableRealClass(DllInfo *dll)
{
    R_altrep_class_t cls =
	R_make_altreal_class("mutable_real", MUTPKG, dll);
    mutable_real_class = cls;

    /* override ALTREP methods */
    R_set_altrep_Inspect_method(cls, mutable_Inspect);
    R_set_altrep_Length_method(cls, mutable_Length);

    /* override ALTVEC methods */
    R_set_altvec_Dataptr_method(cls, mutable_Dataptr);
    R_set_altvec_Dataptr_or_null_method(cls, mutable_Dataptr_or_null);

    /* override ALTREAL methods */
    R_set_altreal_Elt_method(cls, mutable_real_Elt);
    R_set_altreal_Get_region_method(cls, mutable_real_Get_region);
}


/*
 * Constructor
 */

SEXP do_make_mutable(SEXP x)
{
    return make_mutable(x);
}

SEXP do_is_mutable(SEXP x)
{
    switch(TYPEOF(x)) {
    case INTSXP:
	return ScalarLogical(R_altrep_inherits(x, mutable_integer_class));
    case REALSXP:
	return ScalarLogical(R_altrep_inherits(x, mutable_real_class));
    default:
	return ScalarLogical(FALSE);
    }
}


/*
 * Shared Library Initialization
 */

static const R_CallMethodDef CallEntries[] = {
    {"make_mutable", (DL_FUNC) &do_make_mutable, 1},
    {"is_mutable", (DL_FUNC) &do_is_mutable, 1},
    {NULL, NULL, 0}
};

void R_init_mutable(DllInfo *dll)
{
    InitMutableIntegerClass(dll);
    InitMutableRealClass(dll);

    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
}
