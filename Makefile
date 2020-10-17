AR=ar 	 	    		
AS=as
CC=gcc
CPP=cpp
CXX=g++
LD=ld
OBJCOPY = objcopy
OBJDUMP = objdump
STRIP = strip

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTBIN_DIR = ./testbin

UNAME := $(shell uname)

PKGS = 

DEBUG_MODE=TRUE
ifdef DEBUG_MODE
DEFINES  += -DDEBUG
CFLAGS   += -g -ggdb -D_GLIBCXX_DEBUG
else 
CFLAGS   += -O3
endif

ifeq ($(UNAME), Darwin)
# add paths if developing on mac with macports
INCLUDE     += -I '/opt/local/include'
TESTLDFLAGS += -L '/opt/local/lib'
endif

INCLUDE         += -I $(INC_DIR)
CFLAGS          += -Wall
LDFLAGS         += 
TESTLDFLAGS     += -lgtest -lgtest_main  -lpthread
CPPFLAGS        += -std=c++14 
PROJ_NAME       = proj2
TESTEDIT_NAME   = testedit

MAIN_OBJ        = $(OBJ_DIR)/main.o

TESTEDIT_OBJ    = $(OBJ_DIR)/testedit.o

PROJ_OBJS       = $(OBJ_DIR)/EditBuffer.o 			
      
                    
all: directories test $(BIN_DIR)/$(PROJ_NAME)
test: $(TESTBIN_DIR)/$(TESTEDIT_NAME)
	$(TESTBIN_DIR)/$(TESTEDIT_NAME)
	
$(BIN_DIR)/$(PROJ_NAME): $(PROJ_OBJS) $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(PROJ_OBJS) -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 
	
$(TESTBIN_DIR)/$(TESTEDIT_NAME): $(PROJ_OBJS) $(TESTEDIT_OBJ)
	$(CXX) $(PROJ_OBJS) $(TESTEDIT_OBJ) -o $(TESTBIN_DIR)/$(TESTEDIT_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

directories: $(BIN_DIR) $(OBJ_DIR) $(TESTBIN_DIR)
	
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TESTBIN_DIR):
	mkdir -p $(TESTBIN_DIR)

clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(TESTBIN_DIR)
	
.PHONY: clean