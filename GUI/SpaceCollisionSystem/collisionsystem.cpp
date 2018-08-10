#include "collisionsystem.h"
#include "ui_collisionsystem.h"
#include "collisiondisplay.h"
#include "collisiondisplay2d.h"
#include "collisiondisplay3d.h"
#include "bruteforcealgorithm.h"
#include "quadtreealgorithm.h"
#include "octreealgorithm.h"
#include "sweepnprunealgorithmsimple.h"
#include "sweepnprunealgorithmmulti.h"
#include "constants.h"
#include "spatialhashalgorithm.h"
#include "spatialhashalgorithm3d.h"
#include "spatialindexalgorithm.h"
#include "spatialindexalgorithm3d.h"
#include <QWindow>

CollisionSystem::CollisionSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CollisionSystem)
{
    ui->setupUi(this);

    // place the 2D collision display in the stacket widget
    ui->stackedWidget->addWidget(ui->collisionDisplay2D);

    // create 3D collision display and place it in the stacked wigdet
    collisionDisplay3D = new CollisionDisplay3D();
    collisionDisplay3D->resize(X_DIMENSION_SIZE, Y_DIMENSION_SIZE);
    collisionDisplay3D->camera()->setCenter(QVector3D(X_DIMENSION_SIZE/2, Y_DIMENSION_SIZE/2, Z_DIMENSION_SIZE/2));
    resetCamera();
    collisionDisplay3D->show();
    display3DContainer = QWidget::createWindowContainer((QGLView*)collisionDisplay3D);
    ui->stackedWidget->addWidget(display3DContainer);

    // set default display to 2D
    ui->stackedWidget->setCurrentWidget(ui->collisionDisplay2D);

    algThread = new QThread;
    qalg = NULL;
    paused = false;
    algRunning = false;
    displayRunning = false;
    environment = 0;
    closing = NULL;

    initializeConnections();
}

CollisionSystem::~CollisionSystem()
{
    delete ui;
    delete collisionDisplay3D;
}

void CollisionSystem::
initializeConnections() {
    // connect the input for number of particles with the setter
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start()));

    // connect switching between 2D and 3D to appropriate changes
    connect(ui->dimensionSelector, SIGNAL(currentChanged(int)), this, SLOT(setSpace(int)));

    // connect generate seed button to seed input box
    connect(ui->generateSeedButton, SIGNAL(clicked()), this, SLOT(generateSeed()));

    // connect lookahead time value changing to set lookahead time in real time
    ui->lookaheadTimeInput->setMaximum(MAX_LOOKAHEAD_TIME);
    connect(ui->lookaheadTimeInput, SIGNAL(valueChanged(double)), this, SLOT(setLookaheadTime(double)));

    // connect radius range changes to appropriate min or max range change
    connect(ui->minRadiusBox, SIGNAL(valueChanged(double)), this, SLOT(setMaxRadiusMin(double)));
    connect(ui->maxRadiusBox, SIGNAL(valueChanged(double)), this, SLOT(setMinRadiusMax(double)));

    // connect and disable pause button
    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(changePausedState()));
    ui->pauseButton->setEnabled(false);

    // connect the timestep limit checkbox with the input
    connect(ui->timestepLimitCheckbox, SIGNAL(toggled(bool)), this, SLOT(setTimestepLimitEnabled(bool)));
    // set default value and disable the field
    ui->timestepLimitInput->setMinimum(0);
    ui->timestepLimitInput->setMaximum(99999);
    ui->timestepLimitInput->setValue(600);
    ui->timestepLimitInput->setEnabled(false);
    // connect timestep limit input with variable
    connect(ui->timestepLimitInput, SIGNAL(valueChanged(int)), this, SLOT(setTimestepLimit(int)));

    // build algorithm dropdown
    QStringList algorithms;
    algorithms << "Brute Force" << "Quadtree" << "Spatial Hash" << "Spatial Index" << "Sweep and Prune" << "Sweep Multi";
    ui->algorithmSelector2D->addItems(algorithms);
    QStringList algorithms3D;
    algorithms3D << "Brute Force" << "Octree" << "Spatial Hash" << "Spatial Index" << "Sweep and Prune" << "Sweep Multi";
    ui->algorithmSelector3D->addItems(algorithms3D);
    QStringList algorithmsOrbital;
    algorithmsOrbital << "Brute Force" << "Octree" << "Spatial Hash" << "Sweep and Prune" << "Sweep Multi";
    ui->algorithmSelectorOrbital->addItems(algorithmsOrbital);

    // create initial RNG seed
    ui->seedInput->setText(QString::number(SEED));

    // set range for and initial number of particles
    ui->numParticlesInput->setMinimum(MIN_NUM_PARTICLES);
    ui->numParticlesInput->setMaximum(MAX_NUM_PARTICLES);
    ui->numParticlesInput->setValue(NUM_PARTICLES);

    // set initial min and max particle radius boxes
    ui->minRadiusBox->setMinimum(0.1);
    ui->minRadiusBox->setMaximum(MAX_PARTICLE_RADIUS);
    ui->minRadiusBox->setValue(MIN_PARTICLE_RADIUS);
    ui->minRadiusBox->setSingleStep(0.25);
    ui->maxRadiusBox->setMinimum(MIN_PARTICLE_RADIUS);
    ui->maxRadiusBox->setMaximum(6);
    ui->maxRadiusBox->setValue(MAX_PARTICLE_RADIUS);
    ui->maxRadiusBox->setSingleStep(0.25);

    // set step size and initial displayed look ahead time
    ui->lookaheadTimeInput->setSingleStep(0.25);
    ui->lookaheadTimeInput->setValue(LOOKAHEAD_TIME);

    // set range and starting value for framerate slider
    ui->framerateSlider->setMinimum(MIN_FRAMERATE);
    ui->framerateSlider->setMaximum(MAX_FRAMERATE);
    ui->framerateSlider->setValue(FRAMERATE);
    // connect framerate slider to change framerate in real time
    connect(ui->framerateSlider, SIGNAL(valueChanged(int)), this, SLOT(setTargetFramerate(int)));

    // connect reset camera button
    connect(ui->resetCameraButton, SIGNAL(clicked()), this, SLOT(resetCamera()));

    // connect output timer to output box
    connect(&outputTimer, SIGNAL(timeout()), this, SLOT(printInfo()));

    // set up shortcuts
    pauseShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(changePausedState()));
    startShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, SLOT(start()));
    stopShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), ui->stopButton, SIGNAL(clicked()));

    // disable buttons and shortcuts that shouldn't be clicked now
    ui->stepButton->setEnabled(false);
    ui->resetCameraButton->setEnabled(false);
    pauseShortcut->setEnabled(false);
}

void CollisionSystem::closeEvent(QCloseEvent* event) {
    closing = event;
    bool noClose = false;
    if (algRunning) {
        noClose = true;
        qalg->stop();
    }
    if (displayRunning) {
        noClose = true;
        if (environment == 0) {
            ui->collisionDisplay2D->stop();
        }
    else {
            collisionDisplay3D->stop();
        }
    }

    if (noClose) {
        return;
    }

    delete algThread;

    event->accept();
}

void CollisionSystem::setNumParticles(){
    NUM_PARTICLES = ui->numParticlesInput->value();
    MAX_QUEUE_SIZE = QUEUE_SIZE_MULTIPLIER * NUM_PARTICLES;
}

void CollisionSystem::setSeed() {
    SEED = ui->seedInput->text().toInt();
}

void CollisionSystem::setRadiusRange() {
    MIN_PARTICLE_RADIUS = ui->minRadiusBox->value();
    MAX_PARTICLE_RADIUS = ui->maxRadiusBox->value();
}

// SLOTS - fancy things
void CollisionSystem::start() {
    // set initial parameters
    setSeed();
    setNumParticles();
    setRadiusRange();
    setLookaheadTime(ui->lookaheadTimeInput->value());
    setTargetFramerate(FRAMERATE);

    // create algorithm
    if (environment == 0)
    {
        int index = ui->algorithmSelector2D->currentIndex();
        switch (index) {
        case 0:
            qalg = new QAlgorithm(new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 1:
            qalg = new QAlgorithm(new QuadtreeAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 2:
            qalg = new QAlgorithm(new SpatialHashAlgorithm(X_DIMENSION_SIZE,Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 3:
            qalg = new QAlgorithm(new SpatialIndexAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 4:
            qalg = new QAlgorithm(new SweepNPruneAlgorithmSimple(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 5:
            qalg = new QAlgorithm(new SweepNPruneAlgorithmMulti(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        default:
            qalg = new QAlgorithm(new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        }
    }
    else if (environment == 1)
    {
        int index = ui->algorithmSelector3D->currentIndex();
        switch (index) {
        case 0:
            qalg = new QAlgorithm(new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 1:
            qalg = new QAlgorithm(new OctreeAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 2:
            qalg = new QAlgorithm(new SpatialHashAlgorithm3D(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 3:
            qalg = new QAlgorithm(new SpatialIndexAlgorithm3D(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 4:
            qalg = new QAlgorithm(new SweepNPruneAlgorithmSimple(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        case 5:
            qalg = new QAlgorithm(new SweepNPruneAlgorithmMulti(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        default:
            qalg = new QAlgorithm(new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES));
            break;
        }
    }
    else
    {
        int index = ui->algorithmSelectorOrbital->currentIndex();
        switch (index) {
        case 0:
            qalg = new QAlgorithm(new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES, 1000.0));
            break;
        case 1:
            qalg = new QAlgorithm(new OctreeAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, 1000.0, NUM_PARTICLES));
            break;
        case 2:
            qalg = new QAlgorithm(new SpatialHashAlgorithm3D(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES, 1000.0));
            break;
        case 3:
            qalg = new QAlgorithm(new SweepNPruneAlgorithmSimple(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, 1000.0, NUM_PARTICLES));
            break;
        case 4:
            qalg = new QAlgorithm(new SweepNPruneAlgorithmMulti(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, 1000.0, NUM_PARTICLES));
            break;
        default:
            qalg = new QAlgorithm(new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, NUM_PARTICLES, 1000.0));
            break;
        }
    }

    // move the algorithm to its own thread
    qalg->moveToThread(algThread);

    if (ui->dimensionSelector->currentIndex() == 0) {
        ui->collisionDisplay2D->setQAlgorithm(qalg);

        connect(ui->stopButton, SIGNAL(clicked()), ui->collisionDisplay2D, SLOT(stop()), Qt::DirectConnection);

        // connect timer to display advancement and info panel update
        connect(ui->collisionDisplay2D->getTimer(), SIGNAL(timeout()), ui->collisionDisplay2D, SLOT(step()));
        connect(ui->collisionDisplay2D->getTimer(), SIGNAL(timeout()), ui->collisionDisplay2D->getScene(), SLOT(advance()));

        // connect step button to display advancement
        connect(ui->stepButton, SIGNAL(clicked()), ui->collisionDisplay2D, SLOT(step()));
        connect(ui->stepButton, SIGNAL(clicked()), ui->collisionDisplay2D->getScene(), SLOT(advance()));

        // set up shortcuts
        stepShortcut = new QShortcut(QKeySequence(Qt::Key_S), ui->stepButton, SIGNAL(clicked()));

        // connect finished and starting signals to appropriate slots
        connect(ui->collisionDisplay2D, SIGNAL(finished()), this, SLOT(setDisplayFinished()));
        connect(this, SIGNAL(begin()), ui->collisionDisplay2D, SLOT(start()));
    }
    else {
        ((CollisionDisplay*)collisionDisplay3D)->setQAlgorithm(qalg);

        connect(ui->stopButton, SIGNAL(clicked()), collisionDisplay3D, SLOT(stop()), Qt::DirectConnection);
        // connect timer to display advancement and info panel update
        connect(((CollisionDisplay*)collisionDisplay3D)->getTimer(), SIGNAL(timeout()), collisionDisplay3D, SLOT(step()));
        connect(((CollisionDisplay*)collisionDisplay3D)->getTimer(), SIGNAL(timeout()), collisionDisplay3D, SLOT(update()));

        // connect step button to display advancement
        connect(ui->stepButton, SIGNAL(clicked()), collisionDisplay3D, SLOT(step()));
        connect(ui->stepButton, SIGNAL(clicked()), collisionDisplay3D, SLOT(update()));

        // set up shortcuts
        stepShortcut = new QShortcut(QKeySequence(Qt::Key_S), ui->stepButton, SIGNAL(clicked()));

        // connect finished and starting signals to appropriate slots
        connect(collisionDisplay3D, SIGNAL(finished()), this, SLOT(setDisplayFinished()));
        connect(this, SIGNAL(begin()), collisionDisplay3D, SLOT(start()));
    }

    // connect stop button to algorithm and display
    connect(ui->stopButton, SIGNAL(clicked()), qalg, SLOT(stop()), Qt::DirectConnection);

    // connect algorithm and display finishing to respective slots to allow for termination
    connect(qalg, SIGNAL(finished()), this, SLOT(setAlgFinished()));

    // connect begin signal with algorithm and display
    connect(this, SIGNAL(begin()), algThread, SLOT(start()));
    connect(this, SIGNAL(begin()), qalg, SLOT(run()));

    // disable controls which shouldn't be used while running
    ui->startButton->setEnabled(false);
    ui->timestepLimitCheckbox->setEnabled(false);
    ui->timestepLimitInput->setEnabled(false);
    ui->dimensionSelector->setEnabled(false);
    startShortcut->setEnabled(false);

    // actually begin running
    algRunning = true;
    displayRunning = true;
    emit begin();
    outputTimer.start(1000/60);

    // enable controls which can be used while running
    ui->pauseButton->setEnabled(true);
    pauseShortcut->setEnabled(true);
}

void CollisionSystem::terminate() {
    if (algRunning || displayRunning) {
        return;
    }

    printInfo();
    delete stepShortcut;
    stepShortcut = NULL;
    outputTimer.stop();
    algThread->quit();
    delete qalg;
    qalg = NULL;

    if (ui->dimensionSelector->currentIndex() == 0) {
        ui->collisionDisplay2D->setQAlgorithm(qalg);

        disconnect(ui->stopButton, SIGNAL(clicked()), ui->collisionDisplay2D, SLOT(stop()));
        // disconnect timer to display advancement and info panel update
        disconnect(ui->collisionDisplay2D->getTimer(), SIGNAL(timeout()), ui->collisionDisplay2D->getScene(), SLOT(advance()));
        disconnect(ui->collisionDisplay2D->getTimer(), SIGNAL(timeout()), ui->collisionDisplay2D, SLOT(step()));
        disconnect(ui->collisionDisplay2D->getTimer(), SIGNAL(timeout()), this, SLOT(printInfo()));
        // disconnect step button to display advancement
        disconnect(ui->stepButton, SIGNAL(clicked()), ui->collisionDisplay2D->getScene(), SLOT(advance()));
        disconnect(ui->stepButton, SIGNAL(clicked()), ui->collisionDisplay2D, SLOT(step()));
        disconnect(ui->collisionDisplay2D, SIGNAL(finished()), this, SLOT(setDisplayFinished()));
        disconnect(this, SIGNAL(begin()), ui->collisionDisplay2D, SLOT(start()));
    }
    else {
        ((CollisionDisplay*)collisionDisplay3D)->setQAlgorithm(qalg);

        disconnect(ui->stopButton, SIGNAL(clicked()), collisionDisplay3D, SLOT(stop()));
        // disconnect timer to display advancement and info panel update
        disconnect(((CollisionDisplay*)collisionDisplay3D)->getTimer(), SIGNAL(timeout()), collisionDisplay3D, SLOT(update()));
        disconnect(((CollisionDisplay*)collisionDisplay3D)->getTimer(), SIGNAL(timeout()), collisionDisplay3D, SLOT(step()));
        disconnect(((CollisionDisplay*)collisionDisplay3D)->getTimer(), SIGNAL(timeout()), this, SLOT(printInfo()));
        // disconnect step button to display advancement
        disconnect(ui->stepButton, SIGNAL(clicked()), collisionDisplay3D, SLOT(update()));
        disconnect(ui->stepButton, SIGNAL(clicked()), collisionDisplay3D, SLOT(step()));
        disconnect(collisionDisplay3D, SIGNAL(finished()), this, SLOT(setDisplayFinished()));
        disconnect(this, SIGNAL(begin()), collisionDisplay3D, SLOT(start()));
    }

    // disable input which is only available when running
    ui->pauseButton->setEnabled(false);
    ui->stepButton->setEnabled(false);
    ui->pauseButton->setText(QString("Pause"));

    // enable input which can only be used when not running
    ui->startButton->setEnabled(true);
    ui->dimensionSelector->setEnabled(true);
    ui->timestepLimitCheckbox->setEnabled(true);
    ui->timestepLimitInput->setEnabled(ui->timestepLimitCheckbox->isChecked());
    paused = false;
    pauseShortcut->setEnabled(false);
    startShortcut->setEnabled(true);

    if (closing != NULL) {
        closeEvent(closing);
    }
}

void CollisionSystem::printInfo() {
    // clear output each timestep
    ui->systemOutput->clear();

    int displayTimestep = 0;
    int algTimestep = qalg->getAlgorithm()->getTimestep() / ALG_SPEED_SCALE;
    int numCollisions = 0;
    double systemKE = qalg->getAlgorithm()->getSystemKE();
    double systemPE = qalg->getAlgorithm()->getSystemPE();
    int queueSize = qalg->getAlgorithm()->getPositionQueue()->getSize();

    if (environment == 0)
    {
        displayTimestep = ui->collisionDisplay2D->getDisplayTimestep();
        numCollisions = ui->collisionDisplay2D->getNumCollisions();
    }
    else
    {
        displayTimestep = ((CollisionDisplay*)collisionDisplay3D)->getDisplayTimestep();
        numCollisions = ((CollisionDisplay*)collisionDisplay3D)->getNumCollisions();
    }

    // display timestep
    if (ui->displayTimestepViewable->isChecked())
    {
        ui->systemOutput->append(QString("Display timestep: ") + QString::number(displayTimestep));
    }

    // algorithm timestep
    if (ui->algTimestepViewable->isChecked())
    {
        ui->systemOutput->append(QString("Algorithm timestep: ") + QString::number(algTimestep));
    }

    // number of collisions
    if (ui->numCollisionsViewable->isChecked())
    {
        ui->systemOutput->append(QString("Number of collisions: ") + QString::number(numCollisions));
    }

    // system KE
    if (ui->systemKEViewable->isChecked())
    {
        ui->systemOutput->append(QString("System KE: ") + QString::number(systemKE));
    }

    // system PE
    if (ui->systemPEViewable->isChecked())
    {
        ui->systemOutput->append(QString("System PE: ") + QString::number(systemPE));
    }

    // total energy
    if (ui->totalEnergyViewable->isChecked()) {
        ui->systemOutput->append(QString("Total energy: " + QString::number(systemKE + systemPE)));
    }

    // queue size
    if (ui->queueSizeViewable->isChecked()) {
        ui->systemOutput->append(QString("Queue size: ") + QString::number(queueSize));
    }

    // start conditions
    if (ui->startConditionsViewable->isChecked())
    {
        ui->systemOutput->append(QString("Seed: ") + QString::number(SEED));
        ui->systemOutput->append(QString("Number of particles: ") + QString::number(NUM_PARTICLES));
        ui->systemOutput->append(QString("Particle radius range: (") + QString::number(MIN_PARTICLE_RADIUS) + QString(", ") + QString::number(MAX_PARTICLE_RADIUS) + QString(")"));
        ui->systemOutput->append(QString("Timestep limit: ") + QString::number(TIMESTEP_LIMIT / ALG_SPEED_SCALE));
    }
}

void CollisionSystem::changePausedState() {
    if (paused) {
        if (environment == 0)
        {
            ui->collisionDisplay2D->getTimer()->start(1000 / FRAMERATE);
        }
        else {
            ((CollisionDisplay*)collisionDisplay3D)->getTimer()->start(1000 / FRAMERATE);
        }
        ui->pauseButton->setText(QString("Pause"));
        paused = false;
        ui->stepButton->setEnabled(false);
    }
    else {
        if (environment == 0)
        {
            ui->collisionDisplay2D->getTimer()->stop();
        }
        else {
            ((CollisionDisplay*)collisionDisplay3D)->getTimer()->stop();
        }
        ui->pauseButton->setText(QString("Play"));
        paused = true;
        ui->stepButton->setEnabled(true);
    }
}

void CollisionSystem::resetCamera() {
    collisionDisplay3D->camera()->setEye(QVector3D(-100, Y_DIMENSION_SIZE/2, Z_DIMENSION_SIZE/2));
    collisionDisplay3D->camera()->setUpVector(QVector3D(0, 0, 1));
}

// SLOTS - value setters
void CollisionSystem::generateSeed() {
    ui->seedInput->setText(QString::number(time(0)));
}

void CollisionSystem::setAlgFinished() {
    algRunning = false;
    terminate();
}

void CollisionSystem::setDisplayFinished() {
    displayRunning = false;
    terminate();
}

void CollisionSystem::setLookaheadTime(double d){
    LOOKAHEAD_TIMESTEPS = d * FRAMERATE;
}

void CollisionSystem::setTargetFramerate(int n) {
    FRAMERATE = n;
    setLookaheadTime(ui->lookaheadTimeInput->value());
    MAX_LOOKAHEAD_TIME = ((double)QUEUE_SIZE_MULTIPLIER) / FRAMERATE;
    ui->lookaheadTimeInput->setMaximum(MAX_LOOKAHEAD_TIME);
}

void CollisionSystem::setMinRadiusMax(double d) {
    // change max for minRadius to current max radius
    ui->minRadiusBox->setMaximum(d);
}

void CollisionSystem::setMaxRadiusMin(double d) {
    // change min for maxRadius to current min radius
    ui->maxRadiusBox->setMinimum(d);
}

void CollisionSystem::setTimestepLimit(int n) {
    TIMESTEP_LIMIT = ALG_SPEED_SCALE * n;
}

void CollisionSystem::setTimestepLimitEnabled(bool b) {
    ui->timestepLimitInput->setEnabled(b);
    if (b) {
        TIMESTEP_LIMIT = ALG_SPEED_SCALE * ui->timestepLimitInput->value();
    }
    else {
        TIMESTEP_LIMIT = -1;
    }
}

void CollisionSystem::setSpace(int b) {
    environment = b;
    if (b == 0) {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else {
        ui->stackedWidget->setCurrentIndex(3);
    }
    if (environment != 0) {
        ui->resetCameraButton->setEnabled(true);
        ui->framerateSlider->setMaximum(60);
    }
    else {
        ui->resetCameraButton->setEnabled(false);
        ui->framerateSlider->setMaximum(MAX_FRAMERATE);
    }
}
