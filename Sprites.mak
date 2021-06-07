SPRITE_DIR = $(PROJ_DIR)/sprites

SPRITES = $(wildcard $(SPRITE_DIR)/*.png)
NUM_SPRITES = $(words $(SPRITES))

#cursed
SPRITE_NAMES = $(SPRITES:$(SPRITE_DIR)/%.png=%)
SPRITES_UP = $(shell echo '$(SPRITE_NAMES)' | tr '[:lower:]' '[:upper:]')
ADD_SPRITES = $(foreach sprite, $(SPRITES_UP), addImage($(sprite)_PALETTE_ID, $(sprite)_SPRITE_SHAPE, $(sprite)_SPRITE_SIZE, $(sprite)_ID, \"$(sprite)\");\n)

.PHONY : nin10kit-sprite
nin10kit-sprite: 
ifneq ($(NUM_SPRITES),0)
	@nin10kit --mode=sprites --bpp=4 --for_bitmap --transparent=FF00FF $(BUILD_DIR)/sprite_data $(SPRITES)
endif

.PHONY : sprite_map
sprite_map: nin10kit-sprite
	@cp $(GBALIB_DIR)/sprite_map.c $(BUILD_DIR)
	@sed -i "s#/\*NUM_SPRITES\*/#$(NUM_SPRITES)#g" $(BUILD_DIR)/sprite_map.c
	@sed -i "s#/\*POPULATE_SPRITES\*/#$(ADD_SPRITES)#g" $(BUILD_DIR)/sprite_map.c


