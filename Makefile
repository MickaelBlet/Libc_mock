COMPILER				:=	$(CXX)
COMPILER_FLAG			:=	-DMOCKW_DLFCN -std=c++98 --pedantic -Wall -Wextra -Werror $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

INCLUDES				:=	-I./include
LIVRARIES				:=	-lgtest -lgtest_main -lgmock -lpthread -ldl

SOURCES_EXTENTION		:=	cpp

SOURCE_DIRECTORY		:=	./test
BUILD_DIRECTORY			:=	./build
BINARY_DIRECTORY		:=	./bin

SOURCES					=	$(shell find $(SOURCE_DIRECTORY) -type f -name "*.$(SOURCES_EXTENTION)")
OBJECTS					:=	$(patsubst $(SOURCE_DIRECTORY)/%,$(BUILD_DIRECTORY)/%,$(SOURCES:.$(SOURCES_EXTENTION)=.o))
TESTS					:=	$(patsubst $(SOURCE_DIRECTORY)/%,$(BINARY_DIRECTORY)/%,$(SOURCES:.$(SOURCES_EXTENTION)=.gtest))

all: $(TESTS)

clean:
	$(RM) -r $(BUILD_DIRECTORY) $(BINARY_DIRECTORY)

re: clean
	$(MAKE) all

$(DEPENDANCIES):
	$(MAKE) -C $@

.PHONY: all clean re $(DEPENDANCIES)

$(BINARY_DIRECTORY):
	mkdir -p $@

$(BUILD_DIRECTORY):
	mkdir -p $(sort $(dir $(OBJECTS)))

$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.$(SOURCES_EXTENTION) $(LIBRARIES_DEP) | $(BUILD_DIRECTORY)
	$(COMPILER) $(COMPILER_FLAG) -MMD -o $@ -c $< $(INCLUDES)

$(BINARY_DIRECTORY)/%.gtest: $(BUILD_DIRECTORY)/%.o | $(BINARY_DIRECTORY)
	$(COMPILER) $(COMPILER_FLAG) -o $@ $< $(LIVRARIES)

.SECONDARY: $(OBJECTS)

-include $(OBJECTS:.o=.d)