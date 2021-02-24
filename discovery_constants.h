/*
* (c) Copyright, Real-Time Innovations, 2021.  All rights reserved.
* RTI grants Licensee a license to use, modify, compile, and create derivative
* works of the software solely for use with RTI Connext DDS. Licensee may
* redistribute copies of the software provided that all such copies are subject
* to this license. The software is provided "as is", with no warranty of any
* type, including any warranty for fitness for any purpose. RTI is under no
* obligation to maintain or support the software. RTI shall not be liable for
* any incidental or consequential damages arising out of the use or inability
* to use the software.
*/

#ifndef RTPS_OBJECT_IDS_H
#define RTPS_OBJECT_IDS_H

// contants for the publishing application (example_publisher)
static const char *k_PARTICIPANT01_NAME          = "publisher_1a";
static const int k_OBJ_ID_PARTICIPANT01_DW01     = 100;
static const char *k_PARTICIPANT02_NAME          = "publisher_1b";
static const int k_OBJ_ID_PARTICIPANT02_DW01     = 200;

// contants for one of the subscribing applications (example_subscriber_1) 
static const char *k_PARTICIPANT03_NAME          = "subscriber_1";
static const int k_OBJ_ID_PARTICIPANT03_DR01     = 300;

// contants for the other subscribing application (example_subscriber_2)
static const char *k_PARTICIPANT04_NAME          = "subscriber_2";
static const int k_OBJ_ID_PARTICIPANT04_DR01     = 400;

#endif