#ifndef DWIN_HPP
#define DWIN_HPP

#include <dlib/gui_widgets.h>
#include <iostream>
#include <fstream>

class color_box : public dlib::draggable {
public:
    color_box(dlib::drawable_window &w, dlib::rectangle area, unsigned char red_, unsigned char green_, unsigned char blue_);
    ~color_box();
private:
    unsigned char red, green, blue;
    void draw(const dlib::canvas &c) const;
};

#endif      // DWIN_HPP