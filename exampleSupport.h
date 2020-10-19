/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from example.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#ifndef exampleSupport_1944007787_h
#define exampleSupport_1944007787_h

#include <stdlib.h>

/* Uses */
#include "example.h"
/* Requires */
#include "examplePlugin.h"

/* ========================================================================== */
/**
Uses:     T
Defines:  TTypeSupport, TDataWriter, TDataReader*/

#if (defined(RTI_WIN32) || defined(RTI_WIN64) || defined(RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
    #endif

    NDDSUSERDllExport extern DDS_ReturnCode_t
    my_typeTypeSupport_register_type(
        DDS_DomainParticipant* participant,
        const char* type_name);

    #ifndef RTI_CERT
    NDDSUSERDllExport extern DDS_ReturnCode_t
    my_typeTypeSupport_unregister_type(
        DDS_DomainParticipant* participant,
        const char* type_name);
    #endif

    NDDSUSERDllExport extern const char*
    my_typeTypeSupport_get_type_name(void);

    NDDSUSERDllExport extern my_type *
    my_typeTypeSupport_create_data(void);

    #ifndef RTI_CERT
    NDDSUSERDllExport extern void
    my_typeTypeSupport_delete_data(
        my_type *data);
    #endif

    DDS_DATAWRITER_C(my_typeDataWriter, my_type);

    DDS_DATAREADER_C(my_typeDataReader, my_typeSeq, my_type);

    #ifdef __cplusplus
}
#endif

#if (defined(RTI_WIN32) || defined(RTI_WIN64) || defined(RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif  /* exampleSupport_1944007787_h */

