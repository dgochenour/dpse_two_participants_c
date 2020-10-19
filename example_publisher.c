// This sample code is intended to show that the Connext DDS Micro 2.4.12
// C API can be called from a C++ application. It is STRICTLY an example and
// NOT intended to represent production-quality code.

#include <stdio.h>

// headers from Connext DDS Micro installation
#include "rti_me_c.h"
#include "disc_dpse/disc_dpse_dpsediscovery.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"
#include "netio/netio_udp.h"

// rtiddsgen generated headers
#include "example.h"
#include "examplePlugin.h"
#include "exampleSupport.h"

// DPSE Discovery-related constants defined in this header
#include "discovery_constants.h"

// forward declarations
int register_rh_wh(RT_Registry_T *registry);
int configure_udp(
        RT_Registry_T *registry, 
        struct UDP_InterfaceFactoryProperty *udp_property,
        char *loopback,
        char *eth_nic); 

int main(void)
{
    // user-configurable values
    char *peer = "127.0.0.1";
    char *loopback_name = "lo";         // Ubuntu 20.04
    char *eth_nic_name = "wlp0s20f3";   // Ubuntu 20.04    
    // char *loopback_name = "Loopback Pseudo-Interface 1";    // Windows 10
    // char *eth_nic_name = "Wireless LAN adapter Wi-Fi";      // Windows 10
    int domain_1_id = 10;
    int domain_2_id = 20;

    DDS_DomainParticipantFactory *dpf = NULL;
    struct DDS_DomainParticipantFactoryQos dpf_qos = 
            DDS_DomainParticipantFactoryQos_INITIALIZER;

    // DDS entities related to domain_1_id
    DDS_DomainParticipant *dp_1 = NULL;
    struct DDS_DomainParticipantQos dp_1_qos = 
            DDS_DomainParticipantQos_INITIALIZER;
    struct DPSE_DiscoveryPluginProperty discovery_plugin_properties_1 =
            DPSE_DiscoveryPluginProperty_INITIALIZER;
    RT_Registry_T *registry = NULL;
    struct UDP_InterfaceFactoryProperty *udp_property_1 = NULL;
    DDS_Topic *topic_1 = NULL;
    DDS_Publisher *publisher_1 = NULL;
    DDS_DataWriter *datawriter_1 = NULL;
    struct DDS_DataWriterQos dw_1_qos = DDS_DataWriterQos_INITIALIZER;
    my_typeDataWriter *narrowed_datawriter_1 = NULL;

    // DDS entities related to domain_2_id
    DDS_DomainParticipant *dp_2 = NULL;
    struct DDS_DomainParticipantQos dp_2_qos = 
            DDS_DomainParticipantQos_INITIALIZER;
    DDS_Topic *topic_2 = NULL;
    DDS_Publisher *publisher_2 = NULL;
    DDS_DataWriter *datawriter_2 = NULL;
    struct DDS_DataWriterQos dw_2_qos = DDS_DataWriterQos_INITIALIZER;
    my_typeDataWriter *narrowed_datawriter_2 = NULL;
    struct DDS_SubscriptionBuiltinTopicData rem_subscription_data =
            DDS_SubscriptionBuiltinTopicData_INITIALIZER;

    my_type *sample = NULL;
    int sample_count = 0;

    DDS_ReturnCode_t retcode;


    // The DomainParticipantFactory is a singleton: only one is required in the
    // application, even if we are creating several DomainParticipants
    dpf = DDS_DomainParticipantFactory_get_instance();

    // Make 2 changes to the DPF QoS:
    // 1) Set resource_limits.max_participants = 2 (the default is 1) since this 
    //    application creates 2 DomainParticipants.
    //
    // 2) By setting autoenable_created_entities to false until all of the DDS 
    //    entities are created, we limit all dynamic memory allocation to happen 
    //    *before* the point where we enable everything.
    DDS_DomainParticipantFactory_get_qos(dpf, &dpf_qos);
    dpf_qos.resource_limits.max_participants = 2;
    dpf_qos.entity_factory.autoenable_created_entities = DDS_BOOLEAN_FALSE;
    DDS_DomainParticipantFactory_set_qos(dpf, &dpf_qos);

    // create registry so that we can make some changes to the default values
    registry = DDS_DomainParticipantFactory_get_registry(dpf);

    // register reader and writer history
    if (register_rh_wh(registry) != DDS_RETCODE_OK) {
        printf("ERROR: DP1 register_rh_wh() failed\n");
    }

    // set up the UDP transport
    if (configure_udp(registry, udp_property_1, loopback_name, eth_nic_name) 
            != DDS_RETCODE_OK) {
        printf("ERROR: DP1 configure_udp() failed\n");        
    } 
    
    // register the dpse (discovery) component
    if (!RT_Registry_register(
            registry,
            "dpse",
            DPSE_DiscoveryFactory_get_interface(),
            &discovery_plugin_properties_1._parent, 
            NULL))
    {
        printf("ERROR: failed to register dpse\n");
    }

    //**************************************************************************
    // DomainParticipant #1 and associated entities
    //**************************************************************************

    // configure discovery prior to creating our DomainParticipant
    if(!RT_ComponentFactoryId_set_name(&dp_1_qos.discovery.discovery.name, "dpse")) {
        printf("ERROR: failed to set discovery plugin name\n");
    }
    if(!DDS_StringSeq_set_maximum(&dp_1_qos.discovery.initial_peers, 1)) {
        printf("ERROR: failed to set initial peers maximum\n");
    }
    if (!DDS_StringSeq_set_length(&dp_1_qos.discovery.initial_peers, 1)) {
        printf("ERROR: failed to set initial peers length\n");
    }
    *DDS_StringSeq_get_reference(&dp_1_qos.discovery.initial_peers, 0) = 
            DDS_String_dup(peer);

    // configure the DomainParticipant's resource limits... these are just 
    // examples, if there are more remote or local endpoints these values would
    // need to be increased
    dp_1_qos.resource_limits.max_destination_ports = 8;
    dp_1_qos.resource_limits.max_receive_ports = 8;
    dp_1_qos.resource_limits.local_topic_allocation = 1;
    dp_1_qos.resource_limits.local_type_allocation = 1;
    dp_1_qos.resource_limits.local_reader_allocation = 1;
    dp_1_qos.resource_limits.local_writer_allocation = 1;
    dp_1_qos.resource_limits.remote_participant_allocation = 3;
    dp_1_qos.resource_limits.remote_reader_allocation = 3;
    dp_1_qos.resource_limits.remote_writer_allocation = 2;

    // set the name of the local DomainParticipant (i.e. - this application) 
    // from the constants defined in discovery_constants.h
    // (this is required for DPSE discovery)
    strcpy(dp_1_qos.participant_name.name, k_PARTICIPANT01_NAME);

    // now the DomainParticipant can be created
    dp_1 = DDS_DomainParticipantFactory_create_participant(
            dpf, 
            domain_1_id,
            &dp_1_qos, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(dp_1 == NULL) {
        printf("ERROR: failed to create participant\n");
    }

    // register the type (my_type, from the idl) with the middleware
    retcode = DDS_DomainParticipant_register_type(
            dp_1,
            my_typeTypePlugin_get_default_type_name(),
            my_typeTypePlugin_get());
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to register type\n");
    }

    // Create the Topic to which we will publish. Note that the name of the 
    // Topic is stored in my_topic_name, which was defined in the IDL 
    topic_1 = DDS_DomainParticipant_create_topic(
            dp_1,
            my_topic_name_1, // this constant is defined in the *.idl file
            my_typeTypePlugin_get_default_type_name(),
            &DDS_TOPIC_QOS_DEFAULT, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(topic_1 == NULL) {
        printf("ERROR: topic == NULL\n");
    }

    // assert the remote DomainParticipants (whos names are defined in 
    // discovery_constants.h) that we are expecting to discover
    retcode = DPSE_RemoteParticipant_assert(dp_1, k_PARTICIPANT03_NAME);
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to assert remote participant 3\n");
    }

    // create the Publisher
    publisher_1 = DDS_DomainParticipant_create_publisher(
            dp_1,
            &DDS_PUBLISHER_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    if(publisher_1 == NULL) {
        printf("ERROR: Publisher == NULL\n");
    }

    // Configure the DataWriter's QoS. Note that the 'rtps_object_id' that we 
    // assign to our own DataWriter here needs to be the same number the remote
    // DataReader will configure for its remote peer. We are defining these IDs
    // and other constants in discovery_constants.h
    dw_1_qos.protocol.rtps_object_id = k_OBJ_ID_PARTICIPANT01_DW01;
    dw_1_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    dw_1_qos.writer_resource_limits.max_remote_readers = 3;
    dw_1_qos.resource_limits.max_samples_per_instance = 16;
    dw_1_qos.resource_limits.max_instances = 2;
    dw_1_qos.resource_limits.max_samples = dw_1_qos.resource_limits.max_instances *
            dw_1_qos.resource_limits.max_samples_per_instance;
    dw_1_qos.history.depth = 16;
    dw_1_qos.protocol.rtps_reliable_writer.heartbeat_period.sec = 0;
    dw_1_qos.protocol.rtps_reliable_writer.heartbeat_period.nanosec = 250000000;

    // now create the DataWriter
    datawriter_1 = DDS_Publisher_create_datawriter(
            publisher_1, 
            topic_1, 
            &dw_1_qos,
            NULL,
            DDS_STATUS_MASK_NONE);
    if(datawriter_1 == NULL) {
        printf("ERROR: datawriter == NULL\n");
    }   
    // A DDS_DataWriter is not type-specific, thus we need to cast, or "narrow"
    // the DataWriter before we use it to write our samples
    narrowed_datawriter_1 = my_typeDataWriter_narrow(datawriter_1);

    // When we use DPSE discovery we must mannually setup information about any 
    // DataReaders we are expecting to discover, and assert them. In this 
    // example code we will do this for 1 remote DataReader per Domain. This 
    // information includes a unique  object ID for the remote peer (we are 
    // defining this in discovery_constants.h), as well as its Topic, type, 
    // and QoS. 

    rem_subscription_data.key.value[DDS_BUILTIN_TOPIC_KEY_OBJECT_ID] = 
            k_OBJ_ID_PARTICIPANT03_DR01;
    rem_subscription_data.topic_name = DDS_String_dup(my_topic_name_1);
    rem_subscription_data.type_name = 
            DDS_String_dup(my_typeTypePlugin_get_default_type_name());
    rem_subscription_data.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;

    // Assert that a remote DomainParticipant (with the name held in
    // k_PARTICIANT03_NAME) will contain a DataReader described by the 
    // information in the rem_subscription_data struct.
    retcode = DPSE_RemoteSubscription_assert(
            dp_1,
            k_PARTICIPANT03_NAME,
            &rem_subscription_data,
            my_type_get_key_kind(my_typeTypePlugin_get(), NULL));
    if (retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to assert remote publication\n");
    }     

    //**************************************************************************
    // DomainParticipant #2 and associated entities
    //**************************************************************************

    // configure discovery prior to creating our DomainParticipant
    if(!RT_ComponentFactoryId_set_name(&dp_2_qos.discovery.discovery.name, "dpse")) {
        printf("ERROR: failed to set discovery plugin name\n");
    }
    if(!DDS_StringSeq_set_maximum(&dp_2_qos.discovery.initial_peers, 1)) {
        printf("ERROR: failed to set initial peers maximum\n");
    }
    if (!DDS_StringSeq_set_length(&dp_2_qos.discovery.initial_peers, 1)) {
        printf("ERROR: failed to set initial peers length\n");
    }
    *DDS_StringSeq_get_reference(&dp_2_qos.discovery.initial_peers, 0) = 
            DDS_String_dup(peer);

    // configure the DomainParticipant's resource limits... these are just 
    // examples, if there are more remote or local endpoints these values would
    // need to be increased
    dp_2_qos.resource_limits.max_destination_ports = 8;
    dp_2_qos.resource_limits.max_receive_ports = 8;
    dp_2_qos.resource_limits.local_topic_allocation = 1;
    dp_2_qos.resource_limits.local_type_allocation = 1;
    dp_2_qos.resource_limits.local_reader_allocation = 1;
    dp_2_qos.resource_limits.local_writer_allocation = 1;
    dp_2_qos.resource_limits.remote_participant_allocation = 3;
    dp_2_qos.resource_limits.remote_reader_allocation = 3;
    dp_2_qos.resource_limits.remote_writer_allocation = 2;

    // set the name of the local DomainParticipant (i.e. - this application) 
    // from the constants defined in discovery_constants.h
    // (this is required for DPSE discovery)
    strcpy(dp_2_qos.participant_name.name, k_PARTICIPANT02_NAME);

    // now the DomainParticipant can be created
    dp_2 = DDS_DomainParticipantFactory_create_participant(
            dpf, 
            domain_2_id,
            &dp_2_qos, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(dp_2 == NULL) {
        printf("ERROR: failed to create participant\n");
    }

    // register the type (my_type, from the idl) with the middleware
    retcode = DDS_DomainParticipant_register_type(
            dp_2,
            my_typeTypePlugin_get_default_type_name(),
            my_typeTypePlugin_get());
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to register type\n");
    }

    // Create the Topic to which we will publish. Note that the name of the 
    // Topic is stored in my_topic_name, which was defined in the IDL 
    topic_2 = DDS_DomainParticipant_create_topic(
            dp_2,
            my_topic_name_2, // this constant is defined in the *.idl file
            my_typeTypePlugin_get_default_type_name(),
            &DDS_TOPIC_QOS_DEFAULT, 
            NULL,
            DDS_STATUS_MASK_NONE);
    if(topic_2 == NULL) {
        printf("ERROR: topic == NULL\n");
    }

    // assert the remote DomainParticipants (whos names are defined in 
    // discovery_constants.h) that we are expecting to discover
    retcode = DPSE_RemoteParticipant_assert(dp_2, k_PARTICIPANT04_NAME);
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to assert remote participant 4\n");
    }

    // create the Publisher
    publisher_2 = DDS_DomainParticipant_create_publisher(
            dp_2,
            &DDS_PUBLISHER_QOS_DEFAULT,
            NULL,
            DDS_STATUS_MASK_NONE);
    if(publisher_2 == NULL) {
        printf("ERROR: Publisher == NULL\n");
    }

    // Configure the DataWriter's QoS. Note that the 'rtps_object_id' that we 
    // assign to our own DataWriter here needs to be the same number the remote
    // DataReader will configure for its remote peer. We are defining these IDs
    // and other constants in discovery_constants.h
    dw_2_qos.protocol.rtps_object_id = k_OBJ_ID_PARTICIPANT02_DW01;
    dw_2_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    dw_2_qos.writer_resource_limits.max_remote_readers = 3;
    dw_2_qos.resource_limits.max_samples_per_instance = 16;
    dw_2_qos.resource_limits.max_instances = 2;
    dw_2_qos.resource_limits.max_samples = dw_2_qos.resource_limits.max_instances *
            dw_2_qos.resource_limits.max_samples_per_instance;
    dw_2_qos.history.depth = 16;
    dw_2_qos.protocol.rtps_reliable_writer.heartbeat_period.sec = 0;
    dw_2_qos.protocol.rtps_reliable_writer.heartbeat_period.nanosec = 250000000;

    // now create the DataWriter
    datawriter_2 = DDS_Publisher_create_datawriter(
            publisher_2, 
            topic_2, 
            &dw_2_qos,
            NULL,
            DDS_STATUS_MASK_NONE);
    if(datawriter_2 == NULL) {
        printf("ERROR: datawriter == NULL\n");
    }   
    // A DDS_DataWriter is not type-specific, thus we need to cast, or "narrow"
    // the DataWriter before we use it to write our samples
    narrowed_datawriter_2 = my_typeDataWriter_narrow(datawriter_2);

    // When we use DPSE discovery we must mannually setup information about any 
    // DataReaders we are expecting to discover, and assert them. In this 
    // example code we will do this for 1 remote DataReader per Domain. This 
    // information includes a unique  object ID for the remote peer (we are 
    // defining this in discovery_constants.h), as well as its Topic, type, 
    // and QoS. 

    rem_subscription_data.key.value[DDS_BUILTIN_TOPIC_KEY_OBJECT_ID] = 
            k_OBJ_ID_PARTICIPANT04_DR01;
    rem_subscription_data.topic_name = DDS_String_dup(my_topic_name_2);
    rem_subscription_data.type_name = 
            DDS_String_dup(my_typeTypePlugin_get_default_type_name());
    rem_subscription_data.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;

    // Assert that a remote DomainParticipant (with the name held in
    // k_PARTICIANT03_NAME) will contain a DataReader described by the 
    // information in the rem_subscription_data struct.
    retcode = DPSE_RemoteSubscription_assert(
            dp_2,
            k_PARTICIPANT04_NAME,
            &rem_subscription_data,
            my_type_get_key_kind(my_typeTypePlugin_get(), NULL));
    if (retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to assert remote publication\n");
    }     

    //**************************************************************************
    // END: DomainParticipants and associated entities
    //**************************************************************************

    // create the data sample that we will write
    sample = my_type_create();
    if(sample == NULL) {
        printf("ERROR: failed my_type_create\n");
    }

    // Finaly, now that all of the entities are created, we can enable them all
    retcode = DDS_Entity_enable(DDS_DomainParticipant_as_entity(dp_1));
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to enable entity\n");
    }
    retcode = DDS_Entity_enable(DDS_DomainParticipant_as_entity(dp_2));
    if(retcode != DDS_RETCODE_OK) {
        printf("ERROR: failed to enable entity\n");
    }

    while (1) {
        
        // add some data to the sample
        sample->id = 1; // arbitrary value
        sprintf(sample->msg, "sample #%d\n", sample_count);

        retcode = my_typeDataWriter_write(
                narrowed_datawriter_1, 
                sample, 
                &DDS_HANDLE_NIL);
        if(retcode != DDS_RETCODE_OK) {
            printf("ERROR: Failed to write sample\n");
        } else {
            printf("INFO: narrowed_datawriter_1 wrote sample %d\n", sample_count); 
        } 
        retcode = my_typeDataWriter_write(
                narrowed_datawriter_2, 
                sample, 
                &DDS_HANDLE_NIL);
        if(retcode != DDS_RETCODE_OK) {
            printf("ERROR: Failed to write sample\n");
        } else {
            printf("INFO: narrowed_datawriter_2 wrote sample %d\n", sample_count); 
        } 
        sample_count++;
        OSAPI_Thread_sleep(1000); // sleep 1s between writes 
    }
}

int register_rh_wh(RT_Registry_T *registry) {
    // register writer history
    if (!RT_Registry_register(
            registry, 
            DDSHST_WRITER_DEFAULT_HISTORY_NAME,
            WHSM_HistoryFactory_get_interface(), 
            NULL, 
            NULL))
    {
        printf("ERROR: failed to register wh\n");
        return DDS_RETCODE_ERROR;
    }
    // register reader history
    if (!RT_Registry_register(
            registry, 
            DDSHST_READER_DEFAULT_HISTORY_NAME,
            RHSM_HistoryFactory_get_interface(), 
            NULL, 
            NULL))
    {
        printf("ERROR: failed to register rh\n");
        return DDS_RETCODE_ERROR;
    }
    return DDS_RETCODE_OK;
}

int configure_udp(
        RT_Registry_T *registry, 
        struct UDP_InterfaceFactoryProperty *udp_property,
        char *loopback,
        char *eth_nic) 
{

    // Set up the UDP transport's allowed interfaces. To do this we:
    // (1) unregister the UDP trasport
    // (2) name the allowed interfaces
    // (3) re-register the transport

    if(!RT_Registry_unregister(
            registry, 
            NETIO_DEFAULT_UDP_NAME, 
            NULL, 
            NULL)) 
    {
        printf("ERROR: failed to unregister udp\n");
        return DDS_RETCODE_ERROR;
    }

    udp_property = (struct UDP_InterfaceFactoryProperty *)
            malloc(sizeof(struct UDP_InterfaceFactoryProperty));
    if (udp_property == NULL) {
        printf("ERROR: failed to allocate udp properties\n");
        return DDS_RETCODE_ERROR;
    }
    *udp_property = UDP_INTERFACE_FACTORY_PROPERTY_DEFAULT;

    // For additional allowed interface(s), increase maximum and length, and
    // set interface below:
    REDA_StringSeq_set_maximum(&udp_property->allow_interface,2);
    REDA_StringSeq_set_length(&udp_property->allow_interface,2);
    *REDA_StringSeq_get_reference(&udp_property->allow_interface,0) = 
            DDS_String_dup(loopback); 
    *REDA_StringSeq_get_reference(&udp_property->allow_interface,1) = 
            DDS_String_dup(eth_nic); 

#if 0  

    // When you are working on an RTOS or other lightweight OS, the middleware
    // may not be able to get the NIC information automatically. In that case, 
    // the code below can be used to manually tell the middleware about an 
    // interface. The interface name ("en0" below) could be anything, but should
    // match what you included in the "allow_interface" calls above.

    if (!UDP_InterfaceTable_add_entry(
    		&udp_property->if_table,
            0xc0a864c8,	// IP address of 192.168.100.200
			0xffffff00, // netmask of 255.255.255.0
			"en0",
			UDP_INTERFACE_INTERFACE_UP_FLAG |
			UDP_INTERFACE_INTERFACE_MULTICAST_FLAG)) {

    	LOG(1, "failed to add interface")

    }

#endif

    if(!RT_Registry_register(
            registry, 
            NETIO_DEFAULT_UDP_NAME,
            UDP_InterfaceFactory_get_interface(),
            (struct RT_ComponentFactoryProperty*)udp_property, NULL))
    {
        printf("ERROR: failed to re-register udp\n");
        return DDS_RETCODE_ERROR;
    } 

    return DDS_RETCODE_OK;
}