#include "Hexapod.h"
#include "LegCtrl.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {

   try {
		//LegCtrl lc("/dev/servodrive0");
      //LegCtrl lc("servo.txt");
      LegCtrl lc("/dev/ttyACM0");

	
		if(argc > 1) {
			if(argv[1][0] == 'c') {
				for (int i=0; i<6; i++) {
					for (int j=0; j<3; j++) {
						lc.Calibrate(i,j);			
					}
				}

				for (int i=0; i<6; i++) {
					lc.Print(i,std::cout);	
						
				}
			} else if (argv[1][0] == 't') {
				lc.TestSetXYZ();
			} else {

			}
		} else {
			Hexapod h(50000,&lc);
			h.RunHexapod();
		}
    } catch(std::string const &e) {
        std::cout << "Exception caught: " << e << std::endl;
    } catch(char *e) {
        std::cout << "Exception caught: " << e << std::endl;
    }

	return 0;
}