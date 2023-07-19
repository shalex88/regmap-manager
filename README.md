# regmap-manager

[![Build and Run](https://github.com/shalex88/regmap-manager/actions/workflows/test.yml/badge.svg)](https://github.com/shalex88/reg-manager/actions/workflows/test.yml)

## Current design

* Difficult FW/SW integration. Manual EXCEL->XML
* Difficulty to maintain XML version inside the project/on target
* Not able to maintain XML and enums at the same time
* Runtime parsing is slower
* Runtime parsing can cause runtime errors and memory leaks
* Runtime parsing will fail if the file is not present on target
* Writing to unknows addresses is allowed
* Unsupervided writing to registers by vector
* Only setting the full reg value is available

## Proposed design

* Easy FW/SW integration. Automatic EXCEL->CSV
* Easily maintain CSV version inside the project
* Easily refer to enums generated from the CSV
* Compile time parsing is faster
* Compile time parsing will not cause runtime errors and memory leaks
* No need to store the registers map on target
* Writing to unknows addresses is prohibited
* Wide functionality for register, bit, nibble manipulation 

## Review

* Windows compilation
* Memory footprint
* Memory map injection to the manager
