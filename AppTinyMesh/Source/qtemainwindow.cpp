#include "SDFDifference.h"
#include "SDFSmoothDifference.h"
#include "SDFSmoothUnion.h"
#include "SphereSDF.h"
#include "TorusSDF.h"

#include "beziersurface.h"
#include "qte.h"
#include "implicits.h"
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
    std::vector<Point> points1;
    points1.push_back(Point(0, 0, 0));
    points1.push_back(Point(1, 1, 0));
    points1.push_back(Point(3, 1, 0));

    std::vector<Point> points2;
    points2.push_back(Point(0, 0, 1.5));
    points2.push_back(Point(1, 1, 1.5));
    points2.push_back(Point(3, 1, 1.5));

    std::vector<Point> points3;
    points3.push_back(Point(0, 0, 3));
    points3.push_back(Point(1, 1, 3));
    points3.push_back(Point(3, 1, 3));

    curves.push_back(BezierCurve(points1));
    curves.push_back(BezierCurve(points2));
    curves.push_back(BezierCurve(points3));

    BezierSurface surface(curves);
    Mesh bezier_mesh = surface.polygonize(0.1, 0.1);

    std::vector<Color> cols;
    cols.resize(bezier_mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(bezier_mesh, cols, bezier_mesh.VertexIndexes());
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
    std::cout << "new state: " << arg1 << std::endl;
    m_intersect_with_ray_marching = (bool)arg1;

    std::cout << "m_intersect: " << m_intersect_with_ray_marching << std::endl;
}

void MainWindow::on_save_obj_button_clicked()
{
    meshColor.SaveObj("saved_obj.obj", "implicit_obj");
}
