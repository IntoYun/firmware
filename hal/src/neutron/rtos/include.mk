
TARGET_HAL_FREERTOS_PATH = $(HAL_MODULE_PATH)/src/$(PLATFORM_NAME)/rtos/FreeRTOSv9.0.0/FreeRTOS

# Add include to all objects built for this target
INCLUDE_DIRS += $(TARGET_HAL_FREERTOS_PATH)/Source/include
INCLUDE_DIRS += $(TARGET_HAL_FREERTOS_PATH)/Source/portable/GCC/ARM_CM4F


