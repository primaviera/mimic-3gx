# mimic-3gx

3GX plugin for Miitopia (3DS), written for fun.

## plans

- [ ] Live randomizer
  - [x] Randomize battle BGM
  - [x] Randomize battle intro jingle
  - [x] Randomize Darker Lord's second phase BGM
  - [x] Randomize battle background
  - [x] Randomize level BGM
  - [ ] Randomize level background
  - [x] Randomize title screen BGM
  - [x] Randomize title screen background
  - [x] Randomize overworld BGM
  - [x] Randomize town BGM
  - [x] Randomize enemy attacks
  - [ ] Randomize enemies
  - [ ] Balance randomized enemy stats
  - [ ] Randomize grub stat boosts
- [ ] Setup custom skills
  - [x] Custom skill that targets an enemy
  - [ ] Custom skill that targets all enemies
  - [x] Custom skill that targets an ally
  - [x] Custom skill that targets all alies
  - [ ] Properly handle damage
  - [ ] Properly handle healing
- [ ] Controllable Party Members
  - [ ] Add patch
  - [ ] Store last menu selection for every party member
- [x] Configuration system (TOML)
- [ ] Limit amount of log files on SD

## known issues

### Closing the game causes a crash on real hardware

As of writing this, manually installing [CTRPluginFramework on the fb5bc8ec commit](https://gitlab.com/thepixellizeross/ctrpluginframework/-/tree/fb5bc8ec58f78e88f17d3f531971182642aec9ed) seems to fix the issue.
