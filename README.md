# regmap-manager

[![Test](https://github.com/shalex88/regmap-manager/actions/workflows/test.yml/badge.svg)](https://github.com/shalex88/regmap-manager/actions/workflows/test.yml)
[![Coverage](https://img.shields.io/codecov/c/github/shalex88/regmap-manager)](https://codecov.io/github/shalex88/regmap-manager)
[![Release](https://img.shields.io/github/v/release/shalex88/regmap-manager.svg)](https://github.com/shalex88/regmap-manager/releases/latest)

## Proposed design

* Easy FW/SW integration. Automatic EXCEL->CSV
* Easily maintain CSV version inside the project
* Easily refer to enums generated from the CSV
* Compile time parsing is faster
* Compile time parsing will not cause runtime errors and memory leaks
* No need to store the registers map on target
* Writing to unknows addresses is prohibited
* Wide functionality for register, bit, nibble manipulation 

## TODO

* Handle multiple register maps
* Handle ready only registers
