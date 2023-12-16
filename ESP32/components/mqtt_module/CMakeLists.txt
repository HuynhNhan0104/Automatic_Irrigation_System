idf_component_register(
    SRCS "mqtt_module.c"
    INCLUDE_DIRS "include"
    REQUIRES "esp_wifi" "nvs_flash" "esp_event" "mqtt" 
)
