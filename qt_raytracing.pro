 QT += widgets

  HEADERS       = \
    point.h\
    color.h\
    geom.h \
    scene.h \
    light.h \
    point.h \
    color.h \
    my_project.h
  SOURCES       = \
                  main.cpp \
    my_project.cpp

  QMAKE_PROJECT_NAME = RayTracing

  # install
  target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
  INSTALLS += target
