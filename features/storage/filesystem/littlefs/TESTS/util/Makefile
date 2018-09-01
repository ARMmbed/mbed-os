
all: test_dirs test_files test_seek test_parallel

test_%: ../../littlefs/tests/test_%.sh
	cp $< $(notdir $<)
	sed -i -e 's/tests\//.\//' -e 's/echo/.\/echo.py/' $(notdir $<)
	
	./clean.sh
	ln -f -s replacements_mbed.yml replacements.yml
	./$(notdir $<)
	mkdir -p ../filesystem/$(patsubst test_%,%,$@)
	cp main.cpp ../filesystem/$(patsubst test_%,%,$@)/main.cpp
	
	./clean.sh
	ln -f -s replacements_retarget.yml replacements.yml
	./$(notdir $<)
	mkdir -p ../filesystem_retarget/$(patsubst test_%,%,$@)
	cp main.cpp ../filesystem_retarget/$(patsubst test_%,%,$@)/main.cpp

clean:
	./clean.sh
