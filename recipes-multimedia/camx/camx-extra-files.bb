SUMMARY = "Copy CAMX related"
LICENSE = "CLOSED"

FILESPATH =+ "${WORKSPACE}/layers/meta-rubikpi/recipes-multimedia/camx:"
SRC_URI = "file://files/"
SRC_DIR = "${THISDIR}"

do_install() {
	install -d ${D}${sysconfdir}/camera

	install -m 0644 ${WORKDIR}/files/camxoverridesettings.txt ${D}${sysconfdir}/camera/camxoverridesettings.txt
}

FILES:${PN} += "${sysconfdir}/camera/camxoverridesettings.txt"
