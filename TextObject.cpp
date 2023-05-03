#include "TextObject.h"
//phông chữ và cỡ chữ
TextObject::TextObject(void){
  text_color.r = 255; //chỉ số màu sắc
  text_color.g = 255;
  text_color.b = 255;
}
TextObject::~TextObject(void)
{}

bool TextObject::loadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen){
  //tạo phông ,màu chữ
  SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, str_val.c_str(), text_color);
  if( textSurface != NULL ){
    //thiết lập chữ
    texture = SDL_CreateTextureFromSurface(screen, textSurface);
    if( texture != NULL ){
      //Kích thước của chữ
      width = textSurface->w;
      height = textSurface->h;
    }

    //Giải phóng textSurface
    SDL_FreeSurface( textSurface );
  }
  //nếu thành công trả về true
  return texture != NULL;
}

void TextObject::Free(){ //giải phóng texture
  if(texture != NULL){
    SDL_DestroyTexture(texture);
    texture = NULL;
  }
}

void TextObject::setColor(Uint8 red, Uint8 green, Uint8 blue){ //khai báo các chỉ số màu của chữ
  text_color.r = red;
  text_color.g = green;
  text_color.b = blue;
}

void TextObject::setColor(int type){ //chỉ số màu
  if (type == RED_TEXT){
    SDL_Color color = {255, 0, 0};
    text_color = color;
  }
  else if (type == WHITE_TEXT){
    SDL_Color color = {255, 255, 255};
    text_color = color;
  }
  else{
    SDL_Color color = {0, 0, 0};
    text_color = color;
  }
}
void TextObject::RenderText(SDL_Renderer* screen, int x, int y, SDL_Rect* clip , double angle , SDL_Point* center , SDL_RendererFlip flip){
  //Cài đặt rendering space và hiển thị ra màn hình
  SDL_Rect renderQuad = { x, y, width, height };
  //Cài đặt kích thước rendering clip
  if( clip != NULL ){
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  //Render to screen
  SDL_RenderCopyEx(screen, texture, clip, &renderQuad, angle, center, flip );
}
