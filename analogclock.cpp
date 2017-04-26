
#include <QtWidgets>
#include <memory>

#include "scene.h"
#include "geom.h"
#include "light.h"

  #include "analogclock.h"

  SceneView::SceneView(QWidget *parent)
      : QWidget(parent)
  {
      setWindowTitle(tr("RayTracing"));
      resize(800, 600);
  }

  void SceneView::paintEvent(QPaintEvent *)
  {
      setAttribute(Qt::WA_OpaquePaintEvent);
      QPainter painter(this);
      QPen linepen(Qt::red);
      linepen.setCapStyle(Qt::RoundCap);
      linepen.setWidth(5);
      painter.setRenderHint(QPainter::Antialiasing,true);
      painter.setPen(linepen);

      std::vector<GeomObj*> obj;
      std::vector<LightSource*> lig;
      /*ThreeDVector center(5.0, 5.0, 5.0);
      RGBColor sphcol(10.0, 3.0, 12.0);
      auto sph = new Sphere(center, 3.0, sphcol);
      obj.push_back(sph);
      ThreeDVector ul(15.0, 0.0, 10.0);
      ThreeDVector ur(15.0, 20.0, 10.0);
      ThreeDVector dl(15.0, 0.0, 0.0);
      ThreeDVector eye(30.0, 10.0, 5.0);
      Scene scene(obj, ul, ur, dl, eye, 200, 100);*/
      ThreeDVector ul(0, 0, 40.0);
      ThreeDVector ur(50, 0, 40.0);
      ThreeDVector dl(0, 0, 0);
      ThreeDVector eye(5, -50, 10);
      auto s1 = new Sphere(ThreeDVector(20, 20, 20), 10, RGBColor(255, 0, 0));
      auto s2 = new Sphere(ThreeDVector(40, 40, 40), 10, RGBColor(0, 0, 255));
      auto tri1 = new Triangle(ThreeDVector(20, 20, 20), ThreeDVector(40, 40, 40), ThreeDVector(30, 50, 50), RGBColor(209, 11, 75));
      auto candle = new LightSource(1000, ThreeDVector(20, 20, 50)); //20 20 50
      //auto candle2 = new PointLightSource(ThreeDVector(25, -20, 20), 70000); //20 20 50
      obj.push_back(s1);
      obj.push_back(s2);
      obj.push_back(tri1);
      lig.push_back(candle);
      //lig.push_back(candle2);
      Scene scene(lig, obj, ul, ur, dl, eye, 800, 600);



      auto points = scene.process();
      painter.drawPoint(10 * (1 + points.size()), 10);
        for(int i = 0; i < points.size(); i++){
            QPen linepen(QColor(points[i].second.r, points[i].second.g, points[i].second.b, 255));
            linepen.setCapStyle(Qt::RoundCap);
            linepen.setWidth(1);
            painter.setRenderHint(QPainter::Antialiasing,true);
            painter.setPen(linepen);
          painter.drawPoint(points[i].first.x, points[i].first.y);
        }
    //for(int i = 0; i < points.size(); i++)
    //    painter.drawPoint(points[i].first.x, points[i].first.y);
}
