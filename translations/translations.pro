TEMPLATE = aux

TS_FILE = $$OUT_PWD/nfcshare.ts
ENG_EN_QM = $$OUT_PWD/nfcshare_eng_en.qm

ts.commands += lupdate "$$_PRO_FILE_PWD_/.." -ts "$$TS_FILE"
ts.output = $$TS_FILE
ts.input = ..

ts_install.files = $$TS_FILE
ts_install.path = $$[QT_INSTALL_PREFIX]/share/translations/source
ts_install.CONFIG += no_check_exist

engineering_english.commands += lrelease -idbased "$$TS_FILE" -qm "$$ENG_EN_QM"
engineering_english.depends = ts
engineering_english.input = $$TS_FILE
engineering_english.output = $$ENG_EN_QM

TRANSLATIONS_PATH = $$[QT_INSTALL_PREFIX]/share/translations
engineering_english_install.path = $$TRANSLATIONS_PATH
engineering_english_install.files = $$ENG_EN_QM
engineering_english_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += ts engineering_english
PRE_TARGETDEPS += ts engineering_english
INSTALLS += ts_install engineering_english_install
