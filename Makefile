DOCS_DIR = docs
.PHONY: all say-hello usage help os-arch os-info code-style makedocs

all: say-hello

say-hello:
	@cat $(DOCS_DIR)/intro.txt
	@echo "use <make help>"
	
os-info:
	@cat $(DOCS_DIR)/os-info.txt

usage:
	@cat $(DOCS_DIR)/usage.txt
	
help:
	@cat $(DOCS_DIR)/help.txt

code-style:
	@cat $(DOCS_DIR)/code-style.txt

os-arch:
	@cat $(DOCS_DIR)/os-arch.txt

documentation: 
	doxygen $(DOCS_DIR)/doxygen.cfg
