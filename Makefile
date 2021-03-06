BUILD_DIR = ./objects/
SRC_DIR = ./source/
PI_DIR = ./source/piStuffs/
# Get all C files 
CFILES = $(notdir $(wildcard $(SRC_DIR)*.c))
PCFILES = $(notdir $(wildcard $(PI_DIR)*.c))

# Set the location for object files
OFILES = $(addprefix $(BUILD_DIR), $(CFILES:.c=.o))
POFILES = $(addprefix $(BUILD_DIR), $(PCFILES:.c=.o))

# Flags
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib -nostartfiles

# make all
all: $(BUILD_DIR)kernel8.img set_title

# convert .S file to .o file
$(BUILD_DIR)start.o: $(PI_DIR)start.S
	aarch64-none-elf-gcc $(CFLAGS) -c $(PI_DIR)start.S -o $(BUILD_DIR)start.o

# convert .c files to .o files
$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@

# convert .c files to .o files
$(BUILD_DIR)%.o: $(PI_DIR)%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@

# Use the object files with the .ld file and convert to .elf file
# Then convert .elf file to img file
$(BUILD_DIR)kernel8.img: $(BUILD_DIR)start.o $(OFILES) $(POFILES)
	aarch64-none-elf-ld $(LDFLAGS) $(BUILD_DIR)start.o $(POFILES) $(OFILES) -T $(PI_DIR)link.ld -o $(BUILD_DIR)kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)kernel8.elf $(BUILD_DIR)kernel8.img
	
# Clear object files
clean:
	del .\objects\kernel8.elf .\objects\*.o

# Run the emulator
run:
	qemu-system-aarch64 -M raspi3 -kernel $(BUILD_DIR)kernel8.img -serial null -serial stdio
	
# Rebuild and run
test: all music run

# Set title for the command prompt 
set_title:
	TITLE Bare Metal_ASSESSMENT3 

# Execute music 
music:
	cmd /C start .\bensound-jazzyfrenchy.mp3 --loop
	