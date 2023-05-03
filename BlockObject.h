
#ifndef BLOCK_OBJECT_H_
#define BLOCK_OBJECT_H_
#include "Geometric.h"
class BlockObject : BaseObject{
public:
    BlockObject();
    ~BlockObject();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void SetPos(const int& xp, const int& yp);
    void SetXPos(const int&xp);
    SDL_Rect GetRectPos() const { return this->GetRect(); }
    void ShowImg(SDL_Renderer* screen);
    void DoRun(UINT& x_val);
    bool GetStateBack() const { return is_back; }
    void SetBack(bool b) { is_back = b; }
private:
    bool is_back;
    int xpos;
    int ypos;
};

class DoubleBlock{
public:
    DoubleBlock();
    ~DoubleBlock();
    bool InitBlock(SDL_Renderer* screen, const int& xp);
    void RenderImg(SDL_Renderer* screen);
    void DoMoving();
    bool GetIsBack() const { return is_back; }
    void SetIsBack(bool isBack);
    SDL_Rect GetTopRect();
    void SetRectVal(const UINT& xp);
    void GetRectSlot();
    void DrawBound(SDL_Renderer* des);
    bool CheckPass(SDL_Rect rect);
    bool GetIsPass() const { return is_pass; }
    void SetIsPass(const bool& pp) { is_pass = pp; }
    bool CheckCol(SDL_Rect pl_rect);
private:
    BlockObject m_Topblock;
    BlockObject m_BottomBlock;
    UINT xval{};
    bool is_back;
    SDL_Rect slot_rect;
    bool is_pass;
};


class BlockManager{
public:
    BlockManager();
    ~BlockManager();
    bool InitBlockList(SDL_Renderer* screen);
    void Render(SDL_Renderer* screen);
    void SetStopMoving(const bool& stop);
    void SetPlayerRect(SDL_Rect pRect) { player_rect = pRect; }
    bool GetColState() const { return col; }
    INT GetCount() const { return m_count; }
    void FreeBlock();
private:
    std::vector<DoubleBlock*> m_BlockList;
    UINT xpmax;
    bool stopmove;
    SDL_Rect player_rect;
    INT m_count;
    bool col;
};
#endif