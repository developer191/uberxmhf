######
# top-level Makefile for UberSpark
# author: amit vasudevan (amitvasudevan@acm.org)
######


include ./uberspark-common.mk


###### targets

.PHONY: all
all: build-tools
	@echo $(USPARK_INSTALL_BINDIR)
	@echo $(USPARK_INSTALL_INCLUDEDIR)
	@echo $(USPARK_INSTALL_HWMDIR)
	@echo $(USPARK_INSTALL_HWMINCLUDEDIR)
	@echo $(USPARK_INSTALL_LIBSDIR)
	@echo $(USPARK_INSTALL_LIBSINCLUDESDIR)
	@echo $(USPARK_INSTALL_TOOLSDIR)


.PHONY: build-tools
build-tools:
	cd $(USPARK_TOOLSDIR) && $(OCAMLC) -o ubersparkconfig unix.cma ubersparkconfig.ml
	cd $(USPARK_TOOLSDIR) && $(RM) umfcommon.annot umfcommon.cmi umfcommon.cmo umfcommon.cmt umfcommon.cmx umfcommon.o
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f umf.mk -w all
	cd $(USPARK_TOOLSDIR) && $(RM) umfcommon.annot umfcommon.cmi umfcommon.cmo umfcommon.cmt umfcommon.cmx umfcommon.o
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f ubp.mk -w all
	cd $(USPARK_TOOLSDIR) && $(RM) umfcommon.annot umfcommon.cmi umfcommon.cmo umfcommon.cmt umfcommon.cmx umfcommon.o
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f uccomp.mk -w all
	cd $(USPARK_TOOLSDIR) && $(RM) umfcommon.annot umfcommon.cmi umfcommon.cmo umfcommon.cmt umfcommon.cmx umfcommon.o
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f ucasm.mk -w all
	cd $(USPARK_TOOLSDIR) && $(RM) umfcommon.annot umfcommon.cmi umfcommon.cmo umfcommon.cmt umfcommon.cmx umfcommon.o
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f uhwm.mk -w all
	

.PHONY: install-tools
install-tools:
	$(MKDIR) -p $(USPARK_INSTALL_TOOLSDIR)
	$(MKDIR) -p $(USPARK_INSTALL_BINDIR)
	$(CP) -f $(USPARK_TOOLSDIR)/ubersparkconfig $(USPARK_INSTALL_BINDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Umf.cmi $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Umf.cmx $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Umf.cmo $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Umf.o $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ubp.cmi $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ubp.cmx $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ubp.cmo $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ubp.o $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uccomp.cmi $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uccomp.cmx $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uccomp.cmo $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uccomp.o $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ucasm.cmi $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ucasm.cmx $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ucasm.cmo $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Ucasm.o $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uhwm.cmi $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uhwm.cmx $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uhwm.cmo $(USPARK_INSTALL_TOOLSDIR)/.
	$(CP) -f $(USPARK_TOOLSDIR)/Uhwm.o $(USPARK_INSTALL_TOOLSDIR)/.


.PHONY: install-hdrs
install-hdrs:
	$(MKDIR) -p $(USPARK_INSTALL_INCLUDEDIR)
	$(CP) -f $(USPARK_INCLUDEDIR)/* $(USPARK_INSTALL_INCLUDEDIR)/.

.PHONY: install-hwm
install-hwm:
	$(MKDIR) -p $(USPARK_INSTALL_HWMDIR)
	$(CP) -rf $(USPARK_HWMDIR)/* $(USPARK_INSTALL_HWMDIR)/.



.PHONY: clean
clean:
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f umf.mk -w clean
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f ubp.mk -w clean
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f uccomp.mk -w clean
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f ucasm.mk -w clean
	cd $(USPARK_TOOLSDIR) && $(MAKE) -f uhwm.mk -w clean
	cd $(USPARK_TOOLSDIR) && $(RM) *_DEP
	cd $(USPARK_TOOLSDIR) && $(RM) ubersparkconfig ubersparkconfig.cmi ubersparkconfig.cmo
	

# http://www.gnu.org/software/automake/manual/automake.html#Clean
.PHONY: distclean
distclean: clean
	$(RM) config.log config.status
	$(RM) uberspark.mk
	$(RM) uberspark-common.mk
	$(RM) $(USPARK_TOOLSDIR)/ubersparkconfig.ml

###### autoconf rules

uberspark.mk: uberspark.mk.in config.status
	./config.status $@

config.status: configure-uberspark
	./config.status --recheck

configure-uberspark: configure-uberspark.ac
	autoconf --output=configure-uberspark configure-uberspark.ac

