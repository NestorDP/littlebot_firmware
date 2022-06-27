# littlebot_firmware
Source code littlebot firmware 

## Setup the environment

  ### ARM GCC Compiler
  
  Download arm-none-eabi compiler
  ```sh
  export PATH=$PATH:/home/nestor/Documents/littlebot/firmware/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin
  ```

  ### Lm4flash
  ```sh
  git clone https://github.com/utzig/lm4tools.git
  cd lm4tools/lm4ﬂash
  make
  sudo cp lm4flash /usr/local/bin
  ```
talvex precise dar permissão à porta USB sudo chmod /dev/bus/usb/00x/0xx (x são os números da porta)
  ### TivaWare driver
  Dwonload SW-TM4C: TivaWare for C Series Software
   ```sh
  mkdir <TivaWarePah>
  cd <TivaWarePah>
  mv <directory_downloaded>/SW-TM4C-2.1.1.71.exe
  unzip SW-TM4C-2.1.1.71.exe
  rm SW-TM4C-2.1.1.71.exe
  export TIVAWARE_PATH=/your/tivaware/path
   ```