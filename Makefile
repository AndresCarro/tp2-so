
all:  bootloader kernel userland image
buddy: bootloader kernel_buddy userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

kernel_buddy:
	cd Kernel; make buddy

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean
	rm -rf cppcheck.txt PVS-Studio.log report.tasks strace_out

static_analysis:
	cppcheck --quiet --enable=all --force --inconclusive . 2> cppcheck.txt
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log

.PHONY: bootloader image collections kernel userland all clean
