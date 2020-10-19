/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from example.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#ifndef example_1944007787_h
#define example_1944007787_h

#ifndef rti_me_c_h
#include "rti_me_c.h"
#endif

#if (defined(RTI_WIN32) || defined(RTI_WIN64) || defined(RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#define my_topic_name_1 ("my_topic_1")

#define my_topic_name_2 ("my_topic_2")

typedef struct my_type

{

    CDR_Long id;
    CDR_String msg;

} my_type ;

NDDSUSERDllExport extern const char *my_typeTYPENAME;

#define REDA_SEQUENCE_USER_API
#define T my_type
#define TSeq my_typeSeq
#define REDA_SEQUENCE_EXCLUDE_C_METHODS
#define REDA_SEQUENCE_USER_CPP
#include <reda/reda_sequence_decl.h>

#ifdef __cplusplus
extern "C" {
    #endif

    #define REDA_SEQUENCE_USER_API
    #define T my_type
    #define TSeq my_typeSeq
    #define REDA_SEQUENCE_EXCLUDE_STRUCT
    #define REDA_SEQUENCE_USER_CPP
    #include <reda/reda_sequence_decl.h>

    NDDSUSERDllExport extern RTI_BOOL
    my_type_initialize(my_type* sample);

    NDDSUSERDllExport extern my_type*
    my_type_create(void);

    #ifndef RTI_CERT
    NDDSUSERDllExport extern RTI_BOOL
    my_type_finalize(my_type* sample);

    NDDSUSERDllExport extern void
    my_type_delete(my_type* sample);
    #endif

    NDDSUSERDllExport extern RTI_BOOL
    my_type_copy(my_type* dst, const my_type* src);
    #ifdef __cplusplus
}
#endif

#if (defined(RTI_WIN32) || defined(RTI_WIN64) || defined(RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* example */

