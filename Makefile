# Do not edit this file directly. Configuration is done in Makefile.conf.
# Most configuration here honors existing settings.
include Makefile.conf

DEST_DIR:=
EMPTY:=
SPACE:=$(EMPTY) $(EMPTY)
define BREAK
$1
$(1:%=  )  
endef
BR:=$(call BREAK,\)#) <- Backslashes can confuse highlighters/validaters

DIR_APP:=app/
DIR_SRC:=src/
DIR_HDR:=include/
DIR_SO:=lib/
DIR_O:=obj/
DIR_DEP:=$(DIR_OBJ)

MERGE=$(subst $(SPACE),:,$(sort $1))
DIR_OBJ:=$(call MERGE,$(DIR_SO) $(DIR_O))
vpath %.cpp $(DIR_APP)
vpath %.cpp $(DIR_SRC)
vpath %.hpp $(DIR_HPP)
vpath %.tpp $(DIR_HDR)
vpath %.o $(DIR_O)
vpath %.so $(DIR_SO)

RPATH:=-Wl,-rpath,$(DIR_OBJ)
override CXXFLAGS+=$(strip $(addprefix -I,$(sort $(DIR_HDR))))\
	$(sort $(call CONFIG_O,$(REQ_ALL)))
override LDFLAGS+=$(sort $(call CONFIG_SO,$(REQ_ALL)))
override LDLIBS+=$(RPATH) $(strip $(addprefix -L,$(sort $(DIR_SO) $(DIR_O))))

PAT_APP=$(1:%=$(DIR_APP)%.cpp)
PAT_CPP=$(1:%=$(DIR_SRC)%.cpp)
PAT_HPP=$(1:%=$(DIR_HDR)%.hpp)
PAT_TPP=$(1:%=$(DIR_HDR)%.tpp)
PAT_D=$(1:%=$(DEST_DIR)obj/%.d)
PAT_TD=$(1:%=$(DEST_DIR)obj/%.Td)
PAT_O=$(1:%=obj/%.o)
PAT_SO=$(1:%=lib/lib%.so)
PAT_EXE=$(1:%=$(DEST_DIR)bin/%)
WPAT=$(wildcard $(call PAT_$1,$2))

# Pattern inverses; both paths are taken so that the pipeline is flexible;
# sources and targets can be added/removed/etc. beyond their automatic
# discovery/generation.
UNPAT_APP=$(1:$(DIR_APP)%.cpp=%)
UNPAT_CPP=$(1:$(DIR_SRC)%.cpp=%)
UNPAT_EXE=$(1:$(DEST_DIR)bin/%=%)
UNPAT_SO=$(1:$(DEST_DIR)lib/lib%.so=%)
UNPAT_O=$(1:$(DEST_DIR)obj/%.o=%)

# Row-major: { for(x:$1) for(y:$2) x(y) }
DO_EA=$(foreach L,$1,$(call $L,$2))
# Composed row-major: { ... for(z:$3) x(y(z)) }
DO_EA2=$(call DO_EA,$1,$(call DO_EA,$2,$3))

# Col-major: { for(y:$2) for(x:$1) x(y) }
XDO_EA=$(foreach X,$2,$(call DO_EA,$1,$X))
# Composed col-major: { for(z:$3) ... x(y(z)) }
XDO_EA2=$(foreach X,$(call XDO_EA,$2,$3),$(call DO_EA,$1,$X))

# Maps one file path to another through patterns
REPAT=$(call PAT_$2,$(call UNPAT_$1,$3))
# Maps one file path to several
REPAT_EA=$(call DO_EA2,$(2:%=PAT_%),UNPAT_$1,$3)

# Convenience compositions with wildcard
WREPAT=$(wildcard $(call REPAT,$1,$2,$3))
WDO_EA=$(wildcard $(call DO_EA,$1,$2))
WDO_EA2=$(wildcard $(call DO_EA2,$1,$2,$3))
WREPAT_EA=$(wildcard $(call REPAT_EA,$1,$2,$3))

# Splits one patterned file into a file pair
#   Note - this still allows identities like PAT_EXE/UNPAT_EXE
CROSS=$(foreach X,$4,$(call REPAT,$1,$2,$X):$(call REPAT,$1,$3,$X))
# Identical to 'pkg-config $1 $2' except for '$2'=='';
#   '--silence-errors' and redirection just swap false- / false+.
PKG_CONFIG=$(foreach N,$2,$(shell pkg-config $1 $N))
CONFIG_O=$(call PKG_CONFIG,--cflags,$1)
CONFIG_SO=$(call PKG_CONFIG,--libs,$1)

# Preserve alternate application sources from configuration
override SRCS_EXE+=$(call WPAT,APP,*)
override HDRS_EXE:=$(wildcard $(SRCS_EXE:%.cpp=%.hpp %.tpp))
# Only remove this if app and library byproducts are separated!
#   obj/.o,.d,.Td files will collide until the next Makefile version.
override SRCS_SO:=$(filter-out $(SRCS_EXE),$(SRCS_SO) $(call WPAT,CPP,*))

$(eval NAMES_EXE:=$(call UNPAT_APP,$(SRCS_EXE)))
$(eval NAMES_SO:=$(call UNPAT_CPP,$(SRCS_SO)))
$(eval FILES_EXE:=$(call REPAT,APP,EXE,$(SRCS_EXE)))
$(eval FILES_SO:=$(call REPAT,CPP,SO,$(SRCS_SO)))

TARGET?=$(firstword $(FILES_EXE))
default: $(TARGET) $(COMPLETE)

# Examples - uncomment and make 'vars-TESTS'
#E:=$(NAMES_EXE)
#EA:=$(FILES_EXE)
#EB:=$(SRCS_EXE)
#L:=$(NAMES_SO)
#LA:=$(FILES_SO)
#LB:=$(SRCS_SO)
#$(eval TEST1:=$(call DO_EA,PAT_HPP PAT_CPP,$L))
#$(eval TEST2:=$(call WDO_EA,PAT_HPP PAT_CPP PAT_APP,$L))
#$(eval TEST3:=$(call XDO_EA,PAT_HPP PAT_CPP,$L))
#$(eval TEST4:=$(call DO_EA2,dir notdir,PAT_APP PAT_HPP,$L))
#$(eval TEST5:=$(call XDO_EA2,dir notdir,PAT_APP PAT_HPP,$L))
#$(eval TEST6:=$(join $E,$(join $(EA:%=:%),$(EB))))
#$(eval TEST7:=$(call CROSS,SO,CPP,HPP,$(LA)))
#$(eval TEST8:=$(call REPAT_EA,SO,HPP TPP,$(LA)))
#$(eval TEST9:=$(call WREPAT_EA,SO,HPP TPP,$(LA)))
#$(eval TESTS:=$(foreach N,1 3 6 7 8 9,TEST$N))

$(foreach R,$(addsuffix :,$(call REPAT,APP,D,$(SRCS_EXE)))\
	$(addsuffix :,$(call REPAT,CPP,D,$(SRCS_SO)))\
	$(call CROSS,APP,O,APP,$(SRCS_EXE)) $(call CROSS,SO,O,CPP,$(SRCS_SO)),\
	$(eval $(strip $R)))
$(foreach R,$(call CROSS,APP,EXE,O,$(SRCS_EXE)),$(eval $(strip $R);\
	$(CXX) $(LDLIBS) $(BR) -o $$@ $$< $(LDFLAGS) $(NAMES_SO:%=-l%)))
$(foreach R,$(call CROSS,CPP,SO,O,$(SRCS_SO)),$(eval $(strip $R);\
	$(CXX) $(LDLIBS) $$< -shared $(BR) -o $$@ $(LDFLAGS)))
$(foreach N,$(NAMES_EXE) $(NAMES_SO),\
	$(eval $(call PAT_O,$N): \
	$(wildcard $(call PAT_HPP,$N) $(call WPAT_TPP,$N))))

$(foreach N,$(call REPAT,APP,O,$(SRCS_EXE)),\
	$(eval $N:$(call REPAT_EA,O,APP D,$N) $(FILES_SO);\
	$(CXX) -MT $$@ -MMD -MP -MF $(call REPAT,O,TD,$N) $(CXXFLAGS) -c $$<\
	$(BR)-o $$@ && mv $(call REPAT_EA,O,TD D,$N) && touch $$@))

$(foreach N,$(call REPAT,CPP,O,$(SRCS_SO)),\
	$(eval $N:$(call REPAT_EA,O,CPP D,$N);\
	$(CXX) -MT $$@ -MMD -MP -MF $(call REPAT,O,TD,$N) $(CXXFLAGS)\
	$(BR)-fPIC -c $$< -o $$@\
	$(BR)&& mv $(call REPAT_EA,O,TD D,$N) && touch $$@))

$(COMPLETE): $(FILES_SO) $(FILES_EXE); @echo $(CXXFLAGS) > $@
.PRECIOUS: %.d $(COMPLETE)

#temp.txt: override CXXFLAGS+=$(shell date)
#temp.txt:; @echo $(CXXFLAGS) >>$@

val-%:; @echo $($*)
var-%:; @echo '"$$($*)"="$($*)"'
vars-%:; @$(foreach V,$($*),echo '$V={$($V)}';)
.PHONY: val-% var-% vars-% clean-% clean

# Now clean-main removes bin/main, and clean-bin and clean-EXE are the same
#   - but this is too greedy to run without a --dry-run pass, which outweighs
#   or negates the benefit of convenience for the uninitiated.
clean-%:; @rm -i -f $(wildcard $(filter $*% %$* %$*%,\
	$(foreach V,EXE SO O D TD,$(call PAT_$V,$(NAMES_EXE) $(NAMES_SO))))\
	$(call DO_EA,$(*:%=PAT_%),$(NAMES_EXE) $(NAMES_SO)))
clean: $(addprefix clean-,EXE SO O D COMPLETE)

# Generate auto-dep injection - what could go wrong?
-include $(call PAT_D,$(NAMES_EXE) $(NAMES_SO))
include Doxygen.mk
