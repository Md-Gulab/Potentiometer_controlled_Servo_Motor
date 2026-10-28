TARGET  = build/main
CPU     = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
INCS    = -Iinc -ICMSIS/Include -ICMSIS/Device/ST/STM32F4xx/Include

CFLAGS  = $(CPU) -DSTM32F446xx $(INCS) -Wall -Wextra -g -O0 \
          -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS = $(CPU) -g
LDFLAGS = $(CPU) -T linker.ld -specs=nano.specs -specs=nosys.specs \
          -Wl,--gc-sections -Wl,-Map=$(TARGET).map

CSRCS = $(wildcard src/*.c)
ASRCS = $(wildcard src/*.s)
OBJS  = $(CSRCS:src/%.c=build/%.o) $(ASRCS:src/%.s=build/%.o)

.PHONY: all clean flash debug

all: $(TARGET).elf
	arm-none-eabi-size $<

$(TARGET).elf: $(OBJS)
	arm-none-eabi-gcc $(OBJS) $(LDFLAGS) -o $@

build/%.o: src/%.c | build
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

build/%.o: src/%.s | build
	arm-none-eabi-gcc $(ASFLAGS) -c $< -o $@

build:
	mkdir build

-include $(OBJS:.o=.d)

clean: build
	rm -rf build/

# TODO (yours): clean, flash (OpenOCD "program ... verify reset exit"), debug