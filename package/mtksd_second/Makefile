include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sdhci-mt7628-second
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/sdhci-mt7628-second
  SUBMENU:=Other modules
  TITLE:=mt7628 second sdcard driver
  FILES:=files/mtk_sd.ko
  AUTOLOAD:=$(call AutoProbe,mtk_sd,1)
  DEPENDS:=+kmod-mmc +kmod-sdhci
  KCONFIG:=\
		CONFIG_MTK_MMC \
		CONFIG_MTK_AEE_KDUMP=n \
		CONFIG_MTK_MMC_CD_POLL=n
endef

define KernelPackage/sdhci-mt7628-second/description
  This is the driver for mt7628 second sdcard interface
endef

define Build/Compile

endef

define KernelPackage/sdhci-mt7628-second/install

endef

$(eval $(call KernelPackage,sdhci-mt7628-second))
