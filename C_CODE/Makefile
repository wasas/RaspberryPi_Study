###############################################################################
#
#	A makefile script for generation of raspberry pi kernel images.
###############################################################################


ARMGNU ?= arm-none-eabi

# The intermediate directory for compiled object files.
BUILD = build/

# The directory in which source files are stored.
SOURCE = source/

# The name of the output file to generate.
TARGET = kernel7.img

# The name of the assembler listing file to generate.
LIST = kernel.list

# The name of the map file to generate.
MAP = kernel.map

# The name of the linker script to use.
LINKER = kernel.ld

ASM_FLAGS = -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7
ASM_DEFINES = -DRPI2=1

C_FLAGS =  -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -O1 -nostartfiles
C_DEFINES = -DRPI2=1

# The names of all object files that must be generated. Deduced from the 
# assembly code files in source.

# Rule to make everything.
all: $(TARGET) $(LIST)

# Rule to remake everything. Does not include clean.
rebuild: all

# Rule to make the listing file.
$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -d $(BUILD)output.elf > $(LIST)

# Rule to make the image file.
$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET) 

$(BUILD)output.elf : $(BUILD)armc-cstartup.o $(BUILD)armc-cstubs.o $(BUILD)armc.o $(BUILD)armc-start.o $(BUILD)rpi-armtimer.o $(BUILD)rpi-aux.o $(BUILD)rpi-gpio.o $(BUILD)rpi-interrupts.o $(BUILD)rpi-systimer.o $(BUILD)rpi-led.o
	$(ARMGNU)-gcc  $(C_FLAGS) -T $(LINKER) $^ -o $@

$(BUILD)armc-cstartup.o : $(SOURCE)armc-cstartup.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c  $(SOURCE)armc-cstartup.c

$(BUILD)armc-cstubs.o : $(SOURCE)armc-cstubs.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)armc-cstubs.c

$(BUILD)armc.o : $(SOURCE)armc.c $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)armc.c 

$(BUILD)armc-start.o : $(SOURCE)armc-start.S $(BUILD)
	$(ARMGNU)-gcc $(ASM_DEFINES) $(ASM_FLAGS) -o $@ -c $(SOURCE)armc-start.S

$(BUILD)rpi-armtimer.o : $(SOURCE)rpi-armtimer.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)rpi-armtimer.c 

$(BUILD)rpi-aux.o : $(SOURCE)rpi-aux.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)rpi-aux.c

$(BUILD)rpi-gpio.o : $(SOURCE)rpi-gpio.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)rpi-gpio.c

$(BUILD)rpi-interrupts.o : $(SOURCE)rpi-interrupts.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)rpi-interrupts.c

$(BUILD)rpi-systimer.o : $(SOURCE)rpi-systimer.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)rpi-systimer.c 

$(BUILD)rpi-led.o : $(SOURCE)rpi-led.c  $(BUILD)
	$(ARMGNU)-gcc $(C_DEFINES) $(C_FLAGS) -o $@ -c $(SOURCE)rpi-led.c 

$(BUILD):
	mkdir $@
# Rule to clean files.
clean : 
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)
