# set(srcs
#     "board.c")

# idf_component_register(SRCS "oled.c" "message.c" "mesh_device_app.c" "mesh_client.c" "main.c" "${srcs}"
#                     INCLUDE_DIRS  ".")
set(srcs "main.c"
        "board.c"
        "mesh_client.c"
        "mesh_server.c"
        "mesh_device_app.c"
        "message.c"
        "ssd1306/ssd1306.c"
        "ssd1306/ssd1306_fonts.c"
        "oled.c"
        "app_mqtt.c"
        )

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  "."
                    INCLUDE_DIRS  "ssd1306/include")

component_compile_options(-Wno-error=format= -Wno-format)

# set(COMPONENT_EMBED_TXTFILES "isrgrootx1.pem")

