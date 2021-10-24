# qmlgreet

QML [greetd](https://git.sr.ht/~kennylevinsen/greetd) greeter.

![Screenshot](https://github.com/nowrep/qmlgreet/blob/master/doc/screenshot.jpg?raw=true)


## Building

    mkdir build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr ..
    make && make install

## Configuration

    Usage: qmlgreet [options]

    Options:
      -u, --user <user>        User.
      -i, --icons <path>       User icons directory.
      -b, --background <path>  Background.
      -c, --command <command>  Command.

**Running with eglfs platform**

It needs access to GPU and input devices. On ArchLinux that means the *greeter* user needs to be in `video` and `input` groups.

**Example greetd config**

```toml
command = "qmlgreet --platform eglfs --user your-username --icons /usr/share/qmlgreet/icons --background /usr/share/qmlgreet/wallpaper.png --command sway"
```
