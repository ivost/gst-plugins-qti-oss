cmake_minimum_required(VERSION 3.10)
project(azc)

include_directories(.)
# link_directories(. /usr/lib /usr/local/lib)

add_executable(azc 
	sample1.c 
	certs.c
	parson.c
	mqtt_client.c mqtt_codec.c mqtt_message.c
	iothub_client.c iothub_client_core.c iothub_client_core_ll.c iothub_client_diagnostic.c
	iothub_client_ll.c iothub_device_client.c iothub_device_client_ll.c iothub_message.c
	iothubtransport.c version.c
	iothub_client_authorization.c iothub_client_retry_control.c
	iothub_transport_ll_private.c iothubtransport_mqtt_common.c iothubtransportmqtt.c
	)

target_link_libraries(azc
	crypto
	ssl
	curl
	pthread
	m
	)
