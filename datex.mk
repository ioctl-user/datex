DATEX_INSTALL_STAGING = YES
DATEX_SOURCE = $(notdir $(DATEX_TARBALL))

define DATEX_EXTRACT_CMDS
	cp $(BASE_DIR)/../package/datex/datex.h  $(BASE_DIR)/../package/datex/datex_type.h $(BASE_DIR)/../package/datex/libdatex.c  $(BASE_DIR)/../package/datex/Makefile $(@D)/
endef

define DATEX_BUILD_CMDS
	$(MAKE) CFLAGS="$(TARGET_CFLAGS)" LDFLAGS="$(TARGET_LDFLAGS)" CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all
endef

define DATEX_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libdatex.so $(STAGING_DIR)/usr/lib
	$(INSTALL) -D -m 0644 $(@D)/datex.h $(STAGING_DIR)/usr/include
	$(INSTALL) -D -m 0644 $(@D)/datex_type.h $(STAGING_DIR)/usr/include
endef

define DATEX_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libdatex.so $(TARGET_DIR)/usr/lib
endef

$(eval $(generic-package))
