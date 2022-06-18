# -*- Makefile -*-

DEP_DIR := .d
$(shell mkdir -p $(DEP_DIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
POSTCOMPILE = @mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

CXX := g++
CXXFLAGS := -std=c++17 -g
LIBS := -lglfw -lGL -lGLEW -lassimp
PRODUCT := main
SRC_DIR := src

SRCFILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))

all: $(PRODUCT)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) -o $@ -c $<
	$(POSTCOMPILE)

$(PRODUCT): $(OBJFILES)
	$(CXX) $^ $(LIBS) -o $@

.PRECIOUS: $(DEP_DIR)/%.d
.PHONY: clean

clean:
	find $(SRC) -name '*.o' -delete
	rm $(PRODUCT) 

include $(wildcard $(patsubst %,$(DEP_DIR)/%.d,$(basename $(notdir $(SRCFILES)))))

