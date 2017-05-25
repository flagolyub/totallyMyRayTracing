#include <QApplication>

#include "my_project.h"


int main(int argc, char *argv[])
{
      QApplication app(argc, argv);

      SceneView clock;
      clock.show();
      return app.exec();
}
