#include "Hexapod.h"
#include "LegCtrl.h"
#include <string>
#include <iostream>

int main() {

   try {
      LegCtrl lc("/dev/servodrive0");
      //LegCtrl lc("servo.txt");
      Hexapod h(50000,&lc);
	   h.RunHexapod();
    } catch(std::string const &e) {
        std::cout << "Exception caught: " << e << std::endl;
    } catch(char *e) {
        std::cout << "Exception caught: " << e << std::endl;
    }

	return 0;
}
