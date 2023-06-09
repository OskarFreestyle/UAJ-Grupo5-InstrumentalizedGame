#pragma once
#include "player_health_component.h"
#include "../ecs/Manager.h"
#include "../utils/checkML.h"
#include "../game/constant_variables.h"
class Physiognomy;
class HungerComponent;

/*
* BleedoutComponent es un componente de da�o que puede sufrir el jugador en raid.
* Va a ser provocado por balas o proyectiles que disparen los enemigos.
* Tiene efecto acumulativo, de tal forma que si no es curado en un cierto tiempo, se va a a�adir otro da�o de desangrado a la pila.
* No est� implementado con diferentes niveles de gravedad de desangrado si no como tiempo pasado desde que el jugador recibi� el da�o.
*/
class BleedoutComponent : public PlayerHealthComponent
{
private:
	int accumulatedTime;
	float percentage = 0.0;	// Indicates how full the bleed meter is. At 100%, becomes Bloodloss and creates a new Bleedout
	Physiognomy* phys;
	HungerComponent* hunger;
public:
	BleedoutComponent() : accumulatedTime(0), phys(nullptr), hunger(nullptr) {};
	void init() override;
	void update() override;
};

