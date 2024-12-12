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
	initEnemies();
	initGoals();

	//Seteamos la camara
	Camera* cam = GetCamera();
	cam->setView(PxVec3(-18.0f, 630.0f, 101.0f), PxVec3(0.0f, -0.9f, -0.1f)); //Alejar camara
	//cam->setView(PxVec3(-18.0f, -200.0f, 101.0f), PxVec3(0.0f, -0.9f, -0.1f)); //Acercar camara
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
	for (Car* c : cars_)
		delete c;
	cars_.clear();

	//Metas
	for (Goal* g : goals_)
		delete g;
	goals_.clear();

}

void GameScene::initPlatforms() {
	//Creacion de las plataformas
	Platform* leftLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x - WIDTH, -290, floor_->getGlobalPose().p.z),
		20, 100, HEIGHT, Vector4(0, 1, 1, 1));

	Platform* rightLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x + WIDTH, -290, floor_->getGlobalPose().p.z),
		20, 100, HEIGHT, Vector4(0, 1, 1, 1));

	Platform* downLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x, -290, floor_->getGlobalPose().p.z - HEIGHT),
		WIDTH, 100, 20, Vector4(0, 1, 1, 1));

	Platform* upLimit = new Platform(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x, -290, floor_->getGlobalPose().p.z + HEIGHT),
		WIDTH, 100, 20, Vector4(0, 1, 1, 1));

	limits_.push_back(leftLimit);
	limits_.push_back(rightLimit);
	limits_.push_back(downLimit);
	limits_.push_back(upLimit);
}

void GameScene::initEnemies() {
	Car* car1 = new Car(gPhysics_, gScene_,
		Vector3(floor_->getGlobalPose().p.x / 2, -290, floor_->getGlobalPose().p.z / 2),
		1, Vector4(1, 0, 0, 1));

	cars_.push_back(car1);
}

void GameScene::initGoals() {

	Vector3 pos = Vector3(floor_->getGlobalPose().p.x - 300, -310, floor_->getGlobalPose().p.z - 400);
	float offsetX = 150;

	for (int i = 0; i < GOALS; i++) {
		Goal* goal = new Goal(pos);
		goals_.push_back(goal);
		pos += Vector3(offsetX, 0, 0);
	}
}

void GameScene::collisionsPlayerGoals() {
	Vector3 playerPos = player_->getPosition();

	for (int i = 0; i < GOALS; i++) {
		bool goalActive = goals_[i]->getActive();
		if (goals_[i]->insideGoal(playerPos) && goalActive) {
			resetPlayerPosition();
			goals_[i]->setGoal();
			std::cout << "GOAL_ " + to_string(i) + " Ha sido desactivada" << std::endl;
		}

	}
}

void GameScene::resetPlayerPosition() {
	player_->resetPosition();
}

void GameScene::initPlayer() {
	//Inicialización del jugador
	Vector3 centerFloor = Vector3(floor_->getGlobalPose().p.x / 2, -290, floor_->getGlobalPose().p.z + 400);
	player_ = new Frogger(gPhysics_, gScene_, centerFloor);
}



void GameScene::update(double t) {
	//Update entidades
	player_->integrate(t);

	for (Goal* g : goals_)
		g->update(t);

	//Comprobacion de colisiones
	collisionsPlayerGoals();
}

void GameScene::keyPressed(char k) {
	player_->keyPressed(k);
}
