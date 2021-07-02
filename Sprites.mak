SPRITE_DIR = $(PROJ_DIR)/sprites

SPRITES = $(wildcard $(SPRITE_DIR)/*.png)
GIFS = $(wildcard $(SPRITE_DIR)/*.gif)
NUM_SPRITES = $(words $(SPRITES))
NUM_GIFS = $(words $(GIFS))

#cursed
GIF_NAMES = $(GIFS:$(SPRITE_DIR)/%.gif=%)
GIFS_UP = $(shell echo '$(GIF_NAMES)' | tr '[:lower:]' '[:upper:]')
ADD_GIFS = $(foreach gif, $(GIFS_UP), addGif(\"$(gif)\", $(gif)_FRAMES, $(gif)1_ID - $(gif)0_ID, $(gif)0_PALETTE_ID, $(gif)_SPRITE_SHAPE, $(gif)_SPRITE_SIZE, $(gif)0_ID);\n) 
TOTAL_FRAMES = $(foreach gif, $(GIFS_UP), $(gif)_FRAMES +) 0

SPRITE_NAMES = $(SPRITES:$(SPRITE_DIR)/%.png=%)
SPRITES_UP = $(shell echo '$(SPRITE_NAMES)' | tr '[:lower:]' '[:upper:]')
ADD_SPRITES = $(foreach sprite, $(SPRITES_UP), addImage($(sprite)_PALETTE_ID, $(sprite)_SPRITE_SHAPE, $(sprite)_SPRITE_SIZE, $(sprite)_ID, \"$(sprite)\");\n)


.PHONY : nin10kit-sprite
nin10kit-sprite:
ifneq ($(NUM_SPRITES) + $(NUM_GIFS),0)
	@nin10kit --mode=sprites --bpp=8 --for_bitmap --transparent=FF00FF $(BUILD_DIR)/sprite_data $(SPRITES) $(GIFS)
else 
	@nin10kit --mode=sprites --bpp=8 --for_bitmap --transparent=FF00FF $(BUILD_DIR)/sprite_data $(GBALIB_DIR)/src/dummy_assets/dummy_image.png
endif

.PHONY : sprite_map
sprite_map: nin10kit-sprite
	@cp $(GBALIB_DIR)/sprite_map.c $(BUILD_DIR)
	@sed -i "s#/\*NUM_SPRITES\*/#$(NUM_SPRITES)#g" $(BUILD_DIR)/sprite_map.c
	@sed -i "s#/\*POPULATE_SPRITES\*/#$(ADD_SPRITES)#g" $(BUILD_DIR)/sprite_map.c
	@sed -i "s#/\*NUM_GIFS\*/#$(NUM_GIFS)#g" $(BUILD_DIR)/sprite_map.c
	@sed -i "s#/\*TOTAL_FRAMES\*/#$(TOTAL_FRAMES)#g" $(BUILD_DIR)/sprite_map.c
	@sed -i "s#/\*POPULATE_GIFS\*/#$(ADD_GIFS)#g" $(BUILD_DIR)/sprite_map.c


