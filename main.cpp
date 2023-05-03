
#include "CommonFunc.h"
#include "PlayerObject.h"

#include "CommonFunc.h"
#include "PlayerObject.h"
#include "ImpTimer.h"
#include "BlockObject.h"
#include "Geometric.h"
#include "SDL_mixer.h"

#define SCREEN_WIDTH 1280 //chiều rộng màn hình hiển thị
#define SCREEN_HEIGHT 660 //chiều cao màn hình hiển thị


BaseObject g_background;
BaseObject g_ground;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;
bool InitData() //khởi tạo thông số ban đầu
{
    BOOL bSucess = true;
    int ret = SDL_Init(SDL_INIT_VIDEO); //thiết lập môi trường ban đầu cho SDL
    if (ret < 0) {  //nếu hàm lỗi
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); //thiết lập tỉ lệ chất lượng

    //tạo của sổ hiển thị game
    g_window = SDL_CreateWindow("SkyWar - Group_FirePlane",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    if (g_window != NULL){
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); //tạo render cho màn hình
        if (g_screen != NULL){
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                bSucess = false;
        }

        if (TTF_Init() == -1){
            bSucess = false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
            bSucess = false;
        }

        g_font_text = TTF_OpenFont("font//ARCADE.ttf", 38);
        if (g_font_text == NULL){
            return false;
        }

        g_font_MENU = TTF_OpenFont("font//ARCADE.ttf", 80);
        if (g_font_MENU == NULL){
            return false;
        }
    }

    return bSucess;
}

bool LoadBackground(){ //tải ảnh nền game
    bool ret = g_background.LoadImageFile("img//background.png", g_screen);
    return ret;
}


void close(){ //giải phóng tài nguyên khi thoát game
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]){
    if (!InitData()){
        return -1;
    }

    ImpTimer fps;
    bool quit = false;

    //Tạo menu game 
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Play Game", "Exit", "img//MENU.png"); 
    //thiết lập các lựa chọn trong game và tải ảnh giao diện game lên
    if (ret_menu == 1)
        quit = true;
    

    if (!LoadBackground()){
        return -1;
    }

    bool ret = g_ground.LoadImageFile("img//ground.png", g_screen); //tải ảnh bề mặt bên dưới
    if (!ret){
        return -1;
    }
    g_ground.SetRect(0, GROUND_MAP);

    //chạy nhạc nền game//
    Mix_Chunk* sound = Mix_LoadWAV("sound//background_music.wav");
    if (sound != NULL)  Mix_PlayChannel(-1, sound, -1);

again_label: //khởi tạo các hàm//

    TextObject timegame;
    timegame.setColor(TextObject::WHITE_TEXT);
    
    TextObject text_count;
    text_count.setColor(TextObject::WHITE_TEXT);

    PlayerObject player;
    ret = player.LoadImg("img//plane.png", g_screen); //tải ảnh máy bay lên
    player.SetRect(100, 100);
    if (!ret){
        return -1;
    }

    BlockManager manage_block;
    ret = manage_block.InitBlockList(g_screen);
    if (!ret)
        return -1;

    //Vòng lặp chính//
    while (!quit) 
    {
        fps.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type==SDL_QUIT)
            {
                quit = true;
            }

            player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR,  RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        //thiết lập chế độ màu cho màn hình

        SDL_RenderClear(g_screen);//làm mới màn hình

        g_background.Render(g_screen, NULL);//fill ảnh nền vào cửa sổ

        manage_block.SetPlayerRect(player.GetRect());

        //nhân vật va chạm và rơi
        bool is_falling = player.GetFalling();
        if (is_falling){
            manage_block.SetStopMoving(true);
        }

        manage_block.Render(g_screen);

        bool endgame = manage_block.GetColState(); 
        if (endgame){
            player.SetFalling(true);
        }
        player.DoFalling(g_screen); 
        player.Show(g_screen); 

        //Vẽ thanh hcn hiển thị điểm và thời gian chơi 
        GeometricFormat rectange_size(0, 0, SCREEN_WIDTH, 30);
        ColorData color_data(36, 36, 36);
        Gemometric::RenderRectange(rectange_size, color_data, g_screen);

        GeometricFormat outlie_size(1, 1, SCREEN_WIDTH - 1, 28);
        ColorData color_data1(255, 255, 255);
        Gemometric::RenderOutline(outlie_size, color_data1, g_screen);

       //Bộ đếm thời gian trong game
        std::string str_time = "Time : ";
        UINT32 time_val = SDL_GetTicks() / 1000;
        std::string str_val = std::to_string(time_val);
        str_time += str_val;

        timegame.SetText(str_time);
        timegame.loadFromRenderedText(g_font_text, g_screen);
        timegame.RenderText(g_screen, SCREEN_WIDTH - 200, 5);

        //hàm tính điểm máy bay khi qua mỗi chướng ngại vật
        int count = manage_block.GetCount(); 
        std::string count_str = std::to_string(count);
        text_count.SetText(count_str);
        text_count.loadFromRenderedText(g_font_text, g_screen);
        text_count.RenderText(g_screen, SCREEN_WIDTH*0.5, 2);

        g_ground.Render(g_screen);
        SDL_RenderPresent(g_screen);

        // Tạo màn hình "Game over"
        bool game_over = player.GetIsDie();
        if (game_over)
        {
            Sleep(500);
            int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Try Again", "Exit", "img//Ending.png");
            //thiết lập các lựa chọn cho người chơi và tải ảnh menu "game over" lên
            if (ret_menu == 1){
                quit = true;
                continue;
            }
            else{
                quit = false;
                manage_block.FreeBlock();
                goto again_label;
            }
        }
        //Cài đặt tốc độ khung hình 
        int val1 = fps.get_ticks();
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND){
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }
    close();
    return 0;
}

