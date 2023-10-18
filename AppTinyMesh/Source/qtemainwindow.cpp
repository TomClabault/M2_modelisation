#include "SDFDifference.h"
#include "SDFSmoothDifference.h"
#include "SDFSmoothUnion.h"
#include "SphereSDF.h"
#include "TorusSDF.h"

#include "beziersurface.h"
#include "qte.h"
#include "implicits.h"
#include "revolution.h"
#include "ui_interface.h"

#define SDF_MESHING_SAMPLES 96

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;

    m_circle_radius_pointer = new int;
    m_erosion_rays_count_pointer = new int;
    *m_circle_radius_pointer = 50;
    *m_erosion_rays_count_pointer = 10;
    meshWidget->SetCircleRadiusPointer(m_circle_radius_pointer);
    meshWidget->SetErosionCountPointer(m_erosion_rays_count_pointer);

	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

	meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
    delete m_circle_radius_pointer;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->testSDFButton, SIGNAL(clicked()), this, SLOT(TestSDF()));
    connect(uiw->testBezierButton, SIGNAL(clicked()), this, SLOT(TestBezier()));
    connect(uiw->testRevolutionButton, SIGNAL(clicked()), this, SLOT(TestRevolution()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

	// Widget edition
	connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
    connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
    connect(meshWidget, SIGNAL(_signalPaintErosion(const std::vector<Ray>&)), this, SLOT(paintErosion(const std::vector<Ray>&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
}

void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::paintErosion(const std::vector<Ray>& rays)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const Ray& ray : rays)
    {
        Vector inter_point;
        if (m_intersect_with_ray_marching)
            inter_point = meshColor.intersectRayMarching(m_current_sdf, ray, 0.03f, -1);
        else
            inter_point = meshColor.intersect(ray);

        SphereSDF* new_sphere = nullptr;
        if (inter_point != Vector::Null)
            new_sphere = new SphereSDF(inter_point, m_erosion_sphere_radius);

        if (new_sphere)
        {
            SDFSmoothDifference* new_sdf = new SDFSmoothDifference(m_current_sdf, new_sphere);
            m_current_sdf = new_sdf;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " milliseconds for mesh intersection with " << *m_erosion_rays_count_pointer << " rays using " << (m_intersect_with_ray_marching ? std::string("ray marching strategy") : std::string("standard ray tracing + BVH strategy")) << std::endl;


    //Recomputing the mesh
    std::function<float(const Vector& point)> function = std::bind(&SDF::Value, m_current_sdf, std::placeholders::_1);

    Mesh implicitMesh;
    implicitMesh.set_corresponding_sdf(m_current_sdf);

    start = std::chrono::high_resolution_clock::now();
    AnalyticScalarField::Polygonize_from_function(function, SDF_MESHING_SAMPLES, implicitMesh, Box(6.0));
    stop = std::chrono::high_resolution_clock::now();

    std::cout << "Polygonize time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::BoxMeshExample()
{
	Mesh boxMesh = Mesh(Box(1.0));

	std::vector<Color> cols;
	cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

	meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
    AnalyticScalarField implicit;

    Mesh implicitMesh;
    implicit.Polygonize(31, implicitMesh, Box(2.0));

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::TestSDF()
{
    SphereSDF* sphere1 = new SphereSDF(Vector(1, 0, 0), 2);
    SphereSDF* sphere2 = new SphereSDF(Vector(-3, 0, 0), 2);
    SphereSDF* sphere3 = new SphereSDF(Vector(-1, 3.6f, 0), 2);
    TorusSDF* torus = new TorusSDF(2.0f, 0.75f, RotationX(90) * Translation(-1, 1.75, -1.25));

    SDFSmoothUnion* sdf_two_spheres = new SDFSmoothUnion(sphere1, sphere2, 1.5f);
    SDFSmoothUnion* three_spheres = new SDFSmoothUnion(sdf_two_spheres, sphere3, 1.0f);

    delete m_current_sdf;
    m_current_sdf = new SDFDifference(three_spheres, torus);

    std::function<float(const Vector& point)> function = std::bind(&SDF::Value, m_current_sdf, std::placeholders::_1);

    Mesh implicitMesh;
    implicitMesh.set_corresponding_sdf(m_current_sdf);
    AnalyticScalarField::Polygonize_from_function(function, SDF_MESHING_SAMPLES, implicitMesh, Box(6.0));

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::TestBezier()
{
    std::vector<BezierCurve> curves;
    std::vector<Point> points_curve_1;
    points_curve_1.push_back(Point(0, 0, 1));
    points_curve_1.push_back(Point(0, 1, 2));
    points_curve_1.push_back(Point(0, 2, 3));
    points_curve_1.push_back(Point(0, 3, 3));
    points_curve_1.push_back(Point(0, 4, 2));
    points_curve_1.push_back(Point(0, 5, 1));

    std::vector<Point> points_curve_2;
    points_curve_2.push_back(Point(1, 0, 2));
    points_curve_2.push_back(Point(1, 1, 3));
    points_curve_2.push_back(Point(1, 2, 4));
    points_curve_2.push_back(Point(1, 3, 4));
    points_curve_2.push_back(Point(1, 4, 3));
    points_curve_2.push_back(Point(1, 5, 2));

    std::vector<Point> points_curve_3;
    points_curve_3.push_back(Point(2, 0, 3));
    points_curve_3.push_back(Point(2, 1, 4));
    points_curve_3.push_back(Point(2, 2, 5));
    points_curve_3.push_back(Point(2, 3, 5));
    points_curve_3.push_back(Point(2, 4, 4));
    points_curve_3.push_back(Point(2, 5, 3));

    std::vector<Point> points_curve_4;
    points_curve_4.push_back(Point(3, 0, 3));
    points_curve_4.push_back(Point(3, 1, 4));
    points_curve_4.push_back(Point(3, 2, 5));
    points_curve_4.push_back(Point(3, 3, 5));
    points_curve_4.push_back(Point(3, 4, 4));
    points_curve_4.push_back(Point(3, 5, 3));

    std::vector<Point> points_curve_5;
    points_curve_5.push_back(Point(4, 0, 2));
    points_curve_5.push_back(Point(4, 1, 3));
    points_curve_5.push_back(Point(4, 2, 4));
    points_curve_5.push_back(Point(4, 3, 4));
    points_curve_5.push_back(Point(4, 4, 3));
    points_curve_5.push_back(Point(4, 5, 2));

    std::vector<Point> points_curve_6;
    points_curve_6.push_back(Point(5, 0, 1));
    points_curve_6.push_back(Point(5, 1, 2));
    points_curve_6.push_back(Point(5, 2, 3));
    points_curve_6.push_back(Point(5, 3, 3));
    points_curve_6.push_back(Point(5, 4, 2));
    points_curve_6.push_back(Point(5, 5, 1));

    curves.push_back(BezierCurve(points_curve_1));
    curves.push_back(BezierCurve(points_curve_2));
    curves.push_back(BezierCurve(points_curve_3));
    curves.push_back(BezierCurve(points_curve_4));
    curves.push_back(BezierCurve(points_curve_5));
    curves.push_back(BezierCurve(points_curve_6));

    BezierSurface surface(curves);
    auto start = std::chrono::high_resolution_clock::now();
    Mesh bezier_mesh = surface.polygonize(0.01, 0.01);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Mesh time: " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << " microseconds" << std::endl;

    std::vector<Color> cols;
    cols.resize(bezier_mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    //bezier_mesh.twist(10.0f, 1);
    start = std::chrono::high_resolution_clock::now();
    bezier_mesh.local_attenuated_translation(bezier_mesh.Vertexes() / 2, 1.0f, Vector(0, 0, 1));
    bezier_mesh.local_attenuated_translation(bezier_mesh.Vertexes() / 2, 1.1f, Vector(0.75, 0, 0));
    bezier_mesh.local_attenuated_translation(bezier_mesh.Vertexes() / 5, 0.5f, Vector(0, 0, 0.5));
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Deformations time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

    meshColor = MeshColor(bezier_mesh, cols, bezier_mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::TestRevolution()
{
    std::vector<Point> curve_points;
    curve_points.push_back(Point(0.5, -1, 0));
    curve_points.push_back(Point(1.75, -1, 0));
    curve_points.push_back(Point(0, 3.5, 0));
    curve_points.push_back(Point(1, 3.5, 0));

    BezierCurve curve(curve_points);
    Revolution revolution(curve);

    Mesh revolution_mesh = revolution.polygonize(200, 200, 1);

    std::vector<Color> cols;
    cols.resize(revolution_mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);


    //revolution_mesh.Load("cube_high.obj");
    Vector min_vec = Vector(std::numeric_limits<float>::max()), max_vec = Vector(std::numeric_limits<float>::min());
    for (int i = 0; i < revolution_mesh.Vertexes(); i++)
    {
        min_vec = min(min_vec, revolution_mesh.Vertex(i));
        max_vec = max(max_vec, revolution_mesh.Vertex(i));
    }

    std::cout << "min, max: " << min_vec << ", " << max_vec << std::endl;
    revolution_mesh.twist(10, 1);

    meshColor = MeshColor(revolution_mesh, cols, revolution_mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::UpdateGeometry()
{
	meshWidget->ClearAll();
	meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

	UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}

void MainWindow::on_erosion_sphere_radius_input_textChanged(const QString &erosion_sphere_radius_input)
{
    bool ok;
    float circle_radius = erosion_sphere_radius_input.toFloat(&ok);

    if (ok)
        m_erosion_sphere_radius = circle_radius;
}

void MainWindow::on_erosion_sphere_spread_input_textChanged(const QString &sphere_spread_input)
{
    bool ok;
    float sphere_spread = sphere_spread_input.toInt(&ok);

    if (ok)
        *m_circle_radius_pointer = sphere_spread;
}

void MainWindow::on_erosion_count_input_textChanged(const QString &erosion_count_input)
{
    bool ok;
    float rays_count = erosion_count_input.toInt(&ok);

    if (ok)
        *m_erosion_rays_count_pointer = rays_count;
}

void MainWindow::on_use_ray_marching_checkbox_stateChanged(int arg1)
{
    m_intersect_with_ray_marching = (bool)arg1;
}

void MainWindow::on_save_obj_button_clicked()
{
    meshColor.SaveObj("saved_obj.obj", "implicit_obj");
}
