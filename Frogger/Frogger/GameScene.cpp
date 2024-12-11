#include "GameScene.h"

GameScene::GameScene(PxPhysics* gPhysics, PxScene* gScene) :
	gPhysics_(gPhysics), gScene_(gScene) {

	//Creacion del suelo
	floor_ = gPhysics->createRigidStatic(PxTransform({ 0,-300,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(WIDTH, 0.1, HEIGHT));
	floor_->attachShape(*shape);
	gScene->addActor(*floor_);
	renderItemFloor_ = new RenderItem(shape, floor_, { 1,1,1,1 });

	/*floor_ = new Platform(gPhysics_, gScene_, Vector3(0, -300, 0),
		WIDTH, 0.1, HEIGHT, Vector4(1, 1, 1, 1));*/

	initPlatforms();
	initPlayer();

	//Seteamos la camara
	Camera* cam = GetCamera();
	cam->setView(PxVec3(-18.0f, 550.0f, 101.0f), PxVec3(0.0f, -0.9f, -0.1f));
}


void GameScene::initPlatforms() {

	//Creacion de las plataformas
	Platform* leftLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x - WIDTH, -290, floor_->getGlobalPose().p.z),
		20, 20, HEIGHT, Vector4(0, 1, 1, 1));

	Platform* rightLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x + WIDTH, -290, floor_->getGlobalPose().p.z),
		20, 20, HEIGHT, Vector4(0, 1, 1, 1));

	Platform* downLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x, -290, floor_->getGlobalPose().p.z - HEIGHT),
		WIDTH, 20, 20, Vector4(0, 1, 1, 1));

	Platform* upLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x, -290, floor_->getGlobalPose().p.z + HEIGHT),
		WIDTH, 20, 20, Vector4(0, 1, 1, 1));

	limits_.push_back(leftLimit);
	limits_.push_back(rightLimit);
	limits_.push_back(downLimit);
	limits_.push_back(upLimit);
}

void GameScene::initPlayer() {
	//Inicialización del jugador
	Vector3 centerFloor = Vector3(floor_->getGlobalPose().p.x / 2, -290, floor_->getGlobalPose().p.z + 400);
	player_ = new Frogger(gPhysics_, gScene_, centerFloor);
}

GameScene::~GameScene() {
	//Floor
	//delete floor_;
	floor_->release();
	renderItemFloor_->release();
	renderItemFloor_ = nullptr;

	//Plataformas
	for (Platform* p : limits_)
		delete p;
	limits_.clear();

	//Player
	delete player_;
	player_ = nullptr;

	//Enemigos
}

void GameScene::update(double t) {
	player_->integrate(t);
}

void GameScene::keyPressed(char k) {
	player_->keyPressed(k);
}
