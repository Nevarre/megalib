#----------------------------------------------------------------
#
#  Makefile for Spectral Analyzer
#
#  Author: Andreas Zoglauer and Michelle Galloway
#
#----------------------------------------------------------------



#----------------------------------------------------------------
# Definitions:
#

PRG	 = $(BN)/spectralyze

MOBJS    = $(LB)/MSpectralyzerMain.o

GOBJS    = $(LB)/MGUISpectralAnalyzer.o 

LOBJS    = $(LB)/MSettingsSpectralyzer.o \
	$(LB)/MSettingsSpectralOptions.o \
	$(LB)/MSpectralAnalyzer.o \
	$(LB)/MPeak.o \
	$(LB)/MQualifiedIsotope.o \

SGOBJ = $(LB)/libSpectralyzeGui.$(DLL)
SLOBJ = $(LB)/libSpectralyze.$(DLL)


#----------------------------------------------------------------
# Commands:
#

all: $(PRG)

lib: $(SLOBJ)

gui: $(SGOBJ)

link:
		@$(LINK) $(shell pwd)/inc/*.h $(IN)
		@rm -f $(IN)/LinkDef.h

clean:
		@rm -f src/*~
		@rm -f src/\#*
		@rm -f inc/*~
		@rm -f inc/\#*
		@rm -f *~
		@rm -f \#*
		@rm -f $(PRG)
		@rm -f $(MOBJS) $(GOBJS) $(LOBJS) $(SGOBJ) $(SLOBJ)


#----------------------------------------------------------------
# Dependencies:
#


$(SLOBJ): $(LOBJS)
	@echo "Linking $(subst $(LB)/,,$@) ..."
	@$(LD) $(LDFLAGS) $(SOFLAGS) $(LOBJS) $(GLIBS) $(LIBS) -o $(SLOBJ)

$(SGOBJ): $(GOBJS) $(LOBJS)
	@echo "Linking $(subst $(LB)/,,$@) ..."
	@$(LD) $(LDFLAGS) $(SOFLAGS) $(GOBJS) $(LOBJS) $(GLIBS) $(LIBS) -o $(SGOBJ)

$(MOBJS): $(LB)/%.o: src/%.cxx inc/%.h
	@echo "Compiling $(subst src/,,$<) ..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(GOBJS): $(LB)/%.o: src/%.cxx inc/%.h
	@echo "Compiling $(subst src/,,$<) ..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(LOBJS): $(LB)/%.o: src/%.cxx inc/%.h
	@echo "Compiling $(subst src/,,$<) ..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(PRG): $(SGOBJ) $(SLOBJ) $(MOBJS)
	@echo "Linking $(subst $(BN)/,,$@) ..."
	@$(LD) $(LDFLAGS) $(SPECTRALYZELIBS) $(GLIBS) $(LIBS) -o $(PRG)


#
#----------------------------------------------------------------



