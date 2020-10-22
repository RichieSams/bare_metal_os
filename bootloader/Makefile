ARMGNU ?= aarch64-linux-gnu

CFLAGS = -Wall -nostdlib -nostartfiles -ffreestanding

BUILD_DIR ?= build


all: build

clean:
	rm -rf $(BUILD_DIR)


$(BUILD_DIR)/%_c.o: %.c
	@mkdir -p $(@D)
	@echo $<
	@$(ARMGNU)-gcc $(CFLAGS) -MMD -I../kernel/src -c $< -o $@
	
$(BUILD_DIR)/%_s.o: %.S
	@mkdir -p $(@D)
	@echo $<	
	@$(ARMGNU)-gcc $(CFLAGS) -MMD -c $< -o $@

C_FILES = $(wildcard *.c)
ASM_FILES =  $(wildcard *.S)
OBJ_FILES = $(C_FILES:%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)


$(BUILD_DIR)/kernel8.img: linker.ld $(OBJ_FILES)
	@echo 'Link $(BUILD_DIR)/kernel8.elf' 
	@$(ARMGNU)-ld -T linker.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary $(BUILD_DIR)/kernel8.img

build: $(BUILD_DIR)/kernel8.img

/media/adrian/boot/kernel8.img: $(BUILD_DIR)/kernel8.img
	cp $< $@

copy: /media/adrian/boot/kernel8.img
