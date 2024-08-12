#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <functional>
#include <graphics.h>

#include "Atlas.h"
#include "util.h"

class Animation
{
public:
    Animation() = default;
    ~Animation() = default;

    void reset(){
        timer = 0;
        frame_index = 0;
    }

    void set_atlas(Atlas* atlas){
        reset();
        this->atlas = atlas;
    }

    void set_loop(bool is_loop){
        this->is_loop = is_loop;
    }

    void set_interval(int interval){
        this->interval = interval;
    }

    int get_frame_index() const {
        return frame_index;
    }

    IMAGE* get_current_frame() const {
        if(atlas == nullptr){
            return nullptr;
        }
        return atlas->get_image(frame_index);
    }

    bool check_finish() const {
        if(is_loop){
            return false;
        }
        return frame_index == atlas->get_size() - 1;
    }

    void on_update(int delta){
        if(atlas == nullptr){
            return;
        }
        timer += delta;
        if(timer >= interval){
            timer = 0;
            frame_index++;
            if(frame_index >= atlas->get_size()){
                if(is_loop){
                    frame_index = 0;
                }else{
                    if(on_finish != nullptr){
                        on_finish();
                    }
                    frame_index = atlas->get_size() - 1;
                }
            }
        }
    }

    void on_draw(int x, int y) const {
        if(atlas == nullptr){
            return;
        }
        put_image_alpha(x, y, atlas->get_image(frame_index));
    }

    void set_on_finish(std::function<void()> on_finish){
        this->on_finish = on_finish;
    }

private:
    int timer = 0;
    int interval = 0;
    int frame_index = 0;
    bool is_loop = true;
    Atlas* atlas = nullptr;
    std::function<void()> on_finish = nullptr;

};



#endif // _ANIMATION_H_