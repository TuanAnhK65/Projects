
#include "CommonFunc.h"
int SDLCommonFunc::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font, const std::string& menu_game, const std::string& ending, const std::string& img_name){
//tạo màn hình menu và ending cho game
    char* ch1 = (char*)menu_game.c_str();
    char* ch2 = (char*)ending.c_str();
    char* img_file = (char*)img_name.c_str();

    int size1 = menu_game.length(); //khởi tạo chiều dài menu và ending
    int size2 = ending.length();
    int time = 0;
    int x = 0;
    int y = 0;
    const int kMenuNum = 2;
    char* labels[kMenuNum];

    labels[0] = new char [size1 + 1];
    labels[1] = new char[size2 + 1];

    strcpy_s(labels[0], size1+1, ch1);
    strcpy_s(labels[1], size2+1, ch2);

    SDL_Texture* menu[kMenuNum]; //Cấu trúc menu và ending
    bool selected[kMenuNum] = { 0, 0 };
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } }; //màu của menu và ending

    TextObject text_object[kMenuNum]; //chữ của menu và ending
    text_object[0].SetText(labels[0]);
    text_object[0].setColor(color[0].r, color[0].g, color[0].b);

    text_object[0].loadFromRenderedText(font, g_screen); //tải phông chữ lên

    text_object[1].SetText(labels[1]);
    text_object[1].setColor(color[0].r, color[0].g, color[0].b);
    text_object[1].loadFromRenderedText(font, g_screen);

    SDL_Rect pos[kMenuNum]; //khởi tạo vị trí cac chữ trên menu và ending 
    pos[0].x = SCREEN_WIDTH*0.3 - 150;
    pos[0].y = SCREEN_HEIGHT*0.8 - 40;
    pos[1].x = SCREEN_WIDTH*0.3 - 150;
    pos[1].y = SCREEN_HEIGHT*0.8 + 40;

    BaseObject gBackground;
    bool ret = gBackground.LoadImageFile(img_file, g_screen); //tải background từ link file

    SDL_Event event; //SDL_Event su dung du lieu su kien ban phim
    while (1){
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event)){
            switch (event.type){ //kiểu dữ liệu          
            case SDL_QUIT: //giải phóng bộ nhớ nếu màn hình out 
                text_object[0].Free();
                text_object[0].Free();
                gBackground.Free();
                return 1;
            case SDL_MOUSEMOTION: //thiết lập chuyển động của chuột
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i){
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                        if (!selected[i]) {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                    else {
                        if (selected[i]) {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN: //Thao tác nhấn chuột
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                        text_object[0].Free();
                        text_object[1].Free();
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN: //nhấn
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    text_object[0].Free();
                    text_object[1].Free();
                    gBackground.Free();
                    return 1;
                }
            }
        }

        gBackground.Render(g_screen, NULL);//kích thước background 

        for (int i = 0; i < kMenuNum; ++i){
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].getWidth();
            pos[i].h = text_object[i].getHeight();
        }


        SDL_RenderPresent(g_screen); 
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }
    return 0;
}


int SDLCommonFunc::MyRandom(int min, int max)//lấy giới hạn từ min, max được tạo ngẫu nhiên
{
    return rand() % (max - min + 1) + min;
}

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)  //va chạm của người chơi
{
    //khởi tạo vị trị đối tượng 1 //
  int left_a = object1.x;
  int right_a = object1.x + object1.w;
  int top_a = object1.y;
  int bottom_a = object1.y + object1.h;

  //khởi tạo vị trị đối tượng 2 //
  int left_b = object2.x;
  int right_b = object2.x + object2.w;
  int top_b = object2.y;
  int bottom_b = object2.y + object2.h;

  // TH1: kích thước đối tượng 1 < kích thước đối tượng 2
  if (left_a > left_b && left_a < right_b){
    if (top_a > top_b && top_a < bottom_b){
      return true;
    }
  }

  if (left_a > left_b && left_a < right_b){
    if (bottom_a > top_b && bottom_a < bottom_b){
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b){
    if (top_a > top_b && top_a < bottom_b){
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b){
    if (bottom_a > top_b && bottom_a < bottom_b){
      return true;
    }
  }

  // TH2: kích thước đối tượng 1 > kích thước đối tượng 2
  if (left_b > left_a && left_b < right_a){
    if (top_b > top_a && top_b < bottom_a){
      return true;
    }
  }

  if (left_b > left_a && left_b < right_a){
    if (bottom_b > top_a && bottom_b < bottom_a){
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a){
    if (top_b > top_a && top_b < bottom_a){
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a){
    if (bottom_b > top_a && bottom_b < bottom_a){
      return true;
    }
  }

  // TH3: kích thước đối tượng 1 = kích thước đối tượng 2
  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b){
    return true;
  }
  return false;
}

