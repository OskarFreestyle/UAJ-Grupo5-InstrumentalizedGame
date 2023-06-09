#ifndef RAYCAST_H_
#define RAYCAST_H_
#include "Vector2D.h"
#include "Collisions.h"
#include <array>
#include <limits>
#include <list>
#include "../utils/checkML.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

class Transform;
class Entity;

class RayCast
{
private:
	Point2D origin_ = Point2D();
	Vector2D direction_ = Vector2D();
	Point2D pointOfImpact_ = Point2D();
	double distance_ = -1.0;

	
	class Square
	{
	public:
		std::array<Point2D, 4> vertices; // 4 points, arranged anti-clockwise
		Point2D centre;

		Square() : vertices(std::array<Point2D, 4>()), centre(Point2D()) {};

		Square(Point2D centre_, Point2D vertex0, Point2D vertex1): centre(centre_)
		{
			vertices[0] = vertex0;
			vertices[1] = vertex1;
			vertices[2] = centre + (centre - vertex0);
			vertices[3] = centre + (centre - vertex1);
		}
	};

private:
	static short int getClosestVertex(const Point2D& p, const Square& s);

public:
	RayCast(Point2D origin, Vector2D direction) : origin_(origin), direction_(direction) {};

	//Create a raycast pointing towards the centre of a tranform and trace to it
	RayCast(Point2D origin, Transform* tr);

	double getDistance() { return distance_; };

	Point2D getPointOfImpact() { return pointOfImpact_; };
	
	bool hasCollision(float maxRange = std::numeric_limits<float>::max()) { return distance_ >= 0.0 && distance_ < maxRange; };

	void rayCastToSquare(Vector2D centre, Vector2D vertex0, Vector2D vertex1);

	void rayCastToSquare(Transform* transform);

	void rayCastToSquare(Entity* entity);

	void rayCastCollideWalls(Transform* transform);

	/// Detects and returns all entities with transforms belonging to a specified group that
	/// the ray collides with
	template <typename Group>
	std::list<Entity*> allCollisionsInLine(Manager* entityManager)
	{
		std::list<Entity*> collided = std::list<Entity*>();
		auto entities = entityManager->getEntities();
		for (Entity* e : entities)
		{
			if (e->hasGroup<Group>() && e->hasComponent<Transform>())
			{
				rayCastCollideWalls(e->getComponent<Transform>());
				if (distance_ >= 0.0)
					collided.push_back(e);
			}
		}
		return collided;
	}

	/// Detects and returns all entities with transforms belonging to a specified group that
	/// are within a certain radius of a point with a clear line of sight
	template <typename Group>
	static std::list<Entity*> allCollisionsInRadius(Manager* entityManager, Point2D origin, float radius)
	{
		std::list<Entity*> collided = std::list<Entity*>();
		auto entities = entityManager->getEntities();
		for (Entity* e : entities)
		{
			if (e->hasGroup<Group>() && e->hasComponent<Transform>())
			{
				// Cast a new ray from the origin to the entity
				RayCast aux = RayCast(origin, e->getComponent<Transform>()->getPos() - origin);
				aux.rayCastCollideWalls(e->getComponent<Transform>());

				// Check that the ray collided and was within specified radius
				if (aux.distance_ >= 0.0 && aux.distance_ < radius)
					collided.push_back(e);
			}
		}
		return collided;
	}

	/// Calculates minimum distance in a specified direction to any and all entities with transforms that belong to a certain group.
	/// Makes modifications to RayCast to display point of nearest collision and distance, which it returns
	/// If no collision is detected, distance will be -1.0
	template <typename Group>
	double distanceToGroup(Manager* entityManager)
	{
		auto entities = entityManager->getEntities();
		RayCast aux = RayCast(*this);
		for (Entity* e : entities)
		{
			if (e->hasGroup<Group>() && e->hasComponent<Transform>())
			{
				aux.rayCastToSquare(e->getComponent<Transform>());
				if (aux.distance_ != -1.0 && (aux.distance_ < distance_ || distance_ == -1.0))
					*this = RayCast(aux);
			}
		}
		return distance_;
	}

	// Returns true if transform is less than 0.2 above an object of group Wall_grp
	static bool isGrounded(Transform* tr);
};

#endif