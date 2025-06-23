# set(srcs
#        )

# idf_component_register(SRCS "rfid_servo.c" "main.c" "board.c" "mesh_device_app.c" "mesh_server.c" "mesh_client.c" "${srcs}"
#                     INCLUDE_DIRS  ".")
# ")
set(srcs "main.c"
        "mesh_client.c"
        "mesh_server.c"
        "mesh_device_app.c"
        "rfid_servo.c"
        "rc522/rc522.c"
         )

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  "."
                    INCLUDE_DIRS  "rc522"
                    )

component_compile_options(-Wno-error=format= -Wno-format)
