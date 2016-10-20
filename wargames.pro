TEMPLATE = subdirs

SUBDIRS += \
    wargames \
    test

test.depends = wargames
