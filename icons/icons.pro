TEMPLATE = aux
include(../config.pri)

# sailfish releases < 5.0 don't support the theming setup here
CONFIG(use_svg) {
    svg.files = $$PWD/svgs/icons/*.svg
    svg.path = $$NFCSHARE_UI_DIR
    INSTALLS += svg
} else {
    THEMENAME = sailfish-default
    CONFIG += sailfish-svg2png
}

OTHER_FILES+=$$PWD/svgs/icons/*.svg
