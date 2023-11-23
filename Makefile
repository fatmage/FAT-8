CC := gcc
CCFLAGS := -Wall -Werror -g -DNCURSES
LDFLAGS := -lncurses
DBGFLAGS := -g
CCOBJFLAGS := $(CCFLAGS) -c

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

project_name := fat-8

TARGET_NAME := fat-8
TARGET := $(BIN_PATH)/$(TARGET_NAME)

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

DISTCLEAN_LIST := $(OBJ)
CLEAN_LIST := $(TARGET) \
			  $(DISTCLEAN_LIST)

default: makedir all

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH)

.PHONY: all
all: $(TARGET)


.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST) $(BIN_PATH)/ $(OBJ_PATH)/
	@rm -f $(CLEAN_LIST)
	@rmdir $(BIN_PATH) $(OBJ_PATH)

.PHONY: distclean
distclean:
	@echo CLEAN $(CLEAN_LIST) $(OBJ_PATH)/
	@rm -f $(DISTCLEAN_LIST)
	@rmdir $(OBJ_PATH)