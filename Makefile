#############################################################################
# Makefile for building: Designer7
# Generated by qmake (3.1) (Qt 5.9.2)
# Project:  Designer7.pro
# Template: app
# Command: D:\tools\Qt\5.9.2\mingw53_32\bin\qmake.exe -o Makefile Designer7.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
#############################################################################

MAKEFILE      = Makefile

first: debug
install: debug-install
uninstall: debug-uninstall
QMAKE         = D:\tools\Qt\5.9.2\mingw53_32\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = copy /y
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = copy /y
INSTALL_PROGRAM = copy /y
INSTALL_DIR   = xcopy /s /q /y /i
QINSTALL      = D:\tools\Qt\5.9.2\mingw53_32\bin\qmake.exe -install qinstall
QINSTALL_PROGRAM = D:\tools\Qt\5.9.2\mingw53_32\bin\qmake.exe -install qinstall -exe
DEL_FILE      = del
SYMLINK       = $(QMAKE) -install ln -f -s
DEL_DIR       = rmdir
MOVE          = move
SUBTARGETS    =  \
		debug \
		release


debug: FORCE
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Debug 
debug-all: FORCE
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: FORCE
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Debug uninstall
release: FORCE
	$(MAKE) -f $(MAKEFILE).Release
release-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Release 
release-all: FORCE
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: FORCE
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Release uninstall

Makefile: Designer7.pro ../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/win32-g++/qmake.conf ../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/spec_pre.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/qdevice.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/device_config.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/g++-base.conf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/angle.conf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/sanitize.conf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/gcc-base.conf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/qconfig.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3danimation.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3danimation_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dcore.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dcore_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dextras.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dextras_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dinput.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dinput_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dlogic.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dlogic_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquick.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquick_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickanimation.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickanimation_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickextras.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickextras_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickinput.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickinput_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickrender.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickrender_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickscene2d.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickscene2d_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3drender.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3drender_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_accessibility_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axbase.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axbase_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axcontainer.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axcontainer_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axserver.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axserver_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_bluetooth.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_bluetooth_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_bootstrap_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_charts.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_charts_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_concurrent.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_concurrent_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_core.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_core_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_datavisualization.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_datavisualization_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_dbus.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_dbus_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_designer.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_designer_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_designercomponents_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_devicediscovery_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_egl_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_eventdispatcher_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_fb_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_fontdatabase_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gamepad.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gamepad_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gui.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gui_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_help.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_help_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_location.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_location_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimedia.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimedia_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimediawidgets.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimediawidgets_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_network.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_network_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_networkauth.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_networkauth_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_nfc.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_nfc_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_opengl.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_opengl_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_openglextensions.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_openglextensions_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_packetprotocol_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_platformcompositor_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_positioning.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_positioning_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_printsupport.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_printsupport_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_purchasing.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_purchasing_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qml.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qml_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmldebug_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmltest.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmltest_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quick.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quick_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickcontrols2.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickcontrols2_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickparticles_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quicktemplates2_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickwidgets.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_remoteobjects.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_remoteobjects_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_repparser.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_repparser_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_script.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_script_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scripttools.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scripttools_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scxml.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scxml_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sensors.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sensors_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialbus.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialbus_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialport.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialport_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sql.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sql_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_svg.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_svg_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_testlib.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_testlib_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_texttospeech.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_texttospeech_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_theme_support_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_uiplugin.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_uitools.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_uitools_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_webchannel.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_webchannel_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_websockets.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_websockets_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_widgets.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_widgets_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_winextras.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_winextras_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xml.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xml_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xmlpatterns.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qt_functions.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qt_config.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/win32-g++/qmake.conf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/spec_post.prf \
		.qmake.stash \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/exclusive_builds.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/toolchain.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/default_pre.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/win32/default_pre.prf \
		../../../../14-03\ Framework/framework/fmkQt.pri \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/resolve_config.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/exclusive_builds_post.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/default_post.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qml_debug.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/precompile_header.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/warn_on.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qt.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/resources.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/moc.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/win32/opengl.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/uic.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qmake_use.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/file_copies.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/win32/windows.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/testcase_targets.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/exceptions.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/yacc.prf \
		../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/lex.prf \
		Designer7.pro \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/qtmaind.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5PrintSupport.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5UiTools.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Widgets.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Gui.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Xml.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Network.prl \
		../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Core.prl
	$(QMAKE) -o Makefile Designer7.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/spec_pre.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/qdevice.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/device_config.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/g++-base.conf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/angle.conf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/sanitize.conf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/common/gcc-base.conf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/qconfig.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3danimation.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3danimation_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dcore.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dcore_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dextras.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dextras_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dinput.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dinput_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dlogic.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dlogic_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquick.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquick_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickanimation.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickanimation_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickextras.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickextras_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickinput.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickinput_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickrender.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickrender_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickscene2d.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3dquickscene2d_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3drender.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_3drender_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_accessibility_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axbase.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axbase_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axcontainer.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axcontainer_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axserver.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_axserver_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_bluetooth.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_bluetooth_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_bootstrap_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_charts.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_charts_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_concurrent.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_concurrent_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_core.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_core_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_datavisualization.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_datavisualization_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_dbus.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_dbus_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_designer.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_designer_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_designercomponents_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_devicediscovery_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_egl_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_eventdispatcher_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_fb_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_fontdatabase_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gamepad.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gamepad_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gui.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_gui_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_help.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_help_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_location.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_location_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimedia.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimedia_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimediawidgets.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_multimediawidgets_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_network.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_network_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_networkauth.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_networkauth_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_nfc.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_nfc_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_opengl.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_opengl_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_openglextensions.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_openglextensions_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_packetprotocol_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_platformcompositor_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_positioning.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_positioning_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_printsupport.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_printsupport_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_purchasing.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_purchasing_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qml.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qml_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmldebug_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmldevtools_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmltest.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qmltest_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quick.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quick_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickcontrols2.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickcontrols2_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickparticles_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quicktemplates2_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickwidgets.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_quickwidgets_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_remoteobjects.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_remoteobjects_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_repparser.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_repparser_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_script.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_script_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scripttools.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scripttools_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scxml.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_scxml_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sensors.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sensors_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialbus.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialbus_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialport.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_serialport_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sql.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_sql_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_svg.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_svg_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_testlib.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_testlib_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_texttospeech.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_texttospeech_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_theme_support_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_uiplugin.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_uitools.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_uitools_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_webchannel.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_webchannel_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_websockets.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_websockets_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_widgets.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_widgets_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_winextras.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_winextras_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xml.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xml_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xmlpatterns.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/modules/qt_lib_xmlpatterns_private.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qt_functions.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qt_config.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/win32-g++/qmake.conf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/spec_post.prf:
.qmake.stash:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/exclusive_builds.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/toolchain.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/default_pre.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/win32/default_pre.prf:
../../../../14-03\ Framework/framework/fmkQt.pri:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/resolve_config.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/exclusive_builds_post.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/default_post.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qml_debug.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/precompile_header.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/warn_on.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qt.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/resources.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/moc.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/win32/opengl.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/uic.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/qmake_use.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/file_copies.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/win32/windows.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/testcase_targets.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/exceptions.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/yacc.prf:
../../../../tools/Qt/5.9.2/mingw53_32/mkspecs/features/lex.prf:
Designer7.pro:
../../../../tools/Qt/5.9.2/mingw53_32/lib/qtmaind.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5PrintSupport.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5UiTools.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Widgets.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Gui.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Xml.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Network.prl:
../../../../tools/Qt/5.9.2/mingw53_32/lib/Qt5Core.prl:
qmake: FORCE
	@$(QMAKE) -o Makefile Designer7.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"

qmake_all: FORCE

make_first: debug-make_first release-make_first  FORCE
all: debug-all release-all  FORCE
clean: debug-clean release-clean  FORCE
distclean: debug-distclean release-distclean  FORCE
	-$(DEL_FILE) Makefile
	-$(DEL_FILE) .qmake.stash

debug-mocclean:
	$(MAKE) -f $(MAKEFILE).Debug mocclean
release-mocclean:
	$(MAKE) -f $(MAKEFILE).Release mocclean
mocclean: debug-mocclean release-mocclean

debug-mocables:
	$(MAKE) -f $(MAKEFILE).Debug mocables
release-mocables:
	$(MAKE) -f $(MAKEFILE).Release mocables
mocables: debug-mocables release-mocables

check: first

benchmark: first
FORCE:

$(MAKEFILE).Debug: Makefile
$(MAKEFILE).Release: Makefile