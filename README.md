## Requirements

- ncurses library

---

## Installation

### Ubuntu
```bash
$ sudo apt install libncurses-dev
```

### Arch
```bash
$ sudo pacman -S ncurses
```

### Build and Run
```bash
$ gcc -ansi -o arcade -Wall arcade.c -lncurses
$ ./arcade
```
### Current Issues
- Boarder flickers when navigating menu
- Any keypresses refresh the page (need to filter out any keys that aren't supported)
