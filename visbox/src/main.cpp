#include <dlib/gui_widgets.h>
#include <iostream>

using namespace std;
using namespace dlib;

namespace odds {
    class win : public drawable_window {
    public:
        win() : mbar(*this) {
            
            create_menu();

            set_size(640, 480);
            set_title("visbox");
            show();
        }
    private:
        menu_bar mbar;
        
        void on_new_key_clicked() {

        }

        void create_menu() {
            mbar.set_number_of_menus(2);
            mbar.set_menu_name(0, "File", 'F');
            mbar.menu(0).add_menu_item(menu_item_text("new key", *this, &win::on_new_key_clicked, 'K'));
            mbar.set_menu_name(1, "Help", 'H');
        }
    };
}

int main(int argc, char **argv) {
    /*
    dwin my_win;

    my_win.wait_until_closed();
    */

    return 0;
}