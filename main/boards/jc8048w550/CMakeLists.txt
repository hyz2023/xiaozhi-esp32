idf_component_register(
    SRCS 
        "jc8048w550.cc"
        "esp_lcd_st7262.c"
    INCLUDE_DIRS "."
    REQUIRES 
        esp_lcd
        driver
        esp_common
) 