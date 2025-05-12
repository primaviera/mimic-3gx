# mimic-3gx

3GX plugin for Miitopia (3DS), written for fun.

This plugin has only been used on the US version of Miitopia, other versions might have bugs.

And most importantly, this is all WIP. You can see what features have been added by looking at the [TODO list](https://github.com/primaviera/mimic-3gx/issues/1).

## Building

You need the following:

- [devkitPro/devkitARM](https://devkitpro.org)
- [libctru](https://github.com/devkitPro/libctru)
- [CTRPluginFramework](https://gitlab.com/thepixellizeross/ctrpluginframework)
- [3gxtool](https://gitlab.com/thepixellizeross/3gxtool)

Make sure to clone this repo recursively (``git clone --recursive https://github.com/primaviera/mimic-3gx``)

After that, you can just use ``make``.

## Known Issues

### Closing Miitopia crashes the console

As of writing this (2025/05/07), manually installing [CTRPluginFramework on the fb5bc8ec commit](https://gitlab.com/thepixellizeross/ctrpluginframework/-/tree/fb5bc8ec58f78e88f17d3f531971182642aec9ed) to compile the plugin seems to fix the issue, this could change in the future.
