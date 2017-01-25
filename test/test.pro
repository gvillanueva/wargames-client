isEmpty(GTEST_ROOT) {
    warning("Skipping $$_PRO_FILE_, qmake variable GTEST_ROOT must contain path to googletest")
}
else {
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
