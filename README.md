# mimic-3gx

3GX plugin for Miitopia (3DS), written for fun.

Currently this plugin has a WIP live randomizer along with changes to a few job skills.

## plans

- [ ] Setup custom skills
- [ ] Balance enemy stats (Live Randomizer)
- [x] Configuration system (TOML)

## known issues

### "Closing the game causes a crash on real hardware"

This seems to be an issue with the plugin framework. If you are trying to compile this plugin yourself, you will probably have a better time [manually installing CTRPluginFramework with the "fb5bc8ec58f78e88f17d3f531971182642aec9ed" commit](https://gitlab.com/thepixellizeross/ctrpluginframework/-/commit/fb5bc8ec58f78e88f17d3f531971182642aec9ed)
