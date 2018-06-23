CXX=g++
CC=gcc
CLINK=gcc
WINDRES=
CDEPS=gcc -MM
DEPEX=*.c

#CFLAGS=-g3  `gtk-config --cflags` -D__COMPILE_MINIFMOD__ -D__LINUX__ -Wall -W -Os -ffast-math -fschedule-insns2 -fstrict-aliasing -pipe
CFLAGS=-g3  `gtk-config --cflags` -D__COMPILE_MINIFMOD__ -D__LINUX__ -Wall -W -Os -march=i386 -mcpu=i386 -ffast-math -fstrict-aliasing -pipe
LDFLAGS=-g3 -D__COMPILE_MINIFMOD__ -D__LINUX__ -Wall -W -Os -pipe -s
#LIBS=-lm -lpthread  `gtk-config --libs` -lXxf86vm
LIBS=-lm -lpthread  `gtk-config --libs`

NASM=nasm
NASMFLAGS=-f elf

SOURCE:=font.c drawchar.c effects.c 3de.c primperan.c
SOURCE+=oldskool-linux.c oldskool-mxm.c
SOURCE+=linux/x11.c linux/config.c linux/xf86vmodemini.c

SOURCE+=mxm/mxmplay.asm mxm/ossio.asm mxm/module.asm

#SOURCE+=minifmod/fmusic.c minifmod/fsound.c minifmod/mixer_clipcopy.c
#SOURCE+=minifmod/mixer_fpu_ramp.c minifmod/mixer_fpu_ramp_asm.asm minifmod/music_formatxm.c
#SOURCE+=minifmod/system_file.c

EXE=primperan
BUILD=linux-i586
OEX=o

include Makefile.common

