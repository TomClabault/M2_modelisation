#include "AppTinyMesh/Source/SDF/SDFDifference.h"
#include "AppTinyMesh/Source/SDF/SDFIntersection.h"
#include "AppTinyMesh/Source/SDF/SDFSmoothDifference.h"
#include "AppTinyMesh/Source/SDF/SDFSmoothUnion.h"
#include "AppTinyMesh/Source/SDF/SphereSDF.h"
#include "AppTinyMesh/Source/SDF/TorusSDF.h"
#include "AppTinyMesh/Source/SDF/sdfunion.h"
#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"

#define SDF_MESHING_SAMPLES 128

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;
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
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->testSDFButton, SIGNAL(clicked()), this, SLOT(TestSDF()));
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
    for (const Ray& ray : rays)
    {
        Vector inter_point = meshColor.intersect(ray);
        SphereSDF* new_sphere = new SphereSDF(inter_point, 0.05f);

        SDFSmoothDifference* new_sdf = new SDFSmoothDifference(m_current_sdf, new_sphere);
        m_current_sdf = new_sdf;
    }


    //Recomputing the mesh
    std::function<float(const Vector& point)> function = std::bind(&SDF::Value, m_current_sdf, std::placeholders::_1);

    Mesh implicitMesh;
    AnalyticScalarField::Polygonize_from_function(function, SDF_MESHING_SAMPLES, implicitMesh, Box(6.0));

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
    AnalyticScalarField::Polygonize_from_function(function, SDF_MESHING_SAMPLES, implicitMesh, Box(6.0));

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
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
