// Link tham khao: https://phattrienphanmem123az.com/lap-trinh-game-c-p2/game-cpp-phan-2-ky-thuat-load-nhan-vat-game.html //
#ifndef  BASE_OBJECT_H

#define BASE_OBJECT_H

#include "CommonFunc.h"


class BaseObject
{
public:
	BaseObject();
	~BaseObject();
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }
	SDL_Rect GetRect() const { return rect_; }
	SDL_Texture* GetObject() const { return p_object_; }

	int GetWidth() const { return rect_.w; }
	int GetHeight() const { return rect_.h; }

	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

	void Render_bg(SDL_Renderer* des, const SDL_Rect* clip = NULL, const int& x_pos = 0, const int& y_pos = 0);
	void Free();
protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;

};

#endif // ! BASE_OBJECT_H

