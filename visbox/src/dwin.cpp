#include "../inc/dwin.hpp"

using namespace std;
using namespace dlib;

class color_box : public draggable {
    unsigned char red, green, blue;
public:
    color_box(
        drawable_window &w, rectangle area, unsigned char red_, 
        unsigned char green_, unsigned char blue_
    ) : 
        draggable(w),
        red(red_),
        green(green_),
        blue(blue_)
    {
        rect = area;
        set_draggable_area(rectangle(10, 10, 400, 400));

        enable_events();
    }

    ~color_box() {
        disable_events();

        parent.invalidate_rectangle(rect);
    }
private:
    void draw(const canvas &c) const {
        rectangle area = c.intersect(rect);
        if(area.is_empty() == true) {
            return;
        }

        fill_rect(c, rect, rgb_pixel(red, green, blue));
    }
};

class dwin : public drawable_window {
public:
    dwin() : 
        c(*this),
        b(*this),
        cb(*this, rectangle(100, 100, 200, 200), 0, 0, 255),
        mbar(*this) {
            // put the button at position (10, 60)
            b.set_pos(10, 60);
            b.set_name("button");

            // put the label 5 pixels below the button
            c.set_pos(b.left(), b.bottom() + 5);

            // set which function should get called when button is clicked
            b.set_click_handler(*this, &dwin::on_button_clicked);
            // akternative lambda listener for C++0x support only.
            /*
            b.set_click_handler([&]() {
                counter++;
                ostringstream sout;
                sout << "counter: " << counter;
                c.set_text(sout.str());
            });
            */

            // simple menu bar
            mbar.set_number_of_menus(1);
            // menu name
            mbar.set_menu_name(0, "Menu", 'M');
            mbar.menu(0).add_menu_item(menu_item_text("click button!", *this, &dwin::on_button_clicked, 'C'));
            mbar.menu(0).add_menu_item(menu_item_separator());
            mbar.menu(0).add_menu_item(menu_item_text("about", *this, &dwin::show_about, 'A'));

            // set window size
            set_size(640, 480);

            counter = 0;

            set_title("dlib gui");
            show();
        }

        ~dwin() {
            close_window();
        }

private:
        unsigned long counter;
        label c;
        button b;
        color_box cb;
        menu_bar mbar;

        void on_button_clicked() {
            counter++;
            ostringstream sout;
            sout << "counter: " << counter;
            c.set_text(sout.str());
        }

        void show_about() {
            message_box("about", "this is a dlib gui example program");
        }
};