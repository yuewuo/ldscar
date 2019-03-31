include $(TOPDIR)/rules.mk

PKG_NAME:=ap_client
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ap_client
  CATEGORY:=Ralink Wireless Support
  TITLE:=MT7628/88 STA  Wifi
  DEPENDS:=@TARGET_ramips_mt7628 +wireless-tools +libuci
  SUBMENU:=Applications
  ABI_VERSION:=$(PKG_RELEASE)
endef

define Package/ap_client/description

endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/ap_client/install
	$(INSTALL_DIR) $(1)/usr/sbin

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ap_client $(1)/usr/sbin/

endef

$(eval $(call BuildPackage,ap_client))



