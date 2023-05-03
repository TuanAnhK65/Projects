#include "ImpTimer.h"
#include "CommonFunc.h"

ImpTimer::ImpTimer(){
  tickstart = 0;
  tickpause = 0;
  is_paused_ = false;
  is_started_ = false;
}

void ImpTimer::start(){
  is_started_ = true;
  is_paused_ = false;
  tickstart = SDL_GetTicks();
}

void ImpTimer::stop(){
  is_started_ = false;
  is_paused_ = false;
}

void ImpTimer::pause(){
  if( (is_started_) && (!is_paused_) ){
    is_paused_ = true;
    tickpause = SDL_GetTicks() - tickstart;
  }
}

void ImpTimer::unpause(){
  if( is_paused_){
    is_paused_ = false;
    //Đặt lại dấu tích bắt đầu
    tickstart = SDL_GetTicks() - tickpause;
    //Đặt lại dấu tích dừng lại
    tickpause = 0;
  }
}

int ImpTimer::get_ticks()
{
  if(is_started_){
    //Nếu hẹn giờ dừng 
    if(is_paused_){
      return tickpause;
    }
    else{
      return SDL_GetTicks() - tickstart;
    }
  }
  //Nếu hẹn giờ không chạy tiếp
  return 0;
}

bool ImpTimer::is_started(){
  return is_started_;
}

bool ImpTimer::is_paused(){
  return is_paused_;
}