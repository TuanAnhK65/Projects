#include "BaseObject.h"
BaseObject::BaseObject() { //thiết lập đối tượng
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}
BaseObject::~BaseObject()
{
    Free();
}

bool BaseObject::LoadImageFile(std::string path, SDL_Renderer* screen) {
    Free();
    SDL_Texture* newTexture = NULL;
    //Tải ảnh từ đường dẫn chỉ định
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != NULL) {
        //Color key image
        Uint32 key = SDL_MapRGB(loadedSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);
        //Tạo cấu trúc từ các bề mặt pixel
        newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface);
        if (newTexture != NULL) {//Kích thước hình ảnh hiện thị
            rect_.w = loadedSurface->w;
            rect_.h = loadedSurface->h;
        }
        //xóa bỏ bề mật cũ trước đó
        SDL_FreeSurface(loadedSurface);
    }
    p_object_ = newTexture;
    return p_object_ != NULL;
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip) {
    SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h }; //kích thước rendering clip
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(des, p_object_, clip, &renderQuad);//copy render
}

void BaseObject::Free() { //Giải phóng bộ nhớ
    if (p_object_ != NULL) {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}

