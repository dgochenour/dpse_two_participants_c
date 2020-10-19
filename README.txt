################################################################################
#
# RTI Connext DDS Micro 2.4.12
#
# EXAMPLE: Using DPSE and the C API in a Win64 environment
#
################################################################################

This example contains a publishing application and two subscribing applications 
for a simple IDL-defined data type. "example_subscriber_1" uses a 
WaitSet to be notified of events and "example_subscriber_2" simply polls for data.

The purpose of this example is to demonstrate how static endpoint discovery 
(DPSE) can be used between a publisher and subscriber. Additionally, the use of 
multiple DomainParticipants is illustrated in example_publisher: this application
publishes to two topics, each in a different Domain. The subscribing applications
each participant in a differnt Domain.

Note that actual safety-certified libraries generated from the RTI Connext Micro
Cert product only implement the static DPSE discovery plugin, so DPDE is not 
an option. 


Source Overview
===============

A simple "example" type, containing a message string, is defined in 
example.idl.

For the type to be useable by Connext Micro, type-support files must be 
generated that implement a type-plugin interface.  rtiddsgen can be invoked 
manually, with an example command like this:

On Ubuntu 20.04
-----------------
    $ $RTIMEHOME/rtiddsgen/scripts/rtiddsgen -micro -language C -create typefiles ./example.idl


On Windows 
-----------------
    > %RTIMEHOME%\rtiddsgen\scripts\rtiddsgen -micro -language C -create typefiles example.idl

The generated source files are example.c, exampleSupport.c, and 
examplePlugin.c. Associated header files are also generated.
 
The DataWriter and DataReaders for this type are created and used in 
example_publisher.c and example_subscriber_[1|2].c, respectively. Each 
subscribing app has its own DomainParticipant since the intent is that the 
executables may run independently of each other. The publishing app creates two
DomainParticipants since it communicates with each subscribing app on a different
DDS Domain.


Generated Files Overview
========================

example_publisher.c:
This file contains the logic for creating a Publisher and a DataWriter, and 
sending data.  

example_subscriber_1.c:
This file contains the logic for creating a Subscriber and a DataReader, a 
WaitSet, and how to use the WaitSet to be notified of events.

example_subscriber_2.c:
This file contains the logic for creating a Subscriber and a DataReader, and  
polling is used (calling take() in a loop) to access data.

examplePlugin.c:
This file creates the plugin for the example data type.  This file contains 
the code for serializing and deserializing the example type, creating, 
copying, printing and deleting the example type, determining the size of the 
serialized type, and handling hashing a key, and creating the plug-in.

exampleSupport.c
This file defines the example type and its typed DataWriter, DataReader, and 
Sequence.

example.c
This file contains the APIs for managing the example type. 


Compiling w/ cmake
==================

The following assumptions are made:

-   The environment variable RTIMEHOME is set to the Connext Micro installation 
    directory. 
-   Micro libraries exist in your installation for the architecture in question. 
    If you are unsure if this is the case, please consult the product 
    documentation.
    https://community.rti.com/static/documentation/connext-micro/2.4.12/doc/html/usersmanual/index.html
-   Micro libraries have already been built for x64Win64VS2017    

On Ubuntu 20.04
-----------------
    $ cd your/project/directory
    $ $RTIMEHOME/resource/scripts/rtime-make --config Release --build --name x64Linux5gcc9.3.0 --target Linux --source-dir . -G "Unix Makefiles" --delete


On Windows 
-----------------
    > cd your\project\directory 
    > %RTIMEHOME%\resource\scripts\rtime-make --config Release --build --name x64Win64VS2017 --target Windows --source-dir . -G "Visual Studio 15 2017 Win64" --delete

The executables can be found in the objs\<architecture>\Release directory

Note that there a few variables at the top of main() in both the publisher and
subscriber code that may need to be changed to match your system:

    // user-configurable values
    char *peer = "127.0.0.1";
    char *loopback_name = "Loopback Pseudo-Interface 1";
    char *eth_nic_name = "Wireless LAN adapter Wi-Fi";
    int domain_id = 100;

By default, the remote peer is set to the loopback address (allowing the 
examples to discover each other on the same machine only) and DDS Domain 100 is 
used. The names of the network interfaces should match the actual host where the
example is running (the defaults work for a Windows 10 machine and an Ubuntu 
20.04 machine.)


Running example_publisher and example_subscriber
================================================

Using a Windows10 system as the example, run the first subscriber in one 
terminal with the command:

    > objs\x64Win64VS2017\Release\example_subscriber_1.exe 
    
Run the second subscriber in another terminal with the command:

    > objs\x64Win64VS2017\Release\example_subscriber_2.exe 

And run the publisher in a forth terminal with the command:

    > objs\x64Win64VS2017\Release\example_publisher.exe 

Similarly, under Ubuntu 20.04:

    $ ./objs/x64Linux5gcc9.3.0/example_subscriber_1 
    
Run the second subscriber in another terminal with the command:

    $ ./objs/x64Linux5gcc9.3.0/example_subscriber_2  

And run the publisher in a third terminal with the command:

    $ ./objs/x64Linux5gcc9.3.0/example_publisher