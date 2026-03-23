LIB_NAME = Ulogger
LIB_VERSION = 1.0
PREFIX ?= /usr/local
PREFIX_ABS = $(abspath $(PREFIX))
# Compiler setting
CXX = g++
CXXFLAGS = -fPIC -Wall -g -O2
SHARED_FLAGS = -shared
# Directory
SRC_DIR = src
BIN_DIR = bin
MAN_DIR = man
TEST_DIR = test
# PREFIX
INSTALL_BIN = $(PREFIX_ABS)/bin
INSTALL_LIB = $(PREFIX_ABS)/lib
INSTALL_INC = $(PREFIX_ABS)/include
INSTALL_MAN = $(PREFIX_ABS)/man/man1

# Source files
LIB_SRC = $(SRC_DIR)/$(LIB_NAME).cc
LIB_OBJ = $(LIB_SRC:.cc=.o)
LIB_OUT = lib$(LIB_NAME).so.$(LIB_VERSION)
# Model files
CONFIG_SCRIPT_IN = $(BIN_DIR)/$(LIB_NAME)-config.in
CONFIG_SCRIPT_OUT = $(BIN_DIR)/$(LIB_NAME)-config
ENV_SCRIPT_IN = $(BIN_DIR)/$(LIB_NAME)-env.sh.in
ENV_SCRIPT_OUT = $(BIN_DIR)/$(LIB_NAME)-env.sh
# Test
TEST_SRC = $(TEST_DIR)/test_$(LIB_NAME).cc
TEST_OUT = $(TEST_DIR)/test_$(LIB_NAME)

########################## Target ############################

########################## Build ############################
all: $(LIB_OUT) $(CONFIG_SCRIPT_OUT) $(ENV_SCRIPT_OUT)
	# echo "OK"
	# echo ${LIB_SRC}

# Dynamic library
$(LIB_OUT): $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) -o $@ $(LIB_OBJ)
	ln -sf $@ lib$(LIB_NAME).so
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(CONFIG_SCRIPT_OUT): $(CONFIG_SCRIPT_IN)
	sed -e "s|@PREFIX_ABS@|$(PREFIX_ABS)|g" $< > $@
	chmod +x $@
$(ENV_SCRIPT_OUT): $(ENV_SCRIPT_IN)
	sed -e "s|@PREFIX_ABS@|$(PREFIX_ABS)|g" $< > $@
	chmod +x $@

# Test
test: all $(TEST_OUT)
$(TEST_OUT): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(shell ./$(CONFIG_SCRIPT_OUT) --libs) $(shell ./$(CONFIG_SCRIPT_OUT) --cflags)

# Run test
run: test
	LD_LIBRARY_PATH=./:$$LD_LIBRARY_PATH ./$(TEST_OUT)

########################## Install ############################
install: all
	mkdir -p $(INSTALL_BIN) $(INSTALL_LIB) $(INSTALL_INC) $(INSTALL_MAN)
	cp -f lib$(LIB_NAME).so.$(LIB_VERSION) $(INSTALL_LIB)/
	ln -sf $(INSTALL_LIB)/lib$(LIB_NAME).so.$(LIB_VERSION) $(INSTALL_LIB)/lib$(LIB_NAME).so
	cp -f $(SRC_DIR)/$(LIB_NAME).hh $(INSTALL_INC)/
	cp -f $(CONFIG_SCRIPT_OUT) $(INSTALL_BIN)/
	cp -f $(ENV_SCRIPT_OUT) $(INSTALL_BIN)/
	cp -f $(MAN_DIR)/$(LIB_NAME).1 $(INSTALL_MAN)/

########################## Uninstall ############################
uninstall:
	rm -f $(INSTALL_LIB)/lib$(LIB_NAME).so*
	rm -f $(INSTALL_INC)/$(LIB_NAME).hh
	rm -f $(INSTALL_BIN)/$(LIB_NAME)-config
	rm -f $(INSTALL_BIN)/$(LIB_NAME)-env.sh
	rm -f $(INSTALL_MAN)/$(LIB_NAME).1

########################## Clean ############################
clean:
	rm -f $(LIB_OBJ) lib$(LIB_NAME).so*
	rm -f $(CONFIG_SCRIPT_OUT) $(ENV_SCRIPT_OUT)
	rm -f $(TEST_OUT)
	rm -rf $(TEST_DIR)/*.o

########################## PHONY ############################
.PHONY: all test run install uninstall clean

