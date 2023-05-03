#include "Geometric.h" //tạo hình học

void Gemometric::RenderRectange(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen){
//tạo hình chữ nhật gồm kích thước ,màu sắc
    SDL_Rect fillRect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};//kích thước hcn 
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF); //màu sắc hcn     
    SDL_RenderFillRect(screen, &fillRect ); //tô màu hcn
}

void Gemometric::RenderOutline(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen){ //tạo viền cho hcn
    SDL_Rect outlineRect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_}; //kích thước viền
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF); //màu sắc viền     
}