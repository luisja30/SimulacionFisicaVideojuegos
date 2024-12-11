#include "GameScene.h"

GameScene::GameScene(PxPhysics* gPhysics, PxScene* gScene) :
	gPhysics_(gPhysics), gScene_(gScene) {
	
	//Creacion del suelo
	floor_ = gPhysics->createRigidStatic(PxTransform({ 0,-300,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(WIDTH, 0.1, HEIGHT));
	floor_->attachShape(*shape);
	gScene->addActor(*floor_);
	renderItemFloor_ = new RenderItem(shape, floor_, { 1,1,1,1 });

	//Inicialización del jugador
	Vector3 centerFloor = Vector3(floor_->getGlobalPose().p.x / 2, -290, floor_->getGlobalPose().p.z + 400);
	player_ = new Frogger(gPhysics_, gScene_, centerFloor);

	//Seteamos la camara
	Camera* cam = GetCamera();
	cam->setView(PxVec3(-18.0f, 550.0f, 101.0f), PxVec3(0.0f, -0.9f, -0.1f));
}

GameScene::~GameScene() {
	//Floor
	floor_->release();
	renderItemFloor_->release();
	renderItemFloor_ = nullptr;

	//Player
	delete player_;
	player_ = nullptr;

	//Enemigos

}

void GameScene::update(double t) {
	player_->integrate(t);
}
