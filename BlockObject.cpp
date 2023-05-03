
#include "BlockObject.h"
INT g_pos[] = { 1250, 1500, 1750, 2000, 2250, 2500 }; //vị trí có thể xuất hiện của vật cản
BlockObject::BlockObject(){
    is_back = false;
}
BlockObject::~BlockObject(){}

bool BlockObject::LoadImg(std::string path, SDL_Renderer* screen){
    bool ret = BaseObject::LoadImageFile(path, screen); //tải ảnh từ đường dẫn
    return ret;
}

void BlockObject::SetPos(const int& xp, const int& yp){  //đặt vị trí vật thể
    this->SetRect(xp, yp);
}

void BlockObject::SetXPos(const int& xp){  //vị trí x
    SDL_Rect rect = this->GetRect();
    this->SetRect(xp, rect.y);
}

void BlockObject::ShowImg(SDL_Renderer* screen){  //gọi render
    this->Render(screen);
}

void BlockObject::DoRun(UINT& xval){
    this->rect_.x += xval;
    if (rect_.x + rect_.w < 0){ //chuyển động vật thể ngược với nhân vật khi chạy qua
        is_back = true;
    }
}

DoubleBlock::DoubleBlock(){
    xval = -3;
    is_back = false;
    is_pass = false;
}
DoubleBlock::~DoubleBlock() {}

bool DoubleBlock::InitBlock(SDL_Renderer* screen, const int& xp){
    int number = SDLCommonFunc::MyRandom(1, 10);
    int number2 = number;
    number *= 14;
    bool ret1 = m_Topblock.LoadImg("img//topblock.png", screen);
    bool ret2 = m_BottomBlock.LoadImg("img//bottomblock.png", screen);
    if (number2 % 2){
        m_Topblock.SetPos(xp, -300 - number);  //vi tri topblock //
        m_BottomBlock.SetPos(xp, GROUND_MAP - 220 - number); // vi tri bottom block //
    }
    else{
        m_Topblock.SetPos(xp, -300 + number);
        m_BottomBlock.SetPos(xp, GROUND_MAP - 220 + number);
    }
    if (ret1 && ret2) {
        return true;
    }
    return false;
}

void DoubleBlock::RenderImg(SDL_Renderer* screen){ //gọi render
    m_Topblock.ShowImg(screen);
    m_BottomBlock.ShowImg(screen);
}

void DoubleBlock::DoMoving(){ //di chuyển của vật thể
    m_BottomBlock.DoRun(xval);
    m_Topblock.DoRun(xval);
    if (m_Topblock.GetStateBack()||m_BottomBlock.GetStateBack()){
        is_back = true;
    }
}

SDL_Rect DoubleBlock::GetTopRect(){ //vị trí vật cản bên trên
    return m_Topblock.GetRectPos();
}

void DoubleBlock::SetRectVal(const UINT& xp){ //vị trí 2 vật cản
    m_Topblock.SetXPos(xp);
    m_BottomBlock.SetXPos(xp);
}

void DoubleBlock::SetIsBack(bool isBack){ //khi vật cản đi qua nhân vật
    is_back = isBack;
    m_Topblock.SetBack(isBack);
    m_BottomBlock.SetBack(isBack);
}

void DoubleBlock::GetRectSlot(){
    SDL_Rect recttop = m_Topblock.GetRectPos();
    slot_rect.x = recttop.x + recttop.w;
    slot_rect.y = recttop.y + recttop.h;
    slot_rect.w = 5;
    slot_rect.h = 150;
}

void DoubleBlock::DrawBound(SDL_Renderer* des){ //màu và viền vật cản
    GeometricFormat outlie_size(slot_rect.x, slot_rect.y, slot_rect.w, slot_rect.h);
    ColorData color_data1(255, 255, 255);
    Gemometric::RenderOutline(outlie_size, color_data1, des);
}

bool DoubleBlock::CheckPass(SDL_Rect rect){
    bool ret = false;
    ret = SDLCommonFunc::CheckCollision(rect, slot_rect);
    return ret;
}

bool DoubleBlock::CheckCol(SDL_Rect pl_rect){
    bool ret1 = SDLCommonFunc::CheckCollision(pl_rect, m_Topblock.GetRectPos());
    bool ret2 = SDLCommonFunc::CheckCollision(pl_rect, m_BottomBlock.GetRectPos());
    if (ret1 || ret2){
        return true;
    }
    return false;
}

//Block manager
BlockManager::BlockManager(){
    xpmax = 0;
    stopmove = false;
    m_count = 0;
    col = false;
}

BlockManager::~BlockManager(){
    FreeBlock();
}

void BlockManager::FreeBlock(){
    for (int i = 0; i < m_BlockList.size(); i++){
        DoubleBlock* posBlock = m_BlockList.at(i);
        delete posBlock;
        posBlock = NULL;
    }
    m_BlockList.clear();
}

bool BlockManager::InitBlockList(SDL_Renderer* screen){
    DoubleBlock* m_block1 = new DoubleBlock();
    DoubleBlock* m_block2 = new DoubleBlock();
    DoubleBlock* m_block3 = new DoubleBlock();
    DoubleBlock* m_block4 = new DoubleBlock();
    DoubleBlock* m_block5 = new DoubleBlock();
    DoubleBlock* m_block6 = new DoubleBlock();
    int ret = m_block1->InitBlock(screen, g_pos[0]);
    if (ret == false) return false;
    ret = m_block2->InitBlock(screen, g_pos[1]);
    if (ret == false) return false;
    ret = m_block3->InitBlock(screen, g_pos[2]);
    if (ret == false) return false;
    ret = m_block4->InitBlock(screen, g_pos[3]);
    if (ret == false) return false;
    ret = m_block5->InitBlock(screen, g_pos[4]);
    if (ret == false) return false;
    ret = m_block6->InitBlock(screen, g_pos[5]);
    if (ret == false) return false;

    m_BlockList.push_back(m_block1);
    m_BlockList.push_back(m_block2);
    m_BlockList.push_back(m_block3);
    m_BlockList.push_back(m_block4);
    m_BlockList.push_back(m_block5);
    m_BlockList.push_back(m_block6);

    xpmax = 5;
    return true;
}

void BlockManager::SetStopMoving(const bool& stop){
    stopmove = stop;
}

void BlockManager::Render(SDL_Renderer* screen){
    for (int i = 0; i < m_BlockList.size(); i++){
        DoubleBlock* posBlock = m_BlockList.at(i);
        posBlock->GetRectSlot();
        //posBlock->DrawBound(screen);
        if (!stopmove){
            posBlock->DoMoving();
            bool ret = posBlock->GetIsBack();
            if (ret){
                DoubleBlock* endBlock = m_BlockList.at(xpmax);
                SDL_Rect end_rect = endBlock->GetTopRect();
                UINT xp = end_rect.x + 250;
                posBlock->SetRectVal(xp);
                posBlock->SetIsBack(false);
                posBlock->SetIsPass(false);
                xpmax = i;
            }

            bool isCol = posBlock->CheckCol(player_rect);
            if (isCol){
                col = true;
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//punch.wav");
                if (beep_sound != NULL)
                    Mix_PlayChannel(-1, beep_sound, 0);
                Mix_Chunk* beep_sound1 = Mix_LoadWAV("sound//fall.wav");
                if (beep_sound1 != NULL)
                    Mix_PlayChannel(-1, beep_sound1, 0);
                break;
            }

            ret = posBlock->CheckPass(player_rect);
            if (ret){
                if (posBlock->GetIsPass() == false){
                    m_count++;
                    posBlock->SetIsPass(true);
                }
            }
        }
            posBlock->RenderImg(screen);
    }
}