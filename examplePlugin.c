/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from example.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "example.h"
#include "examplePlugin.h"

/*** SOURCE_BEGIN ***/
#ifndef UNUSED_ARG
#define UNUSED_ARG(x) (void)(x)
#endif

/* --------------------------------------------------------------------------
(De)Serialize functions:
* -------------------------------------------------------------------------- */
RTI_BOOL 
my_type_cdr_serialize(
    struct CDR_Stream_t *stream, const void *void_sample, void *param)
{
    my_type *sample = (my_type *)void_sample;

    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);

    if (!CDR_Stream_serialize_long(
        stream, &sample->id)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_serialize_string(
        stream, sample->msg, (128))) {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL 
my_type_cdr_deserialize(
    struct CDR_Stream_t *stream, void *void_sample, void *param)
{
    my_type *sample = (my_type *)void_sample;

    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);

    if (!CDR_Stream_deserialize_long(
        stream, &sample->id)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_deserialize_string(
        stream, sample->msg, (128))) {
        return RTI_FALSE;
    }

    return RTI_TRUE;

}

RTI_UINT32
my_type_get_serialized_sample_max_size(
    struct NDDS_Type_Plugin *plugin,
    RTI_UINT32 current_alignment,
    void *param)
{
    RTI_UINT32 initial_alignment = current_alignment;

    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    current_alignment += CDR_get_max_size_serialized_long(
        current_alignment);

    current_alignment += CDR_get_max_size_serialized_string(
        current_alignment, (128)+1);

    return  current_alignment - initial_alignment;
}
/* --------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------- */

RTI_BOOL
my_type_cdr_serialize_key(
    struct CDR_Stream_t *stream, const void *void_sample, void *param)
{
    const my_type *sample = (my_type *)void_sample;
    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);
    if (!CDR_Stream_serialize_long(
        stream, &sample->id)) {
        return RTI_FALSE;
    }  

    return RTI_TRUE;
}

RTI_BOOL
my_type_cdr_deserialize_key(
    struct CDR_Stream_t *stream, void *void_sample, void *param)
{
    my_type *sample = (my_type *)void_sample;
    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);
    if (!CDR_Stream_deserialize_long(
        stream, &sample->id)) {
        return RTI_FALSE;
    }  

    return RTI_TRUE;
}

RTI_UINT32 
my_type_get_serialized_key_max_size(
    struct NDDS_Type_Plugin *plugin,
    RTI_UINT32 current_alignment,
    void *param)
{
    RTI_UINT32 initial_alignment = current_alignment;

    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    current_alignment +=  CDR_get_max_size_serialized_long(
        current_alignment );

    return current_alignment - initial_alignment;
}

/* --------------------------------------------------------------------------
*  Sample Support functions:
* -------------------------------------------------------------------------- */
RTI_BOOL
my_typePlugin_create_sample(
    struct NDDS_Type_Plugin *plugin, void **sample, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    *sample = (void *) my_type_create();
    return (sample != NULL);
}

#ifndef RTI_CERT
RTI_BOOL
my_typePlugin_delete_sample(
    struct NDDS_Type_Plugin *plugin, void *sample, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    /* my_type_delete() is a void function
    which expects (sample != NULL). Since my_typePlugin_delete_sample
    is an internal function, sample is assumed to be a valid pointer */ 
    my_type_delete((my_type *) sample);
    return RTI_TRUE;
}
#endif

RTI_BOOL 
my_typePlugin_copy_sample(
    struct NDDS_Type_Plugin *plugin, void *dst, const void *src, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    return my_type_copy(
        (my_type *)dst,
        (const my_type *)src);
}
/* --------------------------------------------------------------------------
*  Type my_type Plugin Instantiation
* -------------------------------------------------------------------------- */

NDDSCDREncapsulation my_typeEncapsulationKind[] =
{ {0,0} };

struct NDDS_Type_Plugin my_typeTypePlugin =
{
    {0, 0},                     /* NDDS_Type_PluginVersion */
    NULL,                       /* DDS_TypeCode_t* */
    my_typeEncapsulationKind,
    NDDS_TYPEPLUGIN_USER_KEY,   /* NDDS_TypePluginKeyKind */
    my_type_cdr_serialize,
    my_type_cdr_deserialize,
    my_type_get_serialized_sample_max_size,
    my_type_cdr_serialize_key,
    my_type_cdr_deserialize_key,
    my_type_get_serialized_key_max_size,
    my_typePlugin_create_sample,
    #ifndef RTI_CERT
    my_typePlugin_delete_sample,
    #else
    NULL,
    #endif
    my_typePlugin_copy_sample,
    PluginHelper_get_key_kind,
    PluginHelper_instance_to_keyhash,
    NULL, NULL, NULL, NULL  /* endpoint wrappers not used in C */
};

/* --------------------------------------------------------------------------
*  Type my_type Plugin Methods
* -------------------------------------------------------------------------- */

struct NDDS_Type_Plugin *
my_typeTypePlugin_get(void) 
{ 
    return &my_typeTypePlugin;
} 

const char*
my_typeTypePlugin_get_default_type_name(void) 
{ 
    return my_typeTYPENAME;
} 

NDDS_TypePluginKeyKind 
my_type_get_key_kind(
    struct NDDS_Type_Plugin *plugin,
    void *param)
{
    UNUSED_ARG(param);
    UNUSED_ARG(plugin);
    return NDDS_TYPEPLUGIN_USER_KEY;
}

