/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from example.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "exampleSupport.h"

/*** SOURCE_BEGIN ***/

/* =========================================================================== */

/* Requires */
#define TTYPENAME   my_typeTYPENAME

/* 
my_typeDataWriter (DDS_DataWriter)   
*/

/* Defines */
#define TDataWriter my_typeDataWriter
#define TData       my_type

#include "dds_c/dds_c_tdatawriter_gen.h"

#undef TDataWriter
#undef TData

/* =========================================================================== */
/* 
my_typeDataReader (DDS_DataReader)   
*/

/* Defines */
#define TDataReader my_typeDataReader
#define TDataSeq    my_typeSeq
#define TData       my_type
#include "dds_c/dds_c_tdatareader_gen.h"
#undef TDataReader
#undef TDataSeq
#undef TData

DDS_ReturnCode_t
my_typeTypeSupport_register_type(
    DDS_DomainParticipant* participant,
    const char* type_name)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if (participant == NULL) 
    {
        goto done;
    }

    if (type_name == NULL) 
    {
        type_name = my_typeTypePlugin_get_default_type_name();
        if (type_name == NULL)
        {
            goto done;
        }
    }

    retcode = DDS_DomainParticipant_register_type(
        participant,
        type_name,
        my_typeTypePlugin_get());

    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:

    return retcode;
}

#ifndef RTI_CERT
DDS_ReturnCode_t
my_typeTypeSupport_unregister_type(
    DDS_DomainParticipant* participant,
    const char* type_name)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if (participant == NULL) 
    {
        goto done;
    }

    if (type_name == NULL) 
    {
        type_name = my_typeTypePlugin_get_default_type_name();
        if (type_name == NULL)
        {
            goto done;
        }
    }

    if (my_typeTypePlugin_get() !=
    DDS_DomainParticipant_unregister_type(participant,type_name))
    {
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:

    return retcode;
}
#endif
const char*
my_typeTypeSupport_get_type_name(void)
{
    return my_typeTYPENAME;
}
my_type *
my_typeTypeSupport_create_data(void)
{
    my_type *data = NULL;

    data = my_type_create();

    return data;
}

#ifndef RTI_CERT
void
my_typeTypeSupport_delete_data(
    my_type *data)
{
    my_type_delete(data);
}
#endif

#undef TTYPENAME

