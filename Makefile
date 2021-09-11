SOURCES := ssd1351.cpp snake.cpp

HEADERS := hwspi.hpp ssd1351.hpp hwlib_ssd1351.hpp snake.hpp hwlib-font-color-16x16.hpp

SEARCH  :=./ ./hardware ./ssd1351 ./snake/ ./font

RESULTS := main.lst main.lss

RELATIVE := ..
include $(RELATIVE)/Makefile.due