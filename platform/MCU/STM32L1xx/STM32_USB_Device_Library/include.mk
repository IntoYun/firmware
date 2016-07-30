TARGET_USB_FS_PATH = $(PLATFORM_MCU_PATH)/STM32_USB_Device_Library

# Add include to all objects built for this target
INCLUDE_DIRS += $(TARGET_USB_FS_PATH)/Class/CDC/Inc
INCLUDE_DIRS += $(TARGET_USB_FS_PATH)/Class/DFU/Inc
INCLUDE_DIRS += $(TARGET_USB_FS_PATH)/Core/Inc

