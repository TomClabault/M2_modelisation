#include "qte.h"
#include <QtWidgets/qapplication.h>

#include "beziercurve.h"
#include "beziersurface.h"

int main(int argc, char *argv[])
{
    /*
    std::vector<Point> points1;
    points1.push_back(Point(0, 0, 0));
    points1.push_back(Point(1, 1, 0));
    points1.push_back(Point(3, 1, 0));

    std::vector<Point> points2;
    points2.push_back(Point(0, 0, 0));
    points2.push_back(Point(1, 1, 0));
    points2.push_back(Point(3, 1, 0));

    std::vector<Point> points3;
    points3.push_back(Point(0, 0, 0));
    points3.push_back(Point(1, 1, 0));
    points3.push_back(Point(3, 1, 0));

    std::vector<BezierCurve> curves;
    curves.push_back(BezierCurve(points1));
    curves.push_back(BezierCurve(points2));
    curves.push_back(BezierCurve(points3));

    BezierSurface surface(curves);
    surface.polygonize(0.1, 0.1);
*/

//    FILE *f = fopen("line.obj", "w");

//        for (int i = 0; i < out_points.size(); i++)
//            fprintf(f, "v %f %f %f\n", out_points[i].x, out_points[i].y, out_points[i].z);

//    fprintf(f, "\nl");
//    for (int i = 1; i <= out_points.size(); i++)
//        fprintf(f, " %d", i);

//    fclose(f);

    //return 0;

	QApplication app(argc, argv);

	MainWindow mainWin;
	mainWin.showMaximized();

	return app.exec();
}
