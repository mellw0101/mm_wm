ARMV8_FASTFLAGS = \
	-O2	\
	-march=armv8-a

ARMV8_CFLAGS = \
	-std=c++20	\
	-pedantic	\
	-Wall	\
	-Wno-deprecated-declarations	\
	-DARMV8_BUILD	\
	${ARMV8_FASTFLAGS}

ARMV8_CXXFLAGS = \
	${ARMV8_CFLAGS}

ARMV8_LDFLAGS =	\
	${LIBS} \
	-flto \
	-O2 \
	-std=c++20 \
	-DARMV8_BUILD

FASTFLAGS = \
	-O3 					\
	-march=native			\
	-funroll-loops			\
	-Rpass=loop-vectorize 	\
	-flto 					\
	-m64

CFLAGS = \
	-std=c++20 						\
	-pedantic 						\
	-Wall 							\
	-Wno-deprecated-declarations	\
	${FASTFLAGS}

CXXFLAGS = \
	${CFLAGS}

LIBS = \
	-lxcb         	\
	-lxcb-keysyms	\
	-lxcb-cursor 	\
	-lxcb-icccm 	\
	-lxcb-ewmh 		\
	-lxcb-xinput	\
	-lpng 			\
	-lxcb-image 	\
	-lxcb-render 	\
	-lxcb-composite \
	-lxcb-xinerama 	\
	-lxcb-xkb 		\
	-lxcb-xfixes 	\
	-lxcb-shape 	\
	-lxcb-randr     \
	-lImlib2		\
	-lXau			\
	-lpthread		\
	-liw			\
	-lpulse			\
	-ldbus-1

CC = \
	clang

CXX = \
	clang++

CXXFLAGS = \
	-std=c++20	\
	-pedantic	\
	-Wall	\
	-Wno-deprecated-declarations	\
	${FASTFLAGS}

LDFLAGS = \
	${LIBS} \
	-flto \
	-O3 \
	-march=native \
	-std=c++20 \
	-funroll-loops \
	-Rpass=loop-vectorize \
	-flto \
	-m64

SRC_DIR = \
	src

OBJ_DIR = \
	o.files

SRC = \
	$(wildcard $(SRC_DIR)/*.cpp)

OBJ = \
	$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEPS = \
	$(OBJ:.o=.d)

all: test

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $< -o $@ -MMD -MP

test: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d test

backup:
	chmod u+x tools/backup.sh
	./tools/backup.sh

help:
	chmod u+x tools/help.sh
	./tools/help.sh

depends:
	chmod u+x tools/check_and_install.sh
	./tools/check_and_install.sh

conf:
	chmod u+x tools/configure.sh 
	./tools/configure.sh

alacritty:
	mkdir -p ~/.config/alacritty
	cp -f tools/alacritty.toml ~/.config/alacritty/

xorg_conf:
	mkdir -p /etc/X11
	cp -f tools/xorg.conf /etc/X11/

dist: clean

install: all
	mkdir -p   /bin
	cp -f test /bin/mwm
	chmod 755  /bin/mwm

install-armv8: ARMV8
	mkdir -p   /bin
	cp -f test /bin/mwm
	chmod 755  /bin/mwm

ARMV8: CXXFLAGS = ${ARMV8_CXXFLAGS}
ARMV8: LDFLAGS = ${ARMV8_LDFLAGS}
ARMV8: test

uninstall:
	rm -f /usr/local/bin/mwm

.PHONY: all depends conf clean dist install uninstall install-armv8 ARMV8 backup alacritty xorg_conf