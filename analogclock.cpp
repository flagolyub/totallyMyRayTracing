#include <QtWidgets>
#include <memory>

#include "scene.h"
#include "geom.h"

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

      std::vector<PointLightSource*> lig;
      std::vector<GeomObj*> obj;
      /*
      Point center(5.0, 2.0, 5.0);
      Color sphcol(255, 216, 0);
      auto sph = new Sphere(center, 3.0, sphcol);
      auto sph1 = new Sphere(Point(5, 3, 10), 3.0, sphcol);
      auto tri1 = new Triangle(Point(5, 5, 10), Point(5, 0, 5), Point(5, 10, 5));
      obj.push_back(tri1);
      obj.push_back(sph);
      obj.push_back(sph1);
      auto quad = new Quadrilateral(Point(5, 0, 0), Point(5, 20, 0), Point(0, 20, 5), Point(0, 0, 5), Color(50, 170, 10));
      obj.push_back(quad);
      */

      // Для сфер
      Point ul(0, 0, 60.0);
      Point ur(80, 0, 60.0);
      Point dl(0, 0, 0);
      Point eye(40, -100, 30); //5 -30 15


      /*
      Point ul(30, 0, 40.0);
      Point ur(30, 40, 40.0);
      Point dl(30, 0, 0);
      Point eye(50, 20, 20);
*/

      //PointLightSource candle(Point(10, 15, 5), 10000);
      //lig.push_back(&candle);

      /*
      Quadrilateral base(Point(30, 50, 30), Point(80, 50, 30), Point(80, 100, 30), Point(30, 100, 30), Color(75, 0, 130));
      obj.push_back(&base);
      Quadrilateral side(Point(40, 60, 30), Point(70, 60, 30), Point(40, 60, 40), Point(70, 60, 40), Color(75, 0, 130));
      obj.push_back(&side);
      obj.push_back(&base);
      Triangle edge(Point(30, 50, 30), Point(80, 50, 30), Point(55, 47, 30), Color(75, 0, 130));
      obj.push_back(&edge);
      Triangle edge2(Point(80, 50, 30), Point(80, 100, 30), Point(83, 125, 30), Color(75, 0, 130));
      Triangle edge3(Point(80, 100, 30), Point(30, 100, 30), Point(55, 103, 30), Color(75, 0, 130));
      Triangle edge4(Point(30, 100, 30), Point(30, 50, 30), Point(27, 75, 30), Color(75, 0, 130));
      obj.push_back(&edge2);
      obj.push_back(&edge3);
      obj.push_back(&edge4);
      Sphere cap(Point(55, 75, 45), 10, Color(255, 216, 0));
      obj.push_back(&cap);
      */


      auto s1 = new Sphere(Point(50, 50, 10), 10, Color(255, 216, 0));
      auto s2 = new Sphere(Point(30, 30, 10), 10, Color(216, 100, 20));
      //auto s3 = new Sphere(Point(50, 50, 50), 10, Color(255, 100, 20));
      //auto s5 = new Sphere(Point(10, 10, 10), 10, Color(216, 10, 20));
      //auto s4 = new Sphere(Point(30, 30, 30), 10, Color(216, 100, 2));
      //auto tri1 = new Triangle(Point(0, 20, 20), Point(50, 0, 50), Point(30, 50, 50), Color(209, 11, 75));
      auto floor = new Quadrilateral(Point(0, 0, 0), Point(80, 0, 0), Point(80, 60, 0), Point(0, 60, 0), Color(0, 255, 0));
      auto forward = new Quadrilateral(Point(0, 60, 60), Point(80, 60, 60), Point(80, 60, 0), Point(0, 60, 0), Color(0, 255, 0));
      auto left = new Quadrilateral(Point(0, 0, 0), Point(0, 60, 0), Point(0, 60, 60), Point(0, 0, 60), Color(0, 255, 0));
      auto right = new Quadrilateral(Point(80, 0, 0), Point(80, 60, 0), Point(80, 60, 60), Point(80, 0, 60), Color(0, 255, 0));
      auto roof = new Quadrilateral(Point(0, 0, 60), Point(80, 0, 60), Point(80, 60, 60), Point(0, 60, 60), Color(0, 255, 0));


      auto candle = new PointLightSource(Point(20, 20, 55), 70000); //20 20 50
      auto candle2 = new PointLightSource(Point(30, 30, 30), 70000); //20 20 50
      auto candle3 = new PointLightSource(Point(70, 50, 50), 10000); //20 20 50

      //auto quad = new Quadrilateral(Point(0, 0, 30), Point(40, 0, 30), Point(40, 40, 50), Point(0, 40, 50), Color(50, 170, 10));

      //obj.push_back(quad);

      obj.push_back(s1);
      obj.push_back(s2);
      //obj.push_back(s3);
      //obj.push_back(s4);
      obj.push_back(floor);
      obj.push_back(forward);
      //obj.push_back(left);
      //obj.push_back(right);
      //obj.push_back(roof);

      //obj.push_back(tri1);
      lig.push_back(candle);
      lig.push_back(candle2);
      lig.push_back(candle3);
      Scene scene(obj, lig, ul, ur, dl, eye, 800, 600);

      auto points = scene.process();
      //painter.drawPoint(10 * (points.size()), 10);
      for(size_t i = 0; i < points.size(); i++){
          QPen linepen(QColor(points[i].second.r, points[i].second.g, points[i].second.b, 255));
          linepen.setCapStyle(Qt::RoundCap);
          linepen.setWidth(1);
          painter.setRenderHint(QPainter::Antialiasing,true);
          painter.setPen(linepen);
        painter.drawPoint(points[i].first.x, points[i].first.y);
      }
  }
