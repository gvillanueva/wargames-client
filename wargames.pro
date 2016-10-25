TEMPLATE = subdirs

SUBDIRS += \
    wargames \
    test \
    libwargames

test.depends = wargames libwargames
wargames.depends = libwargames
