SHADERS_SRC := $(shell find assets/shaders -name "*.vert" -o -name "*.frag")

SHADERS_OBJ := $(addsuffix .spv, $(SHADERS_SRC))

all: $(SHADERS_OBJ)

%.spv: %
	glslc $< -o $@

clean:
	rm -f $(SHADERS_OBJ)
