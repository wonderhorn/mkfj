#pragma once
#include"framework/Object.h"
#include"jumpnrun/mover/character/Character.h"
#include"utils/OwnerHandle.h"
#include"jumpnrun/stage/Stage.h"
namespace jnr
{
	class EnemySearcher
	{
	public:
		EnemySearcher();
		void initialize(jnr::Mover* me);
		std::vector<gfw::Object::Pipe> search(double range);

	private:
		OWNERHANDLE owner;
		gfw::Tasklist* tl;
		//jnr::Character* me;
		jnr::Mover* me;
	};

	class Router
	{
	public:
		Router();
		gmtr::Vector2D grid2point(int x, int y, const gmtr::Vector2D& p_ori, const gmtr::Vector2D& p_ter);
		std::pair<int, int> point2grid(const gmtr::Vector2D& p, const gmtr::Vector2D& p_ori, const gmtr::Vector2D& p_ter);
		int route(Stage& stage, const gmtr::Vector2D& p_ori, int myw, int myh, const gmtr::Vector2D& p_ter);
		int w, h, grid_size, margin;
		int ter_x, ter_y;
		std::vector<double> grids;
	};
};