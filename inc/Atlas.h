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

    void LoadFromFile(LPCTSTR path_template, int num) ;
    void Clear();
    auto GetSize() const -> int ;
    auto GetImage(int index) -> IMAGE* ;
    void AddImage(IMAGE& img) ;

private:
    std::vector<IMAGE> img_list;
};


#endif // _ATLAS_H_