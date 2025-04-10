SUMMARY = "rubikpi config"
DESCRIPTION = "rubik PI Device Configuration Tool"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRCPROJECT = "git://github.com/rubikpi-ai/tools.git;protocol=https"
SRCBRANCH  = "rubikpi_config"
SRCREV = "d3a2af75f67468f1b530fa94e772a50977f82f19"

SRC_URI =  "${SRCPROJECT};branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

do_install() {
	install -d ${D}${bindir}
	install -d ${D}/etc/rubikpi-config

	install -m 0755 ${S}/rubikpi_config ${D}${bindir}/
	cp -r ${S}/rubikpi_config.ini ${D}/etc/rubikpi-config/rubikpi_config.ini
	cp -r ${S}/rubikpi.dtso ${D}/etc/rubikpi-config/rubikpi.dtso
}

FILES:${PN} += "/etc/rubikpi-config/*"
