#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/LDS-MINI-64MByte
	NAME:=lds76x8 mini board 64MByte
	PACKAGES:=\
		kmod-usb-core kmod-usb2 kmod-usb-ohci \
		kmod-ledtrig-usbdev kmod-sdhci-mt7620 \
		ap_client maccalc
endef

define Profile/LDS-MINI-64MByte/Description
	Default package set compatible with lds76x8 iot module.
endef
$(eval $(call Profile,LDS-MINI-64MByte))

define Profile/LDS-MINI-128MByte
        NAME:=lds76x8 mini board 128MByte
        PACKAGES:=\
                kmod-usb-core kmod-usb2 kmod-usb-ohci \
                kmod-ledtrig-usbdev kmod-sdhci-mt7620 \
		ap_client maccalc
endef

define Profile/LDS-MINI-128MByte/Description
        Default package set compatible with lds76x8 iot module.
endef
$(eval $(call Profile,LDS-MINI-128MByte))

define Profile/LDS-PRO-64MByte
	NAME:=lds76x8 Pro board 64MByte
	PACKAGES:=\
		kmod-usb-core kmod-usb2 kmod-usb-ohci \
		kmod-ledtrig-usbdev kmod-sdhci-mt7628-second \
		ap_client maccalc
endef

define Profile/LDS-PRO-64MByte/Description
	Default package set compatible with lds76x8 iot module.
endef
$(eval $(call Profile,LDS-PRO-64MByte))

define Profile/LDS-PRO-128MByte
        NAME:=lds76x8 Pro board 128MByte
        PACKAGES:=\
                kmod-usb-core kmod-usb2 kmod-usb-ohci \
                kmod-ledtrig-usbdev kmod-sdhci-mt7628-second \
		ap_client maccalc
endef

define Profile/LDS-PRO-128MByte/Description
        Default package set compatible with lds76x8 iot module.
endef
$(eval $(call Profile,LDS-PRO-128MByte))


