CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -O2 `pkg-config allegro-5 --cflags`
LDFLAGS := -lm
LDLIBS := `pkg-config allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5 allegro_video-5 --libs`

Main: *.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) *.cpp -o Main

clean:
	rm -f Main

.PHONY: Main clean
