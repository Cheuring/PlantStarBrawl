#include "Atlas.h"

void Atlas::LoadFromFile(LPCTSTR path_template, int num) {
    img_list.clear();
    img_list.resize(num);

    TCHAR path_file[256];
    for (int i = 0; i < num; i++) {
        _stprintf_s(path_file, path_template, i + 1);
        loadimage(&img_list[i], path_file);
    }
}

void Atlas::Clear() {
    img_list.clear();
}

auto Atlas::GetSize() const -> int {
    return img_list.size();
}

auto Atlas::GetImage(int index) -> IMAGE* {
    if (index < 0 || index >= img_list.size()) {
        return nullptr;
    }
    return &img_list[index];
}

void Atlas::AddImage(IMAGE& img) {
    img_list.emplace_back(std::move(img));
}