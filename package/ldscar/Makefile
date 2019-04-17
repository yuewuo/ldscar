include $(TOPDIR)/rules.mk

PKG_NAME:=ldscar
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ldscar
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=ldscar -- a library for remote control
	DEPENDS+=libstdcpp
	DEPENDS+=libmosquitto
endef

define Package/ldscar/description
	ldscar,first self-made.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/ldscar/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ldscar $(1)/bin/
endef

$(eval $(call BuildPackage,ldscar))
