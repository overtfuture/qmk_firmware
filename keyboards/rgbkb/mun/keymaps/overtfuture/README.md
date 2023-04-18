# Flashing

```shell
qmk compile

# Flash Left
# Plug in the left most plug
qmk flash -kb rgbkb/mun -km overtfuture -bl dfu-util-split-left

# Flash Right
# Plug in the right most plug
qmk flash -kb rgbkb/mun -km overtfuture -bl dfu-util-split-right

```