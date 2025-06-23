# set(srcs
#        )

# idf_component_register(SRCS "relay.c" "main.c" "board.c" "mesh_device_app.c" "mesh_server.c" "mesh_client.c" "${srcs}"
#                     INCLUDE_DIRS  ".")
# ")
set(srcs "main.c"
        "mesh_client.c"
        "mesh_server.c"
        "mesh_device_app.c"
        "esp32-dht11.c"
        "relay.c"
        )

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")

component_compile_options(-Wno-error=format= -Wno-format)
