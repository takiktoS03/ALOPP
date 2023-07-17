
#############################
# Generator plikow makefile #
# Krzysztof Siminski, 2022  #
#############################

# STALE:
kompilator=g++
standard=-std=c++20

optRelease=-O3
optDebug=

errors=-pedantic-errors

release=
debug=-g

#rownoleglosc=-fopenmp
rownoleglosc=

release_folder=_release
debug_folder=_debug

#############################
dane_folder=dane
#############################

.PHONY : clean $(debug_folder) $(release_folder)

# opcje uruchomienia projektu:

release : $(release_folder) $(release_folder)/main
	./$(release_folder)/main
	for i in `seq 1 11` ; do \
	echo $$i ; \
        ./$(release_folder)/main -i $(dane_folder)/wejscie$$i.txt -o $(dane_folder)/wyjscie$$i.txt ; \
        done

debug : $(debug_folder) $(debug_folder)/main
	valgrind --leak-check=yes ./$(debug_folder)/main

# utworzenie folderow:

$(release_folder) : 
	mkdir -p $(release_folder)
$(debug_folder) : 
	mkdir -p $(debug_folder)

# kompilacja zrodel:

$(release_folder)/funkcje.o : funkcje.cpp
	$(kompilator) $(standard) $(release) $(optRelease) $(rownoleglosc) $(errors) -c -o $@ $^
$(debug_folder)/funkcje.o : funkcje.cpp
	$(kompilator) $(standard) $(debug) $(optyDebug) $(rownoleglosc) $(errors) -c -o $@ $^
$(release_folder)/main.o : main.cpp
	$(kompilator) $(standard) $(release) $(optRelease) $(rownoleglosc) $(errors) -c -o $@ $^
$(debug_folder)/main.o : main.cpp
	$(kompilator) $(standard) $(debug) $(optyDebug) $(rownoleglosc) $(errors) -c -o $@ $^
$(release_folder)/gauss.o : gauss.cpp
	$(kompilator) $(standard) $(release) $(optRelease) $(rownoleglosc) $(errors) -c -o $@ $^
$(debug_folder)/gauss.o : gauss.cpp
	$(kompilator) $(standard) $(debug) $(optyDebug) $(rownoleglosc) $(errors) -c -o $@ $^

# linkowanie:

$(release_folder)/main : $(release_folder)/funkcje.o $(release_folder)/main.o $(release_folder)/gauss.o 
	$(kompilator) $(standard) $(release) $(optRelease) $(rownoleglosc) $(errors) -o $@ $^

$(debug_folder)/main : $(debug_folder)/funkcje.o $(debug_folder)/main.o $(debug_folder)/gauss.o 
	$(kompilator) $(standard) $(debug) $(optDebug) $(rownoleglosc) $(errors) -o $@ $^

# usuniecie plikow skompilowanych:

clean : 
	if [ -d $(debug_folder)      ] ; then rm -r $(debug_folder)   ; fi
	if [ -d $(release_folder)    ] ; then rm -r $(release_folder) ; fi;


