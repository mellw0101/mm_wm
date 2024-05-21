# mm_wm - Minimalist Window Manager

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)


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
    # NOTE: sudo is not needed if logged in as root # (It is highlly fround apon to run anything logged in as root (as opposed to using sudo to elivate priviliges) as this makes it very easy to get your network comprimised)
    
    sudo make depends # (to make dependencys based on distro (for now only 'apt/pacman' is supported))
    sudo make install # (to compile all source files into .o files then likes the .o files into the binary then installs mm_wm into /usr/local/bin (only run 'make' if you just want to comlile the binary without installing))

    # optional options
    sudo make help # (to list all make targets and helpers)
    sudo make alacritty # (copies a basic conf file into ~/.config/alacritty/alacritty.toml)
    sudo make xorg_conf # (copies a xorg.conf file into /etc/X11/xorg.conf)
    ```

## Set up startx

1. Make a backup of old ~/.xinitrc, to do this run command:
    ```sh
    cp ~/.xinitrc ~/.xinitrc.backup
    ```

2. Append 'exec mwm' into ~/.xinitrc:
    ```sh
    echo "exec mwm" > ~/.xinitrc
    ```

## Usage

### Run with 'startx' from tty (Simplest Method)
    startx

### Or

### Manually add mw_wm to your dm of choice
