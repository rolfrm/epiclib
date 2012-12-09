# Compiler
CC = g++
OPTS = -c -Wall -fPIC -g3 -O3 -std=c++0x
LD = ld
LDOPTS = -r

# Project name
PROJECT = epicGraphics.o

# Directories
OBJDIR = obj
SRCDIR = .

# Libraries

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Targets
#$(PROJECT): buildrepo $(OBJS)
#	$(CC) $(OBJS) $(LIBS) -o $@
$(PROJECT): buildrepo $(OBJS)
	$(LD) $(LDOPTS)  -o $(PROJECT) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(OPTS) -c $< -o $@
	
clean:
	rm $(PROJECT) $(OBJDIR) -Rf
	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
