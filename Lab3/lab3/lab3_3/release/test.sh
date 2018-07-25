#!/bin/bash

rtl_fm -f 97e6 -M wbfm -s 2000000 -r 48000 - | aplay -r 48k -f S16_LE
