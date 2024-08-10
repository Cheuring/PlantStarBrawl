#ifndef _ATLAS_H_
#define _ATLAS_H_

#include <vector>
#include <graphics.h>
#include <stdexcept>

class Atlas
{
public:
    Atlas() = default;
    ~Atlas() = default;

    void load_from_file(LPCTSTR path_template, int num) {
        img_list.clear();
        img_list.resize(num);

        TCHAR path_file[256];
        for (int i = 0; i < num; i++) {
            _stprintf_s(path_file, path_template, i + 1);
            loadimage(&img_list[i], path_file);
        }
    }

    void clear(){
        img_list.clear();
    }

    int get_size() const {
        return img_list.size();
    }

    IMAGE* get_image(int index) {
        if(index < 0 || index >= img_list.size()){
            return nullptr;
        }
        return &img_list[index];
    }

    IMAGE& operator[](int index) {
        if(index < 0 || index >= img_list.size()){
            throw std::out_of_range("index out of range");
        }
        return img_list[index];
    }

    const IMAGE& operator[](int index) const {
        if(index < 0 || index >= img_list.size()){
            throw std::out_of_range("index out of range");
        }
        return img_list[index];
    }

    void add_image(const IMAGE& img) {
        img_list.push_back(img);
    }
private:
    std::vector<IMAGE> img_list;
};


#endif // _ATLAS_H_