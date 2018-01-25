# Tools
doxygen?=doxygen
doxymake=$(doxygen) -g

# Input parameters
doc_dir?=doc/
doc_name?=Doxyfile
doc_file?=$(doc_dir)$(doc_name)

DOXY_HTML_EXTRA_STYLESHEET=doc/style.css
DOXY_BUILTIN_STL_SUPPORT?=YES
DOXY_COLS_IN_ALPHA_INDEX?=5
DOXY_DOT_TRANSPARENT?=YES
DOXY_EXCLUDE_PATTERNS?=*.o, *.d, *.h, */obj/*
DOXY_EXTRACT_ALL?=YES
DOXY_EXTRACT_PRIVATE?=YES
DOXY_EXTRACT_PACKAGE?=YES
DOXY_GENERATE_MAN?=YES
DOXY_OUTPUT_DIRECTORY=$(doc_dir)
DOXY_QT_AUTOBRIEF?=YES
DOXY_MULTILINE_CPP_IS_BRIEF=YES
DOXY_RECURSIVE?=YES
DOXY_SHOW_INCLUDE_FILES?=YES
DOXY_STRIP_FROM_PATH?=./

DOXY_WARNINGS=NO

override DOXY_INPUT+=$(DOXY_USE_MDFILE_AS_MAINPAGE) \
	$(wildcard $(foreach T,hpp tpp cpp,\
		$(foreach D,APP SRC HDR,$(DIR_$(D))*.$(T))))

V_DOXY_ALL=$(filter DOXY_%,$(.VARIABLES))
V_DOXY_ALL_SUFFIXES=$(V_DOXY_ALL:DOXY_%=%)
V_DOXY_REPLACEMENT=$(foreach V,$(V_DOXY_ALL_SUFFIXES),\\n$(V) = $(DOXY_$(V)))

$(doc_file): Doxygen.mk $(call TO_OBJ,$(NAMES_LIB) $(NAMES_BIN)) $(DOXY_INPUT)
	@$(doxymake) $(doc_file) >/dev/null && \
		echo $(V_DOXY_REPLACEMENT) >> $(doc_file)

doc: Doxygen.mk $(doc_file)
	@$(doxygen) $(doc_file) >/dev/null
clean-doc:; $(RM) $(doc_file)
.PHONY: doc clean-doc
