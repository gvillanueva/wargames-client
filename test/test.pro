requires(build_test)

GEST_PATH=$${GTEST_PATH}
CONFIG(build_test) {
    isEmpty(GTEST_PATH) {
        error("Please define the qmake variable GTEST_PATH as the path to googletest.")
    }
}

QT -= qt
TEMPLATE = app
TARGET = test

# Create 'check' target in Makefile
CONFIG += testcase

INCLUDEPATH += \
    $$GTEST_PATH \
    $$GTEST_PATH/googletest/include \
    $$GTEST_PATH/googlemock/include \
    ../src

LIBS += \
    -L$$GTEST_PATH/bin -lgoogletest
