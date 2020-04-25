ARMGNU ?= aarch64-linux-gnu

CFLAGS = -Wall -O2 -std=gnu17 -nostdlib -nostartfiles -ffreestanding

BUILD_DIR ?= build

.PHONY: run

all: $(BUILD_DIR)/kernel8.img

clean:
	rm -rf $(BUILD_DIR)


$(BUILD_DIR)/%_c.o: src/%.c
	@mkdir -p $(@D)
	@echo $<
	@$(ARMGNU)-gcc $(CFLAGS) -MMD -Iinclude -Isrc/third_party/printf -c $< -o $@
	
$(BUILD_DIR)/%_s.o: src/%.S
	@mkdir -p $(@D)
	@echo $<	
	@$(ARMGNU)-gcc $(CFLAGS) -MMD -Iinclude -c $< -o $@


C_FILES = $(shell find src -type f -name '*.c')
ASM_FILES =  $(shell find src -type f -name '*.S')
OBJ_FILES = $(C_FILES:src/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:src/%.S=$(BUILD_DIR)/%_s.o)


DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

echo:
	echo $(OBJ_FILES)


$(BUILD_DIR)/kernel8.img: src/linker.ld $(OBJ_FILES)
	@echo 'Link $(BUILD_DIR)/kernel8.elf' 
	@$(ARMGNU)-ld -T src/linker.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary $(BUILD_DIR)/kernel8.img


run: $(BUILD_DIR)/kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel $(BUILD_DIR)/kernel8.img -serial stdio
