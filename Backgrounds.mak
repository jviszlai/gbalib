BACKGROUNDS_DIR = $(PROJ_DIR)/backgrounds

BACKGROUNDS = $(wildcard $(BACKGROUNDS_DIR)/*.png)
NUM_BACKGROUNDS = $(words $(BACKGROUNDS))

#cursed
BACKGROUND_NAMES = $(BACKGROUNDS:$(BACKGROUNDS_DIR)/%.png=%)
BACKGROUNDS_UP = $(shell echo '$(BACKGROUND_NAMES)' | tr '[:lower:]' '[:upper:]')
BACKGROUNDS_LOW = $(shell echo '$(BACKGROUND_NAMES)' | tr '[:upper:]' '[:lower:]')

ADD_BACKGROUNDS_1 = $(foreach background, $(BACKGROUNDS_UP), addImage($(background)_WIDTH, $(background)_HEIGHT, \"$(background)\");\n)

ADD_BACKGROUNDS_2 = $(foreach background, $(BACKGROUNDS_LOW), addImageData($(background));\n)


.PHONY : nin10kit-background
nin10kit-background: 
ifneq ($(NUM_BACKGROUNDS),0)
	@nin10kit --mode=3 $(BUILD_DIR)/background_data $(BACKGROUNDS)
endif

.PHONY : background_map
background_map: nin10kit-background
	@cp $(GBALIB_DIR)/background_map.c $(BUILD_DIR)
	@sed -i "s#/\*NUM_BACKGROUNDS\*/#$(NUM_BACKGROUNDS)#g" $(BUILD_DIR)/background_map.c
	@sed -i "s#/\*POPULATE_BACKGROUNDS_1\*/#$(ADD_BACKGROUNDS_1)#g" $(BUILD_DIR)/background_map.c
	@sed -i "s#/\*POPULATE_BACKGROUNDS_2\*/#$(ADD_BACKGROUNDS_2)#g" $(BUILD_DIR)/background_map.c


