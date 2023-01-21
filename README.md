# PBDoom
Doom for PocketBook eInk devices.

## Installation
1. Download the game from the releases page https://github.com/imustafin/pbdoom/releases
2. Unzip the archive to the `applications` directory on the ereader. You should get
  this structure:
  ```
  applications
  ├── pbdoom
  │   └── pbdoom.app
  └── pbdoom.app
  ```
3. Place your WADs in the `applications/pbdoom` directory. WAD filenames should
   be in lower case (not `DOOM2.WAD` but `doom2.wad`)
4. Run the game using the `pbdoom` icon from the ereader applications menu

## Development process
The following describes a development process for building the game on Linux.

### Building
1. Obtain the SDK (we use https://github.com/c3pio-man/SDK_6.3.0/releases/tag/6.3.2)
2. Write the path to SDK to `sdk_path` (e.g. `../SDK_6.3.0-6.3.2/SDK-A13`)
3. Run `./build.sh`
  * The executable `pbdoom.app` will be located in the `build` dir

### App sender-receiver
To make the development more convenient we can use the approach of
https://github.com/pmartin/pocketbook-demo/tree/master/app-sender-receiver

The general idea is to use `nc` to exchange data between the development computer
and the ebook.

After building the executable we can send it to the device using `send.sh`.

Helper program `pbdoom-get.app` receives the executable sent by `send.sh`,
runs the game and redirects the output to the computer using `nc`.

To see the output from `pbdoom-get.app` have `mon.sh` running.

Don't forget to update ip addresses and ports in all scripts.

Check the ereader's ip in wifi settings for the connected network.
