.PHONY: clean purge

# TODO: Add release build configuration.

#
# Paths.
#

EXE_NAME = space-game
EXE_PREFIX = bin
EXE_SUFFIX = debug
EXE_DIR = ${EXE_PREFIX}/${EXE_SUFFIX}
EXE_PATH = ${EXE_DIR}/${EXE_NAME}
OBJ_PREFIX = obj
OBJ_SUFFIX = debug
OBJ_DIR = ${OBJ_PREFIX}/${OBJ_SUFFIX}
SRC_DIR = src
SOURCES = $(wildcard ${SRC_DIR}/*.cxx)
OBJECTS = $(patsubst ${SRC_DIR}/%.cxx, ${OBJ_DIR}/%.o, ${SOURCES})
DEPENDS = $(wildcard ${OBJ_DIR}/*.d)

#
# Compiler & linker flags.
#

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wunreachable-code -Wshadow
STANDARD_CXX = -std=c++17
OPTIMIZE = -Og
DEBUG = -DDEBUG -g
GEN_DEPENDS = -MMD

#
# Libraries.
#

INCLUDE_PREFIX = include
INCLUDES = -I${INCLUDE_PREFIX}
LIB_SDL = $$(pkg-config --cflags --libs sdl2)
LIBRARIES = ${LIB_SDL}

#
# Recipes.
#

all: prebuild exe

prebuild: ${EXE_DIR} ${OBJ_DIR}

${EXE_DIR}:
	mkdir -p ${EXE_DIR}

ifneq (${DEPENDS},)
include ${DEPENDS}
endif

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

exe: ${EXE_PATH}

${EXE_PATH}: ${OBJECTS}
	${CXX} -o $@ $^ ${LIBRARIES}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cxx
	${CXX} -c -o $@ $< ${GEN_DEPENDS} ${STANDARD_CXX} ${WARNINGS} ${DEBUG} ${OPTIMIZE}

clean:
	${RM} -v ${EXE_PATH} ${OBJ_DIR}/*.o

purge:
	${RM} -r -v ${EXE_PREFIX} ${OBJ_PREFIX}
