#
# Makefile
#
# $Id: //poco/1.6/OSP/Net/Makefile#1 $
#
# Makefile for Net Bundle
#

BUNDLE_TOOL = $(POCO_BASE)/OSP/BundleCreator/$(POCO_HOST_BINDIR)/bundle

include $(POCO_BASE)/build/rules/global

objects = NetBundleActivator

target         = poco.net
target_version = 1
target_libs    = PocoOSP PocoNet PocoUtil PocoXML PocoFoundation

postbuild      = $(BUNDLE_TOOL) -n$(OSNAME) -a$(OSARCH) -o../bundles Net.bndlspec

include $(POCO_BASE)/build/rules/dylib
