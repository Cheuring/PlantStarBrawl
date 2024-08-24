#include "Animation.h"

void Animation::Reset(){
    timer = 0;
    frame_index = 0;
}

void Animation::SetAtlas(Atlas* atlas){
    Reset();
    this->atlas = atlas;
}

void Animation::SetLoop(bool is_loop){
    this->is_loop = is_loop;
}

void Animation::SetInterval(int interval){
    this->interval = interval;
}

auto Animation::GetFrameIndex() const -> int {
    return frame_index;
}

auto Animation::GetCurrentFrame() const -> IMAGE* {
    if(atlas == nullptr){
        return nullptr;
    }
    return atlas->GetImage(frame_index);
}

auto Animation::CheckFinish() const -> bool {
    if(is_loop){
        return false;
    }
    return frame_index == atlas->GetSize() - 1;
}

void Animation::OnUpdate(int delta){
    if(atlas == nullptr){
        return;
    }
    timer += delta;
    if(timer >= interval){
        timer = 0;
        frame_index++;
        if(frame_index >= atlas->GetSize()){
            if(is_loop){
                frame_index = 0;
            }else{
                if(on_finish != nullptr){
                    on_finish();
                }
                frame_index = atlas->GetSize() - 1;
            }
        }
    }
}

void Animation::OnDraw(const Camera& camera, int x, int y) const {
    if(atlas == nullptr){
        return;
    }
    PutImageAlpha(camera, x, y, atlas->GetImage(frame_index));
}

void Animation::SetOnFinish(std::function<void()> on_finish){
    this->on_finish = on_finish;
}