requires(build_test)

CONFIG(build_test) {
    isEmpty(GTEST_ROOT) {
        error("Please define the qmake variable GTEST_ROOT as the path to googletest.")
    }
}

QT -= qt
TEMPLATE = app
TARGET = test

# Create 'check' target in Makefile
CONFIG += testcase

INCLUDEPATH += \
    $$GTEST_ROOT \
    $$GTEST_ROOT/googletest/include \
    $$GTEST_ROOT/googlemock/include \
    ../libwargames

LIBS += \
    -L$$GTEST_ROOT/bin -lgoogletest
