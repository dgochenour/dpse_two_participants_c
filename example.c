/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from example.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "example.h"

#ifndef UNUSED_ARG
#define UNUSED_ARG(x) (void)(x)
#endif

/*** SOURCE_BEGIN ***/

/* ========================================================================= */

const char *my_typeTYPENAME = "my_type";

RTI_BOOL
my_type_initialize(my_type* sample)
{
    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    CDR_Primitive_init_long(&sample->id);
    if (!CDR_String_initialize(&sample->msg, (128)))
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}

my_type *
my_type_create(void)
{
    my_type* sample;
    OSAPI_Heap_allocate_struct(&sample, my_type);
    if (sample != NULL) {
        if (!my_type_initialize(sample)) {
            OSAPI_Heap_free_struct(sample);
            sample = NULL;
        }
    }
    return sample;
}

#ifndef RTI_CERT

RTI_BOOL
my_type_finalize(my_type* sample)
{
    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    CDR_String_finalize(&sample->msg);
    return RTI_TRUE;
}

void
my_type_delete(my_type*sample)
{
    if (sample != NULL) {
        /* my_type_finalize() always 
        returns RTI_TRUE when called with sample != NULL */
        my_type_finalize(sample);
        OSAPI_Heap_free_struct(sample);
    }
}
#endif

RTI_BOOL
my_type_copy(my_type* dst,const my_type* src)
{        
    if ((dst == NULL) || (src == NULL))
    {
        return RTI_FALSE;
    }
    CDR_Primitive_copy_long(&dst->id, &src->id);
    if (!CDR_String_copy(&dst->msg, &src->msg, (128)))
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'my_type' sequence class.
*/
#define REDA_SEQUENCE_USER_API
#define T my_type
#define TSeq my_typeSeq
#define T_initialize my_type_initialize
#define T_finalize   my_type_finalize
#define T_copy       my_type_copy
#include "reda/reda_sequence_defn.h"
#undef T_copy
#undef T_finalize
#undef T_initialize

