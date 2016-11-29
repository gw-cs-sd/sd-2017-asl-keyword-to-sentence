//
//  test.cpp
//  sdproject
//
//  Created by Andy Acosta on 11/15/16.
//  Copyright © 2016 Andy Acosta. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <unistd.h>


int main(){
    
    int t = 10;
    
    initscr();
    //printw("Press any key to start!");
    //getch();
    //cbreak();
    
    while(t--){
    
        initscr();
        printw("Press any key to start!");
        while(true){
            if(getch() == 's') {
                printw("you pressed S. end loop\n");
                break;
            }
            printw("waiting");
        }
        //getch();
        cbreak();
        noecho();
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        while (true) {
            if (getch() == 'x') {
                printw("You pressed G. end loop\n");
                break;
            }
            printw("Running\n");
            napms(250);
        }
    }
    endwin();

    std::cout << "Done!" << std::endl;
    
}
