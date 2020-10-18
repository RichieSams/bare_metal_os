all: build_kernel build_bootloader

build_kernel:
	$(MAKE) -c kernel build

build_bootloader:
	$(MAKE) -c bootloader build
