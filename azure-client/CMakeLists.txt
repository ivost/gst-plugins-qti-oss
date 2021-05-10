cmake_minimum_required(VERSION 3.10)
project(azure-client)

include_directories(. /usr/local/include/azureiot/)
link_directories(. /usr/lib /usr/local/lib)

add_executable(azc sample1.c certs.c)

target_link_libraries(azc
        iothub_client_mqtt_transport
        iothub_client
        umqtt
        aziotsharedutil
        parson
        crypto
        ssl
        curl
        pthread
        m
        )
