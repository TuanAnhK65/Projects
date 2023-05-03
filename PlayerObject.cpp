
#include "PlayerObject.h"
#include "SDL_mixer.h"

PlayerObject::PlayerObject()
{
    is_falling_ = false; //nhan vat dang bay    
    y_val_ = 1;
}


PlayerObject::~PlayerObject()
{

    Free();

}

void PlayerObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
        //SDL_Event su dung du lieu su kien ban phim 
            //SDL_KEYDOWN la 1 lan nhan phim

    {
        switch (events.key.keysym.sym)
        { //xet cac phim dc nhan
        case SDLK_UP: //nut len 
        {
            if (is_falling_ == false)//nhan vat bay len
            {
                y_val_ = -12; //khoang cach bay len cua vat
            }
        }
        }
    }
    else if (events.type == SDL_KEYUP) // 1 lần nhả phím ra
    {

        switch (events.key.keysym.sym)
        {
        case SDLK_UP:
        {
            y_val_ = 3; //nhan vat roi
        }
        break;
        }
    }
}


bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImageFile(path, screen); //tải ảnh từ đường dẫn
    return ret;
}

void PlayerObject::Show(SDL_Renderer* des) //gọi render
{

    BaseObject::Render(des);
}

void PlayerObject::DoFalling(SDL_Renderer* des)
{
    rect_.y += y_val_;
    if ((rect_.y + rect_.h) >= GROUND_MAP) //điều kiện nhân vật chạm đất và chết
    {
        LoadImg("img//explosion.png", des); //tải ảnh vụ nổ lúc nhân vật chết
        Mix_Chunk* beep_sound = Mix_LoadWAV("sound//explosion.wav"); //tải âm thanh nổ
        if (beep_sound != NULL)
            Mix_PlayChannel(-1, beep_sound, 0);
        is_falling_ = true;
        DoGround(des);
    }
}

void PlayerObject::DoGround(SDL_Renderer* screen)
{
    y_val_ = 0;
    rect_.y = GROUND_MAP - rect_.h;
    is_die_ = true;
}