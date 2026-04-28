## Requirements

- NCURSES library

---

## To Install NCURSES

### Ubuntu
```bash
$ sudo apt install libncurses-dev
```

### Arch
```bash
$ sudo pacman -S ncurses
```
--- 

## To Build and Run
```bash
$ gcc -ansi -o arcade -Wall arcade.c tictactoe.c blackjack.c -lncurses
$ ./arcade
```
or
```bash
$ make
$ ./arcade
```

--- 
## Other Notes

### Current Issues
- Boarder flickers when navigating menu
- Any keypresses refresh the page (need to filter out any keys that aren't supported)

### Sources and Documentation Used
- [NCURSES MAN page](https://www.man7.org/linux/man-pages/man3/ncurses.3x.html)
- [NCURSES Programming How-to](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
- [Make File Tutorial (Provided in class)](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/)
- [YouTube NCURSES TUTORIAL by Casual Coder](https://youtu.be/lV-OPQhPvSM?si=Bk2K55bgXVdDGRJc)
