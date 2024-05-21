# mm_wm - Minimalist Window Manager

## Description

mm_wm is a minimalist window manager built exclusively with XCB. This design choice ensures superior response times from the X server by reducing the number of requests sent, making mm_wm significantly faster compared to those using Xlib. By leveraging XCB's efficient and asynchronous communication capabilities, mm_wm offers a highly responsive and lightweight window management experience.

## Features

- **Lightweight Design**: Minimal resource usage for smooth and responsive performance.
- **Customization**: Easily modifiable environment through configuration files.
- **Compatibility**: Seamless integration with common X11 applications.
- **User-Friendly**: Intuitive interface with a minimal learning curve.
- **Stability**: Robust and reliable performance with thorough testing.

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/mellw0101/mm_wm.git
    ```

2. Navigate to the project directory:
    ```sh
    cd mm_wm
    ```

3. Build the project:
    ```sh
    # NOTE: sudo is not needed if logged in as root
    
    sudo make depends # (to make dependencys based on distro (for now only 'apt/pacman' is supported))
    sudo make install # (to compile all source files into .o files then likes the .o files into the binary then installs mm_wm into /usr/local/bin (only run 'make' if you just want to comlile the binary without installing))

    # optional options
    sudo make help # (to list all make targets and helpers)
    sudo make alacritty # (copies a basic conf file into ~/.config/alacritty/alacritty.toml)
    sudo make xorg_conf # (copies a xorg.conf file into /etc/X11/xorg.conf)
    ```

## Usage
```sh
To start using mm_wm, append 'exec mwm' into ~/.xinitrc then run startx from tty