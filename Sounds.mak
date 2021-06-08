SOUND_DIR = $(PROJ_DIR)/sounds

SOUNDS = $(wildcard $(SOUND_DIR)/*.mp3)
SOUNDS_RAW = $(SOUNDS:$(SOUND_DIR)/%.mp3=$(BUILD_DIR)/AAS_Data/%.raw)
NUM_SOUNDS = $(words $(SOUNDS))

#cursed
SOUND_NAMES = $(SOUNDS:$(SOUND_DIR)/%.mp3=%)
ADD_SOUNDS = $(foreach sound, $(SOUND_NAMES), addSound(AAS_DATA_SFX_START_$(sound), AAS_DATA_SFX_END_$(sound), \"$(sound)\");\n)


.PHONY : aas_data
aas_data:
	@cp -r $(SOUND_DIR) $(BUILD_DIR)/AAS_Data

%.wav: %.mp3
	@ffmpeg -i $< $@ 2> /dev/null

%.raw: %.wav
	@sox $< -b 8 -c 1 --rate 8000 $@


.PHONY : sound_map
sound_map: | aas_data $(SOUNDS_RAW)
	@cp $(GBALIB_DIR)/sound_map.c $(BUILD_DIR)
	@sed -i "s#/\*NUM_SOUNDS\*/#$(NUM_SOUNDS)#g" $(BUILD_DIR)/sound_map.c
	@sed -i "s#/\*POPULATE_SOUNDS\*/#$(ADD_SOUNDS)#g" $(BUILD_DIR)/sound_map.c
