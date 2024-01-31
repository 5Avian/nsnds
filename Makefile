TARGET		= nsnds.nds
OBJECT		= nsnds.elf

CC			= ${DEVKITARM}/bin/arm-none-eabi-gcc
NDSTOOL		= ${DEVKITPRO}/tools/bin/ndstool

CFLAGS		= \
	-I${DEVKITPRO}/libnds/include \
	-L${DEVKITPRO}/libnds/lib \
	-DARM9 \
	-mcpu=arm946e-s \
	-specs=ds_arm9.specs \
	-ldswifi9 \
	-lnds9

# ---

nds:
	$(CC) nds/nds.c $(CFLAGS) -o $(OBJECT)
	$(NDSTOOL) -c $(TARGET) -9 $(OBJECT) -b ${DEVKITPRO}/libnds/icon.bmp "nsnds;Nintendo Switch Controller;by 5Avian"

.PHONY: nds
